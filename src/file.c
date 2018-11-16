#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "console.h"
#include "file.h"
#include "strmod.h"

/** Add slash to end of directory path. */
char *addslash(char *p);

char *file_read(char *path) {
	FILE *file;
	int size, ok;
	char *buffer;

	/* Open file for reading. */
	file = fopen(path, "r");
	assert(file);

	/* Calculate size of file. */
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);

	/* Read file into buffer. */
	buffer = malloc(size + 1);
	ok = fread(buffer, sizeof(char), size, file);
	buffer[ok] = '\0';

	fclose(file);
	return buffer;
}

void file_write(char *path, char *buffer) {
	FILE *file;
	int ok;

	/* Open file for writing. */
	file = fopen(path, "w");
	assert(file);

	/* Read buffer into file. */
	fwrite(buffer, sizeof(char), strlen(buffer), file);
	ok = !ferror(file);
	assert(ok);

	fclose(file);
}

void file_append(char *path, char *buffer) {
	FILE *file;
	char *fbuffer;
	int ok;

	/* Open file for writing. */
	file = fopen(path, "a");
	assert(file);

	/* Append to file if not present. */
	fbuffer = file_read(path);
	if (!strstr(fbuffer, buffer)) {
		fprintf(file, "\n%s", buffer);
		ok = !ferror(file);
		assert(ok);
	}

	free(fbuffer);
	fclose(file);
}

void file_copy(char *path, char *newpath) {
	FILE *file;
	int size, ok;
	char *buffer;

	/* Open file for reading. */
	file = fopen(path, "rb");
	assert(file);

	/* Calculate size of file. */
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);

	/* Read file into buffer. */
	buffer = malloc(size);
	ok = fread(buffer, sizeof(char), size, file);
	fclose(file);

	/* Open file for output. */
	file = fopen(newpath, "wb");
	assert(file);

	/* Write buffer to new file. */
	fwrite(buffer, sizeof(char), size, file);
	ok = !ferror(file);
	assert(ok);
	fclose(file);
}

void file_print(char *path) {
	char *text = file_read(path);
	puts(text);
	free(text);
}

bool path_isfile(char *path) {
	FILE *file = fopen(path, "r");
	if (file) {
		fclose(file);
		return true;
	}

	return false;
}

bool path_isdir(char *path) {
	DIR *dir = opendir(path);
	if (dir) {
		closedir(dir);
		return true;
	}

	return false;
}

void dir_create(char *path) {
	if (!path_isdir(path)) {
		char command[128], *c;
		sprintf(command, "mkdir \"%s\"", path);

		#ifdef _WIN32
		for (c = command; *c; ++c)
			if (*c == '/') *c = '\\';
		#else
			(void)c;
		#endif

		system(command);
	}
}

void dir_copy(char *path, char *newpath) {
	char *p, *np, *fp, **sublist;
	int i = 0;

	/* Create top-level path if does not exist. */
	dir_create(newpath);

	/* Remove ../ from path. */
	for (p = path; *p == '.' && p[1] == '.'; p += 3);

	/* Copy path and newpath to allow modification. */
	p = strcln(p);
	p = addslash(p);
	np = strcln(newpath);
	np = addslash(np);

	/* Create top path inside newpath. */
	fp = concat(np, p);
	dir_create(fp);

	sublist = dirlist_alloc(path, true);
	while (sublist[i]) {
		if (path_isdir(sublist[i])) {
			dir_copy(sublist[i], np);
		} else {
			/* Copy file. */
			char *filename = concat(fp, nopath(sublist[i]));
			file_copy(sublist[i], filename);
			free(filename);
		}
		++i;
	}

	free(p); free(np); free(fp);
	dirlist_free(sublist);
}

char **dirlist_alloc(char *path, bool directories) {
	DIR *dir;
	struct dirent *file;
	char *p, **list;
	int id = 0, max = 1;

	/* Get list of all objects. */
	p = strcln(path);
	p = addslash(p);
	dir = opendir(p);
	assert(dir);

	/* Populate object list. */
	list = malloc(sizeof(char*));
	while ((file = readdir(dir))) {
		char *fp = concat(p, file->d_name);
		if ((!directories && path_isdir(fp)) || fp[strlen(fp) - 1] == '.' || strstr(fp, "Thumbs.db")) {
			/* Found a directory, ignore it. */
			free(fp);
		} else {
			/* Append to list. */
			list[id++] = fp;
			if (id == max) {
				/* Resize directory list. */
				list = realloc(list, sizeof(char*) * (max *= 2));
			}
		}
	}

	list[id] = '\0';
	closedir(dir);
	free(p);
	return list;
}

void dirlist_free(char **list) {
	int i = 0;
	while(list[i]) {
		free(list[i++]);
	}
	free(list);
}

char *addslash(char *p) {
	int length = strlen(p);

	/* Ensure path ends in slash. */
	if (p[length - 1] != '/') {
		p = realloc(p, length + 2);
		p[length] = '/';
		p[length + 1] = '\0';
	}

	return p;
}