#include "scenes/score.h"

#include <stdlib.h>

#include "Core/scenes/manager.h"
#include "Core/ui/window.h"

#include "scenes/menu.h"
#include "scenes/typing.h"

void Score_OnEnter(void *data)
{
    ScoreData *pData = (ScoreData *)data;

    double totalTime = Clock_Get(&pData->pTypingScore->miliSeconds) / 1000.0;
    double wpm = pData->pTypingScore->wordsPerMinute;

    WINDOW *win = pData->windowText;

    curs_set(FALSE);
    box(win, 0, 0);

    // Title
    mvwprintw(win, 1, 2, "RESULTS");
    mvwhline(win, 2, 1, ACS_HLINE, Window_GetCols(win));

    // Game info
    mvwprintw(win, 4, 2, "Time:");
    mvwprintw(win, 4, 15, "%.2fs", totalTime);

    mvwprintw(win, 5, 2, "Speed:");
    mvwprintw(win, 5, 15, "%.0f WPM", wpm);

    // Input options
    mvwhline(win, 7, 1, ACS_HLINE, Window_GetCols(win));
    mvwprintw(win, 8, 2, "[R] Restart\t[M] Menu\t[Q] Quit");

    wrefresh(win);
}

void Score_OnExit(void *data)
{
    ScoreData *pData = (ScoreData *)data;

    werase(pData->windowText);
    wrefresh(pData->windowText);
}

void Score_Input(void *data)
{
    ScoreData *pData = (ScoreData *)data;

    char key = (char)wgetch(pData->windowText);

    switch (key)
    {
    case 'q':
        SceneManager_EndLoop();
        break;
    case 'r':
        SceneManager_Switch(SCENE_TYPING);
        break;
    case 'm':
        SceneManager_Switch(SCENE_MENU);
        break;
    }
}

void Score_Update(void *data)
{
    (void)data;
}

void Score_Draw(void *data)
{
    (void)data;
}

void Score_Free(void *data)
{
    ScoreData *pData = (ScoreData *)data;

    delwin(pData->windowText);

    free(data);
    data = NULL;
}

Scene Score_Scene(TypingScore *pTypingScore)
{
    ScoreData *pData = malloc(sizeof(ScoreData));
    Scene scene = {
        .pData = pData,
        .name = SCENE_SCORE,

        .OnEnter = Score_OnEnter,
        .OnExit = Score_OnExit,
        .Input = Score_Input,
        .Update = Score_Update,
        .Draw = Score_Draw,
        .Free = Score_Free,
    };

    pData->pTypingScore = pTypingScore;

    pData->windowText = Window_New(stdscr, WINDOW_LAYOUT_CENTER, WINDOW_ALIGN_CENTER);

    nodelay(pData->windowText, TRUE);
    notimeout(pData->windowText, TRUE);

    return scene;
}
