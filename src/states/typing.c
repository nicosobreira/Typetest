#include "states/typing.h"

#include "constants/key_codes.h"
#include "ui/color.h"
#include "ui/window.h"

static const double SECONDS_CLOCK_UPDATE = 500.0;

static void handleBackspace(GameStateMachine* sm)
{
	TypingData* data = (TypingData *)GameStateMachine_GetData(sm);

	if (!String_IsIndexValid(&data->pTextEntry->text, data->pointerText - 1))
		return;

	data->pointerText--;

	wchar_t textChar = String_GetChar(&data->pTextEntry->text, data->pointerText);

	if (textChar == StackChar_Top(&data->inputBuffer))
		data->correctLetters--;

	StackChar_Pop(&data->inputBuffer);

	mvwaddnwstr(data->windowText, 0, data->pointerText, &textChar, 1);
}

static void handleCharacterInput(GameStateMachine* sm, wint_t key)
{
	TypingData* data = (TypingData *)GameStateMachine_GetData(sm);

	if (!String_IsIndexValid(&data->pTextEntry->text, data->pointerText + 1))
		return;

	wchar_t character = (wchar_t)key;
	StackChar_Push(&data->inputBuffer, character);

	wchar_t textChar = String_GetChar(&data->pTextEntry->text, data->pointerText);

	if (character == textChar) {
		data->correctLetters++;

		COLOR_ON(data->windowText, COLOR_GREEN);
	} else {
		data->score.wrongLetters++;

		COLOR_ON(data->windowText, COLOR_RED);
	}

	// TODO: if the key is space then set the BACKGROUND color to RED

	mvwaddnwstr(data->windowText, 0, data->pointerText, &character, 1);

	COLOR_CLEAN(data->windowText);

	// NOTE: Game win
	if (data->correctLetters >= data->pTextEntry->text.length)
	{
		GameStateMachine_Switch(sm, GAME_STATE_MENU);
		return;
	}

	data->pointerText++;
}

// TODO: Add support for ENTER
static void compareInputText(GameStateMachine* sm, wint_t key)
{
	switch (key)
	{
		case KEY_CODE_BACKSPACE:
			handleBackspace(sm);
			break;
		case KEY_CODE_ESCAPE:
			GameStateMachine_Switch(sm, GAME_STATE_MENU);
			break;
		case KEY_CODE_ENTER:
			break;
		default:
			handleCharacterInput(sm, key);
			break;
	}
}

static void drawEntrySpeed(TypingData* data)
{
	const int startX = 1;
	const int startY = 1;

	const int maxLines = getmaxy(data->windowStatus) - startY * 2;

	Window_ClearRectangle(data->windowStatus, maxLines - 1, startX, maxLines, startX);

	mvwprintw(data->windowStatus, maxLines, startX, "CPS: %.0f", data->score.charsPerSecond);
	mvwprintw(data->windowStatus, maxLines - 1, startX, "WPM: %.0f", data->score.wordsPerMinute);
}

static void drawStatus(TypingData* data)
{
	if (!data->shouldDraw)
		return;

	const int startX = 1;
	const int startY = 1;

	drawEntrySpeed(data);

	mvwprintw(data->windowStatus, startX, startY, "Completion:");

	int percentage = 100 * data->correctLetters/data->pTextEntry->text.length;

	Window_DrawPercentage(data->windowStatus, percentage);
}

static void calculateCharsPerSecond(TypingData* data)
{
	// Question: How much did the inputBuffer grow in one second?
	double deltaChars = (double)(data->inputBuffer.size);
	double deltaTime = Clock_Get(&data->score.seconds) / 1000.0;

	data->score.charsPerSecond = deltaChars / deltaTime;
	data->score.wordsPerMinute = data->score.charsPerSecond * (60.0 / 5.0);
}

void Typing_OnEnter(GameStateMachine* sm)
{
	TypingData* data = (TypingData *)GameStateMachine_GetData(sm);

	curs_set(1);

	// TODO: Implement a countdown
	StackChar_Free(&data->inputBuffer);

	data->pTextEntry = TextEntry_RandomText();
	data->pointerText = 0;
	data->correctLetters = 0;

	Clock_Set(&data->score.seconds, SECONDS_CLOCK_UPDATE);
	data->score.charsPerSecond = 0.0;
	data->score.wordsPerMinute = 0.0;
	data->score.wrongLetters = 0;

	Window_DrawString(data->windowText, &data->pTextEntry->text);

	box(data->windowStatus, 0, 0);

	data->shouldDraw = true;

	wrefresh(data->windowStatus);
	wrefresh(data->windowText);
}

void Typing_OnExit(GameStateMachine* sm)
{
	TypingData* data = (TypingData *)GameStateMachine_GetData(sm);

	data->shouldDraw = false;

	werase(data->windowText);
	wrefresh(data->windowText);

	werase(data->windowStatus);
	wrefresh(data->windowStatus);
}

void Typing_Input(GameStateMachine* sm)
{
	TypingData* data = (TypingData *)GameStateMachine_GetData(sm);

	wint_t key;
	int returnValue = wget_wch(data->windowText, &key);

	if (returnValue == ERR)
	{
		data->shouldDraw = false;
		return;
	}

	data->shouldDraw = true;
	compareInputText(sm, key);
}

void Typing_Update(GameStateMachine* sm, double delta)
{
	TypingData* data = (TypingData *)GameStateMachine_GetData(sm);

	if (Clock_Tick(&data->score.seconds, delta))
	{
		calculateCharsPerSecond(data);
		data->shouldDraw = true;
	}
}

void Typing_Draw(GameStateMachine* sm)
{
	TypingData* data = (TypingData *)GameStateMachine_GetData(sm);

	drawStatus(data);

	wmove(data->windowText, 0, data->pointerText);

	wrefresh(data->windowStatus);
	wrefresh(data->windowText);
}

void Typing_Free(GameStateMachine* sm)
{
	TypingData* data = (TypingData *)GameStateMachine_GetData(sm);

	StackChar_Free(&data->inputBuffer);

	delwin(data->windowText);
	delwin(data->windowStatus);
}

GameState Typing_Constructor(TypingData* data)
{
	GameState typing;

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

	StackChar_New(&data->inputBuffer);

	typing.data = data;

	return typing;
}
