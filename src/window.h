#ifndef WINDOW_H
#define WINDOW_H

#include <ncursesw/ncurses.h>

// TODO: Separate into a Layout enum and Aligment enum
// So a "on_top" layout can be mixed with a "center" aligment, or "left"
typedef enum WindowLayout
{
	WINDOW_LAYOUT_CENTER,
	WINDOW_LAYOUT_ON_TOP,
} WindowLayout;

typedef struct WindowAttrs
{
	int x;
	int y;
	int cols;
	int lines;
} WindowAttrs;

WINDOW* Window_New(WINDOW *base, WindowLayout layout);

void Window_checkAttrs(WindowAttrs attrs);

WindowAttrs Window_SetLayout(WINDOW* base, WindowLayout layout);

WindowAttrs Window_SetLayout_Center(WINDOW *base);

WindowAttrs Window_SetLayout_On_Top(WINDOW *base);


#endif // WINDOW_H
