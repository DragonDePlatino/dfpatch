#pragma once

/***********/
/*	Files  */
/***********/

/** Open a text file and return allocated string. */
char *file_read(char *path);
/** Close file after modifying it. */
void file_write(char *path, char *buffer);
/** Append text to file if not found in it. */
void file_append(char *path, char *buffer);
/** Copy a file to a new directory. */
void file_copy(char *path, char *newpath);
/** Open and print the contents of a file. */
void file_print(char *path);

/*********************/
/*	Path management  */
/*********************/

/** Check if a path is a file. */
bool path_isfile(char *path);
/** Check if a path is a directory. */
bool path_isdir(char *path);

/*****************/
/*	Directories  */
/*****************/

/** Create a directory. */
void dir_create(char *path);
/** Copy a directory and all subfiles to new path. */
void dir_copy(char *path, char *newpath);
/** Get a list of all files in a directory. */
char **dirlist_alloc(char *path, bool directories);
/** Free a file list after creating one. */
void dirlist_free(char **list);