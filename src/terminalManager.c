#include <stdio.h>

#include <terminalManager.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef _WIN32
void cleanTerminal() {
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
			hStdOut,
			(TCHAR) ' ',
			cellCount,
			homeCoords,
			&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
			hStdOut,
			csbi.wAttributes,
			cellCount,
			homeCoords,
			&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition( hStdOut, homeCoords );
}

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

void printc(char* str, int nbArgs, ...) {
	va_list ap;
	va_start(ap, nbArgs);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int arg = 0;
	for(int i=0 ; i<nbArgs ; i++) {
		arg |= va_arg(ap, int);
	}
	SetConsoleTextAttribute(hConsole,
							arg);
	printf(str);
	setTextDefault();
}

#else

void cleanTerminal() {
	printf("\e[1;1H\e[2J");
	printf("\033[3J");
}
/**
 * @param nbArgs nombre d'arguments transmit (nbArgs exclu)
 * @param ... enum de type style définissant le style du text
 */
void setText(int nbArgs, ...) {
    int value;
    va_list ap;
    va_start(ap, nbArgs);
	printf("\033[");
	for (int i = 0; i < nbArgs; i++) {
		value = va_arg(ap, style);
		printf("%d", value);
		if (i < nbArgs - 1)
			printf(";");
	}
	printf("m");

}
void setTextDefault() {
	printf("\033[0m");
}
void printc(char* str, int nbArgs, ...) {
	int value;
	va_list ap;
	va_start(ap, nbArgs);
	printf("\033[");
	for (int i = 0; i < nbArgs; i++) {
		value = va_arg(ap, style);
		printf("%d", value);
		if (i < nbArgs - 1)
			printf(";");
	}
	printf("m");
	printf(str);
}

#endif