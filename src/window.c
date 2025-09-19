#include "window.h"

#include "error.h"

WINDOW* Window_New(WINDOW *base, WindowLayout layout)
{
	WindowAttrs attrs = Window_SetLayout(base, layout);
	Window_checkAttrs(attrs);

	WINDOW* window = newwin(attrs.lines, attrs.cols, attrs.y, attrs.x);
	box(window, 0, 0);

	return window;
}

WindowAttrs Window_SetLayout(WINDOW* base, WindowLayout layout)
{
	switch (layout)
	{
		case WINDOW_LAYOUT_CENTER:
			return Window_SetLayout_Center(base);
		case WINDOW_LAYOUT_ON_TOP:
			return Window_SetLayout_On_Top(base);
		default:
			HANDLE_ERROR(1, "Invalid layout \"%zu\"", layout);
			break;
	}

	return (WindowAttrs){0};
}

WindowAttrs Window_SetLayout_Center(WINDOW *base)
{
	const float maxCols = (float)getmaxx(base);
	const float maxLines = (float)getmaxy(base);

	// FIX: This values are hard coded
	// Those values must be passed as arguments, in a struct called Attributes
	// or Positions
	const float colsFactor = 0.8f;
	const float linesFactor = 0.3f;

	float cols = maxCols * colsFactor;
	float lines = maxLines * linesFactor;

	float x = (maxCols - cols) / 2.0f;
	float y = (maxLines - lines) / 2.0f;

	WindowAttrs attrs = {
		.x = (int)x,
		.y = (int)y,
		.cols = (int)cols,
		.lines = (int)lines,
	};

	return attrs;
}

// FIX: The stdscr must be passed as an argument
WindowAttrs Window_SetLayout_On_Top(WINDOW *onTop)
{
	if (onTop == stdscr)
		HANDLE_ERROR(1, "%s", "No window can be on top of the stdscr");

	const float maxColsStdscr = (float)getmaxx(stdscr);
	const float maxLinesStdscr = (float)getmaxy(stdscr);

	const float beginCols = (float)getbegx(onTop);
	const float beginLines = (float)getbegy(onTop);

	// FIX: This values are hard coded
	// Those values must be passed as arguments, in a struct called Attributes
	// or Positions
	const float colsFactor = 0.4f;
	const float linesFactor = 0.3f;

	float cols = maxColsStdscr * colsFactor;
	float lines = maxLinesStdscr * linesFactor;

	// X Left Aligment
	float x = beginCols;

	const float offset = 3;
	float y = beginLines - lines - offset;

	WindowAttrs attrs = {
		.x = (int)x,
		.y = (int)y,
		.cols = (int)cols,
		.lines = (int)lines,
	};

	return attrs;
}

void Window_checkAttrs(WindowAttrs attrs)
{
	if (attrs.x < 0 || attrs.x > COLS)
		HANDLE_ERROR(1, "The x value is beyond the stdscr: %d", attrs.x);

	if (attrs.cols < 0 || attrs.cols > COLS)
		HANDLE_ERROR(1, "The cols value is beyond the stdscr: %d", attrs.cols);

	if (attrs.y < 0 || attrs.y > LINES)
		HANDLE_ERROR(1, "The y value is beyond the stdscr: %d", attrs.y);

	if (attrs.lines < 0 || attrs.lines > LINES)
		HANDLE_ERROR(1, "The lines value is beyond the stdscr: %d", attrs.lines);
}
