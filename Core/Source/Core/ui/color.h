#ifndef COLOR_H
#define COLOR_H


#include <ncursesw/ncurses.h>

#define COLOR_DEFAULT (-1)

#define COLOR_ON(win, color) (wattron(win, COLOR_PAIR(color)))

#define COLOR_CLEAR(win) (wattroff(win, COLOR_PAIR(COLOR_DEFAULT)))

#define BACKGROUND_ON(win, color) (wbkgd(win, COLOR_PAIR(color)))

#define BACKGROUND_CLEAR(win) (wbkgd(win, COLOR_PAIR(COLOR_DEFAULT)))

#endif // COLOR_H
