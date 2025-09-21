#include "states/typing.h"

#include "ui/color.h"
#include "ui/window.h"
#include "assets/samples.h"

static void handleBackspace(GameStateMachine *sm)
{
	TypingData* data = (TypingData *)GameStateMachine_GetData(sm);

	if (!String_IsIndexValid(&data->pTextEntry->text, data->pointerText - 1))
		return;

	data->pointerText--;

	wchar_t textChar = String_GetChar(&data->pTextEntry->text, data->pointerText);

	if (textChar == StackChar_Top(&data->inputBuffer)) {
		data->correctLetters--;
	}

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
		wattron(data->windowText, COLOR_PAIR(COLOR_GREEN));
		data->correctLetters++;
	} else {
		wattron(data->windowText, COLOR_PAIR(COLOR_RED));
	}

	mvwaddnwstr(data->windowText, 0, data->pointerText, &character, 1);

	wattron(data->windowText, COLOR_PAIR(COLOR_DEFAULT));

	// NOTE: Game win
	if (data->correctLetters >= data->pTextEntry->text.length) {
		GameStateMachine_Quit(sm);
		return;
	}

	data->pointerText++;
}

// TODO: Add support for ENTER
static void compareInputText(GameStateMachine* sm, wint_t key)
{
	switch (key)
	{
		case 127: // Backspace (DEL)
			handleBackspace(sm);
			break;
		case 10: // Enter
			break;
		default:
			handleCharacterInput(sm, key);
			break;
	}
}

void Typing_OnEnter(GameStateMachine* sm)
{
	TypingData* data = (TypingData *)GameStateMachine_GetData(sm);

	// Cleanup previous screen
	erase();

	// data->pTextEntry = TextEntry_RandomText();
	data->pTextEntry = &TEXT_SAMPLES[0];
	data->pointerText = 0;
	data->correctLetters = 0;

	// mvwaddwstr(data->windowText, 1, 1, data->pTextEntry->text.letters);
	waddwstr(data->windowText, data->pTextEntry->text.letters);

	wrefresh(data->windowStatus);
	wrefresh(data->windowText);
}

void Typing_OnExit(GameStateMachine* sm)
{
	TypingData* data = (TypingData *)GameStateMachine_GetData(sm);

	StackChar_Free(&data->inputBuffer);

	delwin(data->windowText);
	delwin(data->windowStatus);
}

void Typing_Input(GameStateMachine* sm)
{
	TypingData* data = (TypingData *)GameStateMachine_GetData(sm);

	wint_t key;
	int returnValue = wget_wch(data->windowText, &key);

	if (returnValue == ERR)
		return;

	compareInputText(sm, key);

	// FIX: Move this elsewhere

	mvwprintw(data->windowStatus, 1, 1, "Correct Letters: %d", data->correctLetters);

	wmove(data->windowText, 0, data->pointerText);

	wrefresh(data->windowStatus);
	wrefresh(data->windowText);
}

void Typing_Update(GameStateMachine* sm)
{
	(void)sm;
}

void Typing_Draw(GameStateMachine* sm)
{
	(void)sm;
}

GameState Typing_Constructor(TypingData* data)
{
	GameState typing;

	typing.OnEnter = Typing_OnEnter;
	typing.OnExit = Typing_OnExit;
	typing.Input = Typing_Input;
	typing.Update = Typing_Update;
	typing.Draw = Typing_Update;

	data->windowText = Window_New(stdscr, WINDOW_LAYOUT_CENTER, WINDOW_ALIGN_NULL);

	nodelay(data->windowText, TRUE);
	notimeout(data->windowText, TRUE);

	data->windowStatus = Window_New(data->windowText, WINDOW_LAYOUT_ON_TOP, WINDOW_ALIGN_LEFT);

	StackChar_New(&data->inputBuffer);

	typing.data = data;

	return typing;
}
