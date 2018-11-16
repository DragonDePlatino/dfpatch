#include <string.h>
#include <stdlib.h>
#include "strmod.h"

char *strcln(char *s) {
	return strcpy(malloc(strlen(s) + 1), s);
}

char *strncln(char *s, int size) {
	char *str = strncpy(malloc(size + 1), s, size);
	str[size] = '\0';
	return str;
}

char *concat(char *s1, char *s2) {
	char *result = malloc(strlen(s1) + strlen(s2) + 1);
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

char *nopath(char *path) {
	while(strchr(path, '/')) {
		path = strchr(path, '/') + 1;
	}

	while(strchr(path, '\\')) {
		path = strchr(path, '\\') + 1;
	}

	return path;
}

char *nofile(char *path) {
	char *last_slash, *dir;
	size_t length;
	
	if (strrchr(path, '/')) {
		last_slash = strrchr(path, '/');
	} else if (strrchr(path, '\\')) {
		last_slash = strrchr(path, '\\');
	} else {
		return (char*)path;
	}

	length = last_slash - path + 1;
	dir = malloc(length);
	dir = strncpy(dir, path, length);
	dir[length] = '\0';
	return dir;
}

/** Get the length of an integer as a string. */
int intlen(unsigned x) {
	if (x >= 1000000000) return 10;
	if (x >= 100000000)  return 9;
	if (x >= 10000000)   return 8;
	if (x >= 1000000)    return 7;
	if (x >= 100000)     return 6;
	if (x >= 10000)      return 5;
	if (x >= 1000)       return 4;
	if (x >= 100)        return 3;
	if (x >= 10)         return 2;
	return 1;
}