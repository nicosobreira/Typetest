#include "scenes/typing.h"

#include <stdlib.h>
#include <string.h>

#include "Core/constants/key_codes.h"
#include "Core/scenes/manager.h"
#include "Core/ui/color.h"
#include "Core/ui/cursor.h"
#include "Core/ui/window.h"

#include "scenes/menu.h"
#include "scenes/score.h"

static void handleBackspace(TypingData *self)
{
    if (!String_IsIndexValid(&self->entry.text, self->pointerText - 1))
        return;

    Cursor_MoveLeft(&self->cursor, self->windowText);

    self->pointerText--;

    wchar_t textChar = String_GetChar(&self->entry.text, self->pointerText);

    if (textChar == StackChar_Top(&self->input))
        self->score.correctLetters--;

    StackChar_Pop(&self->input);

    mvwaddnwstr(self->windowText, self->cursor.y, self->cursor.x, &textChar, 1);
}

static void handleCharacterInput(TypingData *self, wint_t key)
{
    if (!String_IsIndexValid(&self->entry.text, self->pointerText + 1))
        return;

    wchar_t character = (wchar_t)key;
    StackChar_Push(&self->input, character);

    wchar_t textChar = String_GetChar(&self->entry.text, self->pointerText);

    if (character == textChar)
    {
        self->score.correctLetters++;
        COLOR_ON(self->windowText, COLOR_GREEN);
    }
    else
    {
        self->score.wrongLetters++;
        COLOR_ON(self->windowText, COLOR_RED);
    }

    const wchar_t spaceChar = L'_';

    if (textChar == L' ')
    {
        textChar = spaceChar;
    }

    mvwaddnwstr(self->windowText, self->cursor.y, self->cursor.x, &textChar, 1);

    COLOR_CLEAR(self->windowText);

    // NOTE: Game win
    if (self->score.correctLetters >= (int)self->entry.text.length)
    {
        SceneManager_Switch(SCENE_SCORE);
        return;
    }

    self->pointerText++;

    Cursor_MoveRight(&self->cursor, self->windowText);
}

// TODO: Add support for ENTER
static void compareInputText(TypingData *self, wint_t key)
{
    switch (key)
    {
    case KEY_CODE_BACKSPACE:
        handleBackspace(self);
        break;
    case KEY_CODE_ESCAPE:
        SceneManager_Switch(SCENE_MENU);
        break;
    case KEY_CODE_ENTER:
        break;
    default:
        handleCharacterInput(self, key);
        break;
    }
}

static void drawEntrySpeed(TypingData *self)
{
    WINDOW *win = self->windowStatus;

    int maxY = getmaxy(win);

    double wpm = self->score.wordsPerMinute;
    double totalTime = Clock_Get(&self->score.totalTimeMs) / 1000.0;

    mvwprintw(win, maxY - 2, 2, "Speed: %.0f WPM\t\tTime: %.0fs", wpm, totalTime);
}

static int getPercentageColor(int percentage)
{
    const int low = 30;
    const int medium = 60;

    if (percentage < low)
    {
        return COLOR_RED;
    }
    else if (percentage < medium)
    {
        return COLOR_YELLOW;
    }
    else
    {
        return COLOR_GREEN;
    }
}

static void drawPercentage(WINDOW *win, int percentage)
{
    int maxX = getmaxx(win);

    int startX = 2;
    int startY = 3;

    int barWidth = maxX - 4;

    if (percentage < 0)
        percentage = 0;
    if (percentage > 100)
        percentage = 100;

    int fill = (percentage * (barWidth - 2)) / 100;

    mvwaddch(win, startY, startX, '[');
    mvwaddch(win, startY, startX + barWidth - 1, ']');

    int color = getPercentageColor(percentage);
    COLOR_ON(win, color);

    for (int i = 0; i < fill; i++)
    {
        mvwaddch(win, startY, startX + 1 + i, '=');
    }

    if (fill < barWidth - 2)
    {
        mvwaddch(win, startY, startX + 1 + fill, '>');
    }

    COLOR_CLEAR(win);

    mvwprintw(win, startY + 1, startX, "%d%%", percentage);
}

static void statusDraw(TypingData *self)
{
    WINDOW *win = self->windowStatus;

    werase(win);
    box(win, 0, 0);

    int maxX = getmaxx(win);

    // Title
    const char *title = "STATUS";
    mvwprintw(win, 1, (maxX - strlen(title)) / 2, "%s", title);

    mvwhline(win, 2, 1, ACS_HLINE, maxX - 2);

    int percentage = 100 * self->score.correctLetters / self->entry.text.length;

    drawPercentage(win, percentage);

    drawEntrySpeed(self);

    wrefresh(win);
}

static void calculateCharsPerSecond(TypingData *self)
{
    double deltaChars = (double)(self->score.correctLetters);
    double deltaTime = Clock_Get(&self->score.totalTimeMs) / 1000.0;

    if (deltaTime > 0.0)
    {
        self->score.charsPerSecond = deltaChars / deltaTime;
    }
    else
    {
        self->score.charsPerSecond = 0.0;
    }

    self->score.wordsPerMinute = self->score.charsPerSecond * (60.0 / 5.0);
}

void Typing_OnEnter(void *data)
{
    TypingData *self = (TypingData *)data;

    curs_set(TRUE);

    StackChar_Clear(&self->input);

    self->entry = TextEntry_RandomText();
    self->pointerText = 0;

    Cursor_Reset(&self->cursor);

    Clock_Set(&self->score.totalTimeMs, GAME_TICK_MS);
    self->score.charsPerSecond = 0.0;
    self->score.wordsPerMinute = 0.0;
    self->score.wrongLetters = 0;
    self->score.correctLetters = 0;

    Window_DrawString(self->windowText, &self->entry.text);

    self->shouldDraw = true;
}

void Typing_OnExit(void *data)
{
    TypingData *self = (TypingData *)data;

    self->shouldDraw = false;

    werase(self->windowText);
    wrefresh(self->windowText);

    werase(self->windowStatus);
    wrefresh(self->windowStatus);
}

void Typing_Input(void *data)
{
    TypingData *self = (TypingData *)data;

    wint_t key;
    int hasKeyPressed = wget_wch(self->windowText, &key);

    if (hasKeyPressed == ERR)
    {
        return;
    }

    self->shouldDraw = true;

    compareInputText(self, key);
}

void Typing_Update(void *data)
{
    TypingData *self = (TypingData *)data;

    if (!Clock_Tick(&self->score.totalTimeMs))
        return;

    self->shouldDraw = true;
    calculateCharsPerSecond(data);
}

void Typing_Draw(void *data)
{
    TypingData *self = (TypingData *)data;

    if (!self->shouldDraw)
        return;

    statusDraw(data);

    Cursor_Draw(&self->cursor, self->windowText);

    wrefresh(self->windowStatus);
    wrefresh(self->windowText);

    self->shouldDraw = false;
}

void Typing_Free(void *data)
{
    TypingData *self = (TypingData *)data;

    StackChar_Free(&self->input);

    delwin(self->windowText);
    delwin(self->windowStatus);

    free(data);
    data = NULL;
}

Scene Typing_Scene(void)
{
    TypingData *self = malloc(sizeof(TypingData));

    Scene scene = {
        .name = SCENE_TYPING,
        .pData = self,
        .OnEnter = Typing_OnEnter,
        .OnExit = Typing_OnExit,
        .Input = Typing_Input,
        .Update = Typing_Update,
        .Draw = Typing_Draw,
        .Free = Typing_Free,
    };

    self->windowText = Window_New(stdscr, WINDOW_LAYOUT_CENTER, WINDOW_ALIGN_NULL);

    nodelay(self->windowText, TRUE);
    notimeout(self->windowText, TRUE);

    self->windowStatus = Window_New(self->windowText, WINDOW_LAYOUT_ON_TOP, WINDOW_ALIGN_LEFT);

    StackChar_Init(&self->input);

    return scene;
}
