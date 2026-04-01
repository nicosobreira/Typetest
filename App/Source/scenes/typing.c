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

static const double SECONDS_FOR_CLOCK_UPDATE = 500.0;

static void handleBackspace(TypingData *pData)
{
    if (!String_IsIndexValid(&pData->textEntry.text, pData->pointerText - 1))
        return;

    Cursor_MoveLeft(&pData->cursor, pData->windowText);

    pData->pointerText--;

    wchar_t textChar = String_GetChar(&pData->textEntry.text, pData->pointerText);

    if (textChar == StackChar_Top(&pData->inputBuffer))
        pData->score.correctLetters--;

    StackChar_Pop(&pData->inputBuffer);

    mvwaddnwstr(pData->windowText, pData->cursor.y, pData->cursor.x, &textChar, 1);
}

static void handleCharacterInput(TypingData *pData, wint_t key)
{
    if (!String_IsIndexValid(&pData->textEntry.text, pData->pointerText + 1))
        return;

    wchar_t character = (wchar_t)key;
    StackChar_Push(&pData->inputBuffer, character);

    wchar_t textChar = String_GetChar(&pData->textEntry.text, pData->pointerText);

    if (character == textChar)
    {
        pData->score.correctLetters++;
        COLOR_ON(pData->windowText, COLOR_GREEN);
    }
    else
    {
        pData->score.wrongLetters++;
        COLOR_ON(pData->windowText, COLOR_RED);
    }

    const wchar_t spaceChar = L'_';

    if (textChar == L' ')
    {
        textChar = spaceChar;
    }

    mvwaddnwstr(pData->windowText, pData->cursor.y, pData->cursor.x, &textChar, 1);

    COLOR_CLEAR(pData->windowText);

    // NOTE: Game win
    if (pData->score.correctLetters >= (int)pData->textEntry.text.length)
    {
        SceneManager_Switch(SCENE_SCORE);
        return;
    }

    pData->pointerText++;

    Cursor_MoveRight(&pData->cursor, pData->windowText);
}

// TODO: Add support for ENTER
static void compareInputText(TypingData *pData, wint_t key)
{
    switch (key)
    {
    case KEY_CODE_BACKSPACE:
        handleBackspace(pData);
        break;
    case KEY_CODE_ESCAPE:
        SceneManager_Switch(SCENE_MENU);
        break;
    case KEY_CODE_ENTER:
        break;
    default:
        handleCharacterInput(pData, key);
        break;
    }
}

static void drawEntrySpeed(TypingData *pData)
{
    WINDOW *win = pData->windowStatus;

    int maxY = getmaxy(win);

    mvwprintw(win, maxY - 2, 2, "Speed: %.0f WPM", pData->score.wordsPerMinute);
}

static int getPercentageColor(int percentage)
{
    if (percentage < 30)
    {
        return COLOR_RED;
    }
    else if (percentage < 60)
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

static void statusDraw(TypingData *pData)
{
    WINDOW *win = pData->windowStatus;

    werase(win);
    box(win, 0, 0);

    int maxX = getmaxx(win);

    // Title
    const char *title = "STATUS";
    mvwprintw(win, 1, (maxX - strlen(title)) / 2, "%s", title);

    mvwhline(win, 2, 1, ACS_HLINE, maxX - 2);

    int percentage = 100 * pData->score.correctLetters / pData->textEntry.text.length;

    drawPercentage(win, percentage);

    drawEntrySpeed(pData);

    wrefresh(win);
}

static void calculateCharsPerSecond(TypingData *pData)
{
    double deltaChars = (double)(pData->score.correctLetters);
    double deltaTime = Clock_Get(&pData->score.miliSeconds) / 1000.0;

    pData->score.charsPerSecond = deltaChars / deltaTime;
    pData->score.wordsPerMinute = pData->score.charsPerSecond * (60.0 / 5.0);
}

void Typing_OnEnter(void *data)
{
    TypingData *pData = (TypingData *)data;

    curs_set(TRUE);

    StackChar_Free(&pData->inputBuffer);

    pData->textEntry = TextEntry_RandomText();
    pData->pointerText = 0;

    Cursor_Reset(&pData->cursor);

    Clock_Set(&pData->score.miliSeconds, SECONDS_FOR_CLOCK_UPDATE);
    pData->score.charsPerSecond = 0.0;
    pData->score.wordsPerMinute = 0.0;
    pData->score.wrongLetters = 0;
    pData->score.correctLetters = 0;

    Window_DrawString(pData->windowText, &pData->textEntry.text);

    pData->shouldDraw = true;
}

void Typing_OnExit(void *data)
{
    TypingData *pData = (TypingData *)data;

    pData->shouldDraw = false;

    werase(pData->windowText);
    wrefresh(pData->windowText);

    werase(pData->windowStatus);
    wrefresh(pData->windowStatus);
}

void Typing_Input(void *data)
{
    TypingData *pData = (TypingData *)data;

    wint_t key;
    int hasKeyPressed = wget_wch(pData->windowText, &key);

    if (hasKeyPressed == ERR)
    {
        pData->shouldDraw = false;
        return;
    }

    pData->shouldDraw = true;

    compareInputText(pData, key);
}

void Typing_Update(void *data)
{
    TypingData *pData = (TypingData *)data;

    if (Clock_Tick(&pData->score.miliSeconds))
    {
        calculateCharsPerSecond(data);
        pData->shouldDraw = true;
    }
}

void Typing_Draw(void *data)
{
    TypingData *pData = (TypingData *)data;

    statusDraw(data);

    Cursor_Draw(&pData->cursor, pData->windowText);

    wrefresh(pData->windowStatus);
    wrefresh(pData->windowText);
}

void Typing_Free(void *data)
{
    TypingData *pData = (TypingData *)data;

    StackChar_Free(&pData->inputBuffer);

    delwin(pData->windowText);
    delwin(pData->windowStatus);

    free(data);
    data = NULL;
}

Scene Typing_Scene(void)
{
    TypingData *pData = malloc(sizeof(TypingData));

    Scene scene = {.OnEnter = Typing_OnEnter,
                   .OnExit = Typing_OnExit,
                   .Input = Typing_Input,
                   .Update = Typing_Update,
                   .Draw = Typing_Draw,
                   .Free = Typing_Free,
                   .pData = pData,
                   .name = SCENE_TYPING};

    pData->windowText = Window_New(stdscr, WINDOW_LAYOUT_CENTER, WINDOW_ALIGN_NULL);

    nodelay(pData->windowText, TRUE);
    notimeout(pData->windowText, TRUE);

    pData->windowStatus = Window_New(pData->windowText, WINDOW_LAYOUT_ON_TOP, WINDOW_ALIGN_LEFT);

    StackChar_Init(&pData->inputBuffer);

    return scene;
}
