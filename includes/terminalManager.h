#ifndef MALLOCWORLD_TERMINALMANAGER_H
#define MALLOCWORLD_TERMINALMANAGER_H

#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

//      https://man7.org/linux/man-pages/man5/terminal-colors.d.5.html

#ifdef _WIN32
#include <windows.h>

enum e_style {
    //FOREGROUND_BLUE = 1,
    //FOREGROUND_GREEN = 2,
    //FOREGROUND_RED = 4,
    //FOREGROUND_INTENSITY = 8
    FOREGROUND_BLACK = 0,
    FOREGROUND_WHITE = 7,
    FOREGROUND_YELLOW = 6,
    FOREGROUND_PURPLE = 5,
    FOREGROUND_CYAN = 3,

    //BACKGROUND_BLUE = 16,
    //BACKGROUND_GREEN = 32,
    //BACKGROUND_RED = 64,
    //BACKGROUND_INTENSITY = 128
    BACKGROUND_BLACK = 0,
    BACKGROUND_WHITE = 127,
    BACKGROUND_YELLOW = 96,
    BACKGROUND_PURPLE = 80,
    BACKGROUND_CYAN = 48
};


#else
typedef enum e_style {
    DEFAULT = 0,

    FOREGROUND_INTENSITY = 1,
    FG_UNDERLINED = 4,
    FG_FLASHING = 5,
    FOREGROUND_BLACK = 30,
    FOREGROUND_RED = 31,
    FOREGROUND_GREEN = 32,
    FOREGROUND_YELLOW = 33,
    FOREGROUND_BLUE = 34,
    FOREGROUND_PURPLE = 35,
    FOREGROUND_CYAN = 36,
    FOREGROUND_WHITE = 37,

	BACKGROUND_BLACK = 40,
	BACKGROUND_RED = 41,
	BACKGROUND_GREEN = 42,
	BACKGROUND_YELLOW = 43,
	BACKGROUND_BLUE = 44,
	BACKGROUND_PURPLE = 45,
	BACKGROUND_CYAN = 46,
	BACKGROUND_WHITE = 47
}style;

#endif

//---------------| TEXT (FOREGROUND & BACKGROUND) STYLE |---------------
void cleanTerminal();

void setText(int nbArgs, ...);
void setTextDefault();

void printc(char* str, int nbArgs, ...);



#endif //MALLOCWORLD_TERMINALMANAGER_H
