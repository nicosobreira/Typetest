#ifndef WINDOW_H
#define WINDOW_H

#include <ncursesw/ncurses.h>

// TODO: Separate into a Layout enum and Aligment enum
// So a "on_top" layout can be mixed with a "center" aligment, or "left"
typedef enum WindowLayout
{
	WINDOW_LAYOUT_NULL, // NOTE: Used for skipping layout and just align
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

typedef struct WindowAttrs
{
	int x;
	int y;
	int cols;
	int lines;
} WindowAttrs;

WINDOW* Window_New(WINDOW *base, WindowLayout layout, WindowAlign align);

void Window_checkAttrs(WindowAttrs attrs);

WindowAttrs Window_SetLayout(WINDOW* base, WindowLayout layout, WindowAlign align);

WindowAttrs Window_SetLayout_Center(WINDOW *base);

WindowAttrs Window_SetLayout_OnTop(WINDOW *base, WindowAlign align);

float Window_SetAlign(WindowAlign align, float ref_size, float ref_pos, float obj_size);

float Window_SetAlign_Center(float ref_size, float ref_pos, float obj_size);

float Window_SetAlign_Left(float ref_size, float ref_pos, float obj_size);

float Window_SetAlign_Right(float ref_size, float ref_pos, float obj_size);

#endif // WINDOW_H
