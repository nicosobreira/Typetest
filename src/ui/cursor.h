#ifndef CURSOR_H
#define CURSOR_H

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <ncursesw/ncurses.h>

#include "utils/point.h"

void Cursor_Reset(Point* pCursor);

void Cursor_MoveRight(Point* pCursor, WINDOW* win);

void Cursor_MoveLeft(Point* pCursor, WINDOW* win);

void Cursor_Draw(Point* pCursor, WINDOW* win);

#endif // CURSOR_H
