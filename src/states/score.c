#include "states/score.h"

void Score_OnEnter(GameStateMachine* sm)
{
	(void)sm;

	curs_set(0);
}

void Score_OnExit(GameStateMachine* sm)
{
	ScoreData* data = (ScoreData *)GameStateMachine_GetData(sm);

	werase(data->windowText);
	wrefresh(data->windowText);
}

void Score_Input(GameStateMachine* sm)
{
	ScoreData* data = (ScoreData *)GameStateMachine_GetData(sm);

	char key = (char)wgetch(data->windowText);

	switch (key)
	{
		case 'q':
			GameStateMachine_Quit(sm);
			break;
		case 'p':
			GameStateMachine_Switch(sm, GAME_STATE_TYPING);
			break;
	}
}

void Score_Update(GameStateMachine* sm)
{
	(void)sm;
}

void Score_Draw(GameStateMachine* sm)
{
	ScoreData* data = (ScoreData *)GameStateMachine_GetData(sm);

	double totalTime = data->pTypingScore->seconds.total / 1000.0;
	double wordsPerMinute = data->pTypingScore->wordsPerMinute;

	mvwprintw(data->windowText, 0, 0, "Total time: %.0f", totalTime);
	mvwprintw(data->windowText, 1, 0, "WPS: %.0f", wordsPerMinute);

	wrefresh(data->windowText);
}

void Score_Free(GameStateMachine* sm)
{
	ScoreData* data = (ScoreData *)GameStateMachine_GetDataByType(sm, GAME_STATE_SCORE);

	delwin(data->windowText);
}

GameState Score_Constructor(ScoreData* data, TypingScore* pTypingScore)
{
	GameState score;

	score.OnEnter = Score_OnEnter;
	score.OnExit = Score_OnExit;
	score.Input = Score_Input;
	score.Update = Score_Update;
	score.Draw = Score_Draw;
	score.Free = Score_Free;

	data->pTypingScore = pTypingScore;

	data->windowText = newwin(30, 30, 0, 0);

	nodelay(data->windowText, TRUE);
	notimeout(data->windowText, TRUE);

	score.data = data;
	return score;
}
