#pragma once

/** Get a yes/no prompt from the user. */
char console_yn(char* query);
/** Wait for a user keypress. */
void console_continue(char* text);
/** Print an error message and close. */
void console_quit(char *format, ...);