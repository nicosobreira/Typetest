#include "ui/cursor.h"

void Cursor_Reset(Point* pCursor, WINDOW* win)
{
	pCursor->x = 0;
	pCursor->y = 0;
}

void Cursor_MoveRight(Point* pCursor, WINDOW* win)
{
	const int maxCols = getmaxx(win);
	const int maxLines = getmaxy(win);

	if (pCursor->x + 1 > maxCols)
	{
		if (pCursor->y + 1 > maxLines)
			return;

		pCursor->x = 1;
		pCursor->y++;
		return;
	}

	pCursor->x++;
}

void Cursor_MoveLeft(Point* pCursor, WINDOW* win)
{
	const int maxCols = getmaxx(win);

	const int beginX = 0;
	const int beginY = 0;

	if (pCursor->x - 1 < beginX)
	{
		if (pCursor->y - 1 < beginY)
			return;

		pCursor->x = maxCols - 1;
		pCursor->y--;
		return;
	}

	pCursor->x--;
}

void Cursor_Draw(Point* pCursor, WINDOW* win)
{
	wmove(win, pCursor->y, pCursor->x);
}
