#include <stdarg.h>
#include <stdlib.h>
#include "console.h"
#include "stdio.h"

char console_yn(char* query) {
	char in = '\0';

	while (in != 'n') {
		printf("%s (y/n) ", query);
		scanf(" %c", &in);
		while ((getchar()) != '\n');

		switch (in) {
			case 'y':
			case 'n':
				return in;
			default:
				printf("Invalid response. ");
				break;
		}
	}
	return in;
}

void console_continue(char *text) {
	printf(text);
	while ((getchar()) != '\n');
}

void console_quit(char *format, ...) {
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	printf("\nPress enter to quit...");
	while ((getchar()) != '\n');
	exit(0);
}