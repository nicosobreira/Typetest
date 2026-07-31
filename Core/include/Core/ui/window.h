#ifndef WINDOW_H
#define WINDOW_H

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include <ncursesw/ncurses.h>

#include "Core/data_structures/string.h"

typedef enum WindowLayout
{
    WINDOW_LAYOUT_ON_TOP,
    WINDOW_LAYOUT_CENTER,
} WindowLayout;

typedef enum WindowAlign
{
    WINDOW_ALIGN_NULL,
    WINDOW_ALIGN_CENTER,
    WINDOW_ALIGN_RIGHT,
    WINDOW_ALIGN_LEFT,
} WindowAlign;

typedef struct WindowGeometry
{
    int x;
    int y;
    int cols;
    int lines;
} WindowGeometry;

int Window_GetCols(WINDOW *win);

int Window_GetLines(WINDOW *win);

WINDOW *Window_New(WINDOW *base, WindowLayout layout, WindowAlign align);

void Window_ClearRectangle(WINDOW *base, int startY, int startX, int endY, int endX);

/// Has wrap line support
void Window_DrawString(WINDOW *base, String *pString);

void Window_CheckAttrs(WindowGeometry geom);

WindowGeometry Window_SetLayout(WINDOW *base, WindowLayout layout, WindowAlign align);

WindowGeometry Window_SetLayout_Center(WINDOW *base);

WindowGeometry Window_SetLayout_OnTop(WINDOW *base, WindowAlign align);

float Window_SetAlign(WindowAlign align, float ref_size, float ref_pos, float obj_size);

float Window_SetAlign_Center(float ref_size, float ref_pos, float obj_size);

float Window_SetAlign_Left(float ref_size, float ref_pos, float obj_size);

float Window_SetAlign_Right(float ref_size, float ref_pos, float obj_size);

#endif // WINDOW_H
