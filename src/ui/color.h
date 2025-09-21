#ifndef CORE_H
#define CORE_H

#include <ncursesw/ncurses.h>

#define COLOR_DEFAULT (-1)

#define COLOR_ON(win, color) (wattron(win, COLOR_PAIR(color)))

#define COLOR_OFF(win, color) (wattroff(win, COLOR_PAIR(color)))

#endif // CORE_H
