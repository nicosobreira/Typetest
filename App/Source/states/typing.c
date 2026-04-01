#include "states/typing.h"

#include <string.h>

#include "Core/constants/key_codes.h"
#include "Core/manager/game_manager.h"
#include "Core/ui/color.h"
#include "Core/ui/cursor.h"
#include "Core/ui/window.h"

#include "states/id.h"

static const double SECONDS_FOR_CLOCK_UPDATE = 500.0;

static void handleBackspace(GameManager *sm)
{
    TypingData *data = (TypingData *)GameManager_GetData(sm);

    if (!String_IsIndexValid(&data->textEntry.text, data->pointerText - 1))
        return;

    Cursor_MoveLeft(&data->cursor, data->windowText);

    data->pointerText--;

    wchar_t textChar = String_GetChar(&data->textEntry.text, data->pointerText);

    if (textChar == StackChar_Top(&data->inputBuffer))
        data->score.correctLetters--;

    StackChar_Pop(&data->inputBuffer);

    mvwaddnwstr(data->windowText, data->cursor.y, data->cursor.x, &textChar, 1);
}

static void handleCharacterInput(GameManager *sm, wint_t key)
{
    TypingData *data = (TypingData *)GameManager_GetData(sm);

    if (!String_IsIndexValid(&data->textEntry.text, data->pointerText + 1))
        return;

    wchar_t character = (wchar_t)key;
    StackChar_Push(&data->inputBuffer, character);

    wchar_t textChar = String_GetChar(&data->textEntry.text, data->pointerText);

    if (character == textChar)
    {
        data->score.correctLetters++;
        COLOR_ON(data->windowText, COLOR_GREEN);
    }
    else
    {
        data->score.wrongLetters++;
        COLOR_ON(data->windowText, COLOR_RED);
    }

    const wchar_t spaceChar = L'_';

    if (textChar == L' ')
    {
        textChar = spaceChar;
    }

    mvwaddnwstr(data->windowText, data->cursor.y, data->cursor.x, &textChar, 1);

    COLOR_CLEAR(data->windowText);

    // NOTE: Game win
    if (data->score.correctLetters >= (int)data->textEntry.text.length)
    {
        GameManager_Switch(sm, SCREEN_SCORE);
        return;
    }

    data->pointerText++;

    Cursor_MoveRight(&data->cursor, data->windowText);
}

// TODO: Add support for ENTER
static void compareInputText(GameManager *sm, wint_t key)
{
    switch (key)
    {
    case KEY_CODE_BACKSPACE:
        handleBackspace(sm);
        break;
    case KEY_CODE_ESCAPE:
        GameManager_Switch(sm, SCREEN_MENU);
        break;
    case KEY_CODE_ENTER:
        break;
    default:
        handleCharacterInput(sm, key);
        break;
    }
}

static void drawEntrySpeed(TypingData *data)
{
    WINDOW *win = data->windowStatus;

    int maxY = getmaxy(win);

    mvwprintw(win, maxY - 2, 2, "Speed: %.0f WPM", data->score.wordsPerMinute);
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

static void statusDraw(TypingData *data)
{
    WINDOW *win = data->windowStatus;

    werase(win);
    box(win, 0, 0);

    int maxX = getmaxx(win);

    // Title
    const char *title = "STATUS";
    mvwprintw(win, 1, (maxX - strlen(title)) / 2, "%s", title);

    mvwhline(win, 2, 1, ACS_HLINE, maxX - 2);

    int percentage = 100 * data->score.correctLetters / data->textEntry.text.length;

    drawPercentage(win, percentage);

    drawEntrySpeed(data);

    wrefresh(win);
}

static void calculateCharsPerSecond(TypingData *data)
{
    double deltaChars = (double)(data->score.correctLetters);
    double deltaTime = Clock_Get(&data->score.miliSeconds) / 1000.0;

    data->score.charsPerSecond = deltaChars / deltaTime;
    data->score.wordsPerMinute = data->score.charsPerSecond * (60.0 / 5.0);
}

void Typing_OnEnter(GameManager *sm)
{
    TypingData *data = (TypingData *)GameManager_GetData(sm);

    curs_set(TRUE);

    StackChar_Free(&data->inputBuffer);

    data->textEntry = TextEntry_RandomText();
    data->pointerText = 0;

    Cursor_Reset(&data->cursor);

    Clock_Set(&data->score.miliSeconds, SECONDS_FOR_CLOCK_UPDATE);
    data->score.charsPerSecond = 0.0;
    data->score.wordsPerMinute = 0.0;
    data->score.wrongLetters = 0;
    data->score.correctLetters = 0;

    Window_DrawString(data->windowText, &data->textEntry.text);

    data->shouldDraw = true;
}

void Typing_OnExit(GameManager *sm)
{
    TypingData *data = (TypingData *)GameManager_GetData(sm);

    data->shouldDraw = false;

    werase(data->windowText);
    wrefresh(data->windowText);

    werase(data->windowStatus);
    wrefresh(data->windowStatus);
}

void Typing_Input(GameManager *sm)
{
    TypingData *data = (TypingData *)GameManager_GetData(sm);

    wint_t key;
    int hasKeyPressed = wget_wch(data->windowText, &key);

    if (hasKeyPressed == ERR)
    {
        data->shouldDraw = false;
        return;
    }

    data->shouldDraw = true;

    compareInputText(sm, key);
}

void Typing_Update(GameManager *sm)
{
    TypingData *data = (TypingData *)GameManager_GetData(sm);

    if (Clock_Tick(&data->score.miliSeconds))
    {
        calculateCharsPerSecond(data);
        data->shouldDraw = true;
    }
}

void Typing_Draw(GameManager *sm)
{
    TypingData *data = (TypingData *)GameManager_GetData(sm);

    statusDraw(data);

    Cursor_Draw(&data->cursor, data->windowText);

    wrefresh(data->windowStatus);
    wrefresh(data->windowText);
}

void Typing_Free(GameManager *sm)
{
    TypingData *data = (TypingData *)GameManager_GetDataByType(sm, SCREEN_TYPING);

    StackChar_Free(&data->inputBuffer);

    delwin(data->windowText);
    delwin(data->windowStatus);
}

GameScreen Typing_Constructor(TypingData *data)
{
    GameScreen typing;

    typing.OnEnter = Typing_OnEnter;
    typing.OnExit = Typing_OnExit;
    typing.Input = Typing_Input;
    typing.Update = Typing_Update;
    typing.Draw = Typing_Draw;
    typing.Free = Typing_Free;

    data->windowText = Window_New(stdscr, WINDOW_LAYOUT_CENTER, WINDOW_ALIGN_NULL);

    nodelay(data->windowText, TRUE);
    notimeout(data->windowText, TRUE);

    data->windowStatus = Window_New(data->windowText, WINDOW_LAYOUT_ON_TOP, WINDOW_ALIGN_LEFT);

    StackChar_Init(&data->inputBuffer);

    typing.data = data;

    return typing;
}
