#pragma once

/** Return a copy of the string, equivalent to strdup(). */
char *strcln(char *s);
/** Return a copy of string containing n characters. */
char *strncln(char *s, int size);
/** Return a new string from two existing ones. */
char *concat(char *s1, char *s2);

/** Return pointer to filename of path. */
char *nopath(char *path);
/** Return a new string containing directory of a path. */
char *nofile(char *path);

/** Get the length of an integer as a string. */
int intlen(unsigned x);