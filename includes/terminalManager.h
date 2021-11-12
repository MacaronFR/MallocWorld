#ifndef MALLOCWORLD_TERMINALMANAGER_H
#define MALLOCWORLD_TERMINALMANAGER_H

#include <stdarg.h>
#include <stdio.h>

//      https://man7.org/linux/man-pages/man5/terminal-colors.d.5.html

typedef enum e_style {
    DEFAULT = 0,

    FG_BRIGHTER = 1,
    FG_UNDERLINED = 4,
    FG_FLASHING = 5,
    FG_BLACK = 30,
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELlOW = 33,
    FG_BLUE = 34,
    FG_PURPLE = 35,
    FG_CYAN = 36,
    FG_WHITE = 37,

    BG_BLACK = 40,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_YELLOW = 43,
    BG_BLUE = 44,
    BG_PURPLE = 45,
    BG_CYAN = 46,
    BG_WHITE = 47
}style;

//---------------| TEXT (FOREGROUND & BACKGROUND) STYLE |---------------
void setText(int nbArgs, ...);
void setTextDefault();



#endif //MALLOCWORLD_TERMINALMANAGER_H
