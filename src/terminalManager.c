#include <stdio.h>

#include <terminalManager.h>

//      https://man7.org/linux/man-pages/man5/terminal-colors.d.5.html

void setText(int nbArgs, ...) {
    va_list ap;
    va_start(ap, nbArgs);
    printf("\033[");
    while(nbArgs > 0) {
        int value;
        value = va_arg(ap, style);
        printf("%d", value);
        nbArgs--;
        if(nbArgs > 0)
            printf(";");
    }
    printf("m");
}

void setTextDefault() {
    printf("\033[0m");
}
