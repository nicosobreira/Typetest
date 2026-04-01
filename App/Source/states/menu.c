#include "states/menu.h"

#include <ncursesw/ncurses.h>
#include <string.h>

#include "Core/constants/key_codes.h"
#include "Core/manager/game_manager.h"
#include "Core/ui/window.h"

#include "states/id.h"

void Menu_OnEnter(GameManager *sm)
{
    MenuData *data = (MenuData *)GameManager_GetData(sm);
    WINDOW *win = data->windowText;

    curs_set(FALSE);
    werase(win);
    box(win, 0, 0);

    int width = getmaxx(win);

    // Title
    int titleX = (width - strlen(data->title)) / 2;
    mvwprintw(win, 2, titleX, "%s", data->title);

    mvwhline(win, 3, 1, ACS_HLINE, width - 2);

    // Options
    const char *playText = "[P] Play";
    const char *quitText = "[Q] Exit";

    int playX = (width - strlen(playText)) / 2;
    int quitX = (width - strlen(quitText)) / 2;

    mvwprintw(win, 5, playX, "%s", playText);
    mvwprintw(win, 6, quitX, "%s", quitText);

    mvwhline(win, 8, 1, ACS_HLINE, width - 2);
    mvwprintw(win, 9, 2, "Select an option");
    mvwprintw(win, 10, 2, "Obs.: Press [ESC] in game to comeback here");

    wrefresh(win);
}

void Menu_OnExit(GameManager *sm)
{
    MenuData *data = (MenuData *)GameManager_GetData(sm);

    werase(data->windowText);
    wrefresh(data->windowText);
}

void Menu_Input(GameManager *sm)
{
    MenuData *data = (MenuData *)GameManager_GetData(sm);

    char key = (char)wgetch(data->windowText);

    switch (key)
    {
    case 'p':
        GameManager_Switch(sm, SCREEN_TYPING);
        break;
    case KEY_CODE_BACKSPACE:
    case 'q':
        GameManager_Quit(sm);
        break;
    }
}

void Menu_Update(GameManager *sm)
{
    (void)sm;
}

void Menu_Draw(GameManager *sm)
{
    (void)sm;
}

void Menu_Free(GameManager *sm)
{
    MenuData *data = (MenuData *)GameManager_GetDataByType(sm, SCREEN_MENU);

    delwin(data->windowText);
}

GameScreen Menu_Constructor(MenuData *data)
{
    GameScreen menu;

    menu.OnEnter = Menu_OnEnter;
    menu.OnExit = Menu_OnExit;
    menu.Input = Menu_Input;
    menu.Update = Menu_Update;
    menu.Draw = Menu_Draw;
    menu.Free = Menu_Free;

    data->windowText = Window_New(stdscr, WINDOW_LAYOUT_CENTER, WINDOW_ALIGN_NULL);

    nodelay(data->windowText, TRUE);
    notimeout(data->windowText, TRUE);

    data->title = "Typetest";

    menu.data = data;

    return menu;
}
