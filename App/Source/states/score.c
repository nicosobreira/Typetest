#include "states/score.h"

#include "Core/manager/game_manager.h"

#include "states/id.h"

void Score_OnEnter(GameManager* gm)
{
	(void)gm;
}

void Score_OnExit(GameManager* gm)
{
	ScoreData* data = (ScoreData *)GameManager_GetData(gm);

	werase(data->windowText);
	wrefresh(data->windowText);
}

void Score_Input(GameManager* gm)
{
	ScoreData* data = (ScoreData *)GameManager_GetData(gm);

	char key = (char)wgetch(data->windowText);

	switch (key)
	{
		case 'q':
			GameManager_Quit(gm);
			break;
		case 'p':
			GameManager_Switch(gm, SCREEN_TYPING);
			break;
	}
}

void Score_Update(GameManager* gm)
{
	(void)gm;
}

void Score_Draw(GameManager* gm)
{
	ScoreData* data = (ScoreData *)GameManager_GetData(gm);

	double totalTime = data->pTypingScore->miliSeconds.total / 1000.0;
	double wordsPerMinute = data->pTypingScore->wordsPerMinute;

	mvwprintw(data->windowText, 0, 0, "Total time: %.0f", totalTime);
	mvwprintw(data->windowText, 1, 0, "WPS: %.0f", wordsPerMinute);

	wrefresh(data->windowText);
}

void Score_Free(GameManager* gm)
{
	ScoreData* data = (ScoreData *)GameManager_GetDataByType(gm, SCREEN_SCORE);

	delwin(data->windowText);
}

GameScreen Score_Constructor(ScoreData* data, TypingScore* pTypingScore)
{
	GameScreen score;

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
