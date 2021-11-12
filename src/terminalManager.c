#include <stdio.h>

#include <terminalManager.h>

#ifdef _WIN32

void setText(int nbArgs, ...) {
    va_list ap;
    va_start(ap, nbArgs);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int arg = 0;
    for(int i=0 ; i<nbArgs ; i++) {
        arg |= va_arg(ap, int);
    }
    SetConsoleTextAttribute(hConsole,
                            arg);
}

void setTextDefault() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,7);
}

#else

/**
 *
 * @param nbArgs nombre d'arguments transmit (nbArgs exclu)
 * @param ... enum de type style définissant le style du text
 */
void setText(int nbArgs, ...) {
    int value;
    va_list ap;
    va_start(ap, nbArgs);
    printf("\033[");
    for(int i=0 ; i<nbArgs ; i++) {
        value = va_arg(ap, style);
        printf("%d", value);
        if(i<nbArgs-1)
            printf(";");
    }
    printf("m");
}

void setTextDefault() {
    printf("\033[0m");
}
#endif