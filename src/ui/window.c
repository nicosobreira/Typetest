#include "ui/window.h"

#include <stdlib.h>
#include <string.h>

#include "utils/error.h"
#include "utils/my_math.h"
#include "ui/color.h"

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

	return COLOR_GREEN;
}

WINDOW* Window_New(WINDOW *base, WindowLayout layout, WindowAlign align)
{
	WindowAttrs attrs = Window_SetLayout(base, layout, align);
	Window_CheckAttrs(attrs);

	WINDOW* window = newwin(attrs.lines, attrs.cols, attrs.y, attrs.x);

	return window;
}

void Window_DrawString(WINDOW* base, String* pString)
{
	const int startX = 0;
	const int startY = 0;
	const int maxCols = getmaxx(base) - startX * 2;
	const int maxLines = getmaxy(base) - startY * 2;

	int totalLength = pString->length;
	int currentLine = startY;

	for (int i = 0; i < totalLength; i += maxCols)
	{
		const int sliceLength = returnMinInt(totalLength - i, maxCols);

		if (currentLine > maxLines)
			break;

		mvwaddnwstr(base, currentLine, startX, &pString->letters[i], sliceLength);

		currentLine++;
	}

	wrefresh(base);
}

void Window_ClearRectangle(WINDOW* base, int startY, int startX, int endY, int endX)
{
	if (startY > endY || startX > endX)
		HANDLE_ERROR(1, "%s", "Wrong argument order to function");

	int width = endX - startX + 1;
	char *blanckString = malloc(width + 1);
	memset(blanckString, ' ', width);
	blanckString[width] = '\0';

	for (int y = startY; y <= endY; y++) {
		mvwaddstr(base, y, startX, blanckString);
	}

	free(blanckString);
}

void Window_DrawPercentage(WINDOW* base, int percentage)
{
	const int color = getPercentageColor(percentage);

	const int startX = 0;
	const int startY = 0;

	const int maxCols = getmaxx(base) - startX * 2;

	const int perX = startX;
	const int perY = startY + 1;

	Window_ClearRectangle(base, perY, perX, perY, maxCols);

	const char perChar = '=';
	const char perArrow = '>';
	const char perBorderLeft = '[';
	const char perBorderRight = ']';

	int barSize = (percentage * maxCols / 100) - 2;
	int index = 0;

	mvwaddch(base, perY, perX, perBorderLeft);

	COLOR_ON(base, color);

	while (index < barSize)
	{
		mvwaddch(base, perY, perX + index + 1, perChar);
		index++;
	}

	mvwaddch(base, perY, perX + index + 1, perArrow);

	COLOR_CLEAR(base);

	mvwaddch(base, perY, maxCols, perBorderRight);
}

WindowAttrs Window_SetLayout(WINDOW* base, WindowLayout layout, WindowAlign align)
{
	switch (layout)
	{
		case WINDOW_LAYOUT_ON_TOP:
			return Window_SetLayout_OnTop(base, align);
		case WINDOW_LAYOUT_CENTER:
			return Window_SetLayout_Center(base);
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

	WindowAttrs attrs =
		{
			.x = (int)x,
			.y = (int)y,
			.cols = (int)cols,
			.lines = (int)lines,
		};

	return attrs;
}

// FIX: The stdscr must be passed as an argument
WindowAttrs Window_SetLayout_OnTop(WINDOW *onTop, WindowAlign align)
{
	if (onTop == stdscr)
		HANDLE_ERROR(1, "%s", "No window can be on top of the stdscr");

	const float maxColsStdscr = (float)getmaxx(stdscr);
	const float maxLinesStdscr = (float)getmaxy(stdscr);

	const float beginCols = (float)getbegx(onTop);
	const float beginLines = (float)getbegy(onTop);

	const float maxCols = (float)getmaxx(onTop);

	// FIX: This values are hard coded
	// Those values must be passed as arguments, in a struct called Attributes
	// or Positions
	const float colsFactor = 0.4f;
	const float linesFactor = 0.2f;

	float cols = maxColsStdscr * colsFactor;
	float lines = maxLinesStdscr * linesFactor;

	// X Aligment
	float x = Window_SetAlign(align, (maxCols - beginCols), beginCols, cols);

	const float offset = .0f;
	float y = beginLines - lines - offset;

	WindowAttrs attrs =
		{
			.x = (int)x,
			.y = (int)y,
			.cols = (int)cols,
			.lines = (int)lines,
		};

	return attrs;
}

float Window_SetAlign(WindowAlign align, float ref_size, float ref_pos, float obj_size)
{
	switch (align)
	{
		case WINDOW_ALIGN_CENTER:
			return Window_SetAlign_Center(ref_size, ref_pos, obj_size);
		case WINDOW_ALIGN_LEFT:
			return Window_SetAlign_Left(ref_size, ref_pos, obj_size);
		case WINDOW_ALIGN_RIGHT:
			return Window_SetAlign_Right(ref_size, ref_pos, obj_size);
		default:
			HANDLE_ERROR(1, "%s", "Invalid window aligment");
	}

	return 0;
}

float Window_SetAlign_Center(float ref_size, float ref_pos, float obj_size)
{
	return ref_pos + (obj_size - ref_size) / 2.0f;
}

float Window_SetAlign_Left(float ref_size, float ref_pos, float obj_size)
{
	return ref_pos;
}

// FIX: don't work
float Window_SetAlign_Right(float ref_size, float ref_pos, float obj_size)
{
	return obj_size;
}

void Window_CheckAttrs(WindowAttrs attrs)
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
