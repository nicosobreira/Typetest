#include "states/score.h"

#include "Core/manager/game_manager.h"
#include "Core/ui/window.h"

#include "states/id.h"

void Score_OnEnter(GameManager *gm)
{
    ScoreData *data = (ScoreData *)GameManager_GetData(gm);

    double totalTime = data->pTypingScore->miliSeconds.total / 1000.0;
    double wpm = data->pTypingScore->wordsPerMinute;

    WINDOW *win = data->windowText;

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
    mvwprintw(win, 8, 2, "[R] Restart\t[Q] Quit");

    wrefresh(win);
}

void Score_OnExit(GameManager *gm)
{
    ScoreData *data = (ScoreData *)GameManager_GetData(gm);

    werase(data->windowText);
    wrefresh(data->windowText);
}

void Score_Input(GameManager *gm)
{
    ScoreData *data = (ScoreData *)GameManager_GetData(gm);

    char key = (char)wgetch(data->windowText);

    switch (key)
    {
    case 'q':
        GameManager_Quit(gm);
        break;
    case 'r':
        GameManager_Switch(gm, SCREEN_TYPING);
        break;
    }
}

void Score_Update(GameManager *gm)
{
    (void)gm;
}

void Score_Draw(GameManager *gm)
{
    (void)gm;
}

void Score_Free(GameManager *gm)
{
    ScoreData *data = (ScoreData *)GameManager_GetDataByType(gm, SCREEN_SCORE);

    delwin(data->windowText);
}

GameScreen Score_Constructor(ScoreData *data, TypingScore *pTypingScore)
{
    GameScreen score;

    score.OnEnter = Score_OnEnter;
    score.OnExit = Score_OnExit;
    score.Input = Score_Input;
    score.Update = Score_Update;
    score.Draw = Score_Draw;
    score.Free = Score_Free;

    data->pTypingScore = pTypingScore;

    data->windowText = Window_New(stdscr, WINDOW_LAYOUT_CENTER, WINDOW_ALIGN_CENTER);

    nodelay(data->windowText, TRUE);
    notimeout(data->windowText, TRUE);

    score.data = data;
    return score;
}
