#include "scenes/menu.h"

#include <ncursesw/ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "Core/constants.h"
#include "Core/scenes.h"
#include "Core/ui/window.h"

#include "scenes/typing.h"

void Menu_OnEnter(void *data)
{
    MenuData *self = (MenuData *)data;
    WINDOW *win = self->windowText;

    curs_set(FALSE);
    werase(win);
    box(win, 0, 0);

    int width = getmaxx(win);

    char *title = "Typetest";
    int titleX = (width - strlen(title)) / 2;
    mvwprintw(win, 1, titleX, "%s", title);

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

void Menu_OnExit(void *data)
{
    MenuData *self = (MenuData *)data;

    werase(self->windowText);
    wrefresh(self->windowText);
}

void Menu_Input(void *data)
{
    MenuData *self = (MenuData *)data;

    char key = (char)wgetch(self->windowText);

    switch (key)
    {
    case 'p':
        SceneManager_Switch(SCENE_TYPING);
        break;
    case KEY_CODE_BACKSPACE:
    case 'q':
        SceneManager_Close();
        break;
    }
}

void Menu_Update(void *data)
{
    (void)data;
}

void Menu_Draw(void *data)
{
    (void)data;
}

void Menu_Free(void *data)
{
    MenuData *self = (MenuData *)data;

    delwin(self->windowText);

    free(data);
    data = NULL;
}

Scene Menu_Scene(void)
{
    MenuData *self = malloc(sizeof(MenuData));

    Scene scene = {
        .name = SCENE_MENU,
        .pData = self,
        .OnEnter = Menu_OnEnter,
        .OnExit = Menu_OnExit,
        .Input = Menu_Input,
        .Update = Menu_Update,
        .Draw = Menu_Draw,
        .Free = Menu_Free,
    };

    self->windowText = Window_New(stdscr, WINDOW_LAYOUT_CENTER, WINDOW_ALIGN_NULL);

    nodelay(self->windowText, TRUE);
    notimeout(self->windowText, TRUE);

    return scene;
}
