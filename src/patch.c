#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "console.h"
#include "file.h"
#include "patch.h"
#include "strmod.h"

/** Comparator between two tag objects. */
int tagcmp(const void *t1, const void *t2) {
	return strcmp(((TAG*)t1)->name, ((TAG*)t2)->name);
}

/** Comparator between text and tag object. */
int entcmp(const void *s, const void *tag) {
	return strcmp(s, ((TAG*)tag)->name);
}

/* TODO: Consider doing this with strcmp instead of strncmp. */
/** Comparator between tag text and tag object. */
int rawcmp(const void *s, const void *tag) {
	char * n = ((TAG*)tag)->name;
	int len1, len2;
	int result;

	len1 = strcspn(s, ":]");
	len2 = strchr(n, '|') - (char*)n;
	if (len2 < 0) {
		len2 = strcspn(n, ":]");
		return strncmp(s, n, len1 > len2 ? len1 : len2);
	} else if ((result = strncmp(s, n, len1 > len2 ? len1 : len2))) {
		return result;
	} else {
		n[len2] = ':';
		return 0;
	}
}

/** Print the given string, cutting arguments off at newlines. */
void warn(char* c, ...) {
	va_list args;
	va_start(args, c);

	printf("    ");
	while (*c) {
		if (*c == '%') {
			char *str = va_arg(args, char*);
			printf("%.*s", (int)strcspn(str, "]\n"), str);
		} else {
			putchar(*c);
		}
		++c;
	}
	console_continue("");
}

TAGLIST *patch_read_raw(char* path, TAGLIST *deflist) {
	char *text = file_read(path), *name;
	char *start = text, *end = text;
	TAGLIST *list = taglist_alloc();
	TAG *last;

	while ((start = strchr(start, '['))) {
		if (!list->size || *(start - 1) == '\n') {
			/* Calculate length of entity. */
			end = strchr(++start, ']');
			if (!end) {
				end = start + strlen(start);
			}

			/* Append to list. */
			name = strncln(start, (int)(end - start));
			taglist_append(list, name, 0);
			if (!strchr(start, ']')) {
				warn("[%] has no closing bracket!!", start);
			}
			if (strchr(name, '\n')) {
				warn("[%] contains line breaks!!", name);
			}
		} else if (list->size) {
			/* Calculate length of entity tag. */
			end = strchr(++start, ']');
			if (!end) {
				end = start + strlen(start);
				warn("[%]:[%] has no closing bracket!!", list->data[list->size - 1].name, start);
			}

			/* Append to list. */
			name = strncln(start, (int)(end - start));
			name = taglist_replace(name, deflist);
			if ((end = strchr(name, '\n'))) {
				warn("[%]:[%] contains line breaks!!", list->data[list->size - 1].name, name);
			}

			/* Add a new mod tag. */
			last = list->data + list->size - 1;
			while (last->mod) {
				last = last->mod;
			}
			last->mod = calloc(1, sizeof(TAG));
			last->mod->name = name;
		}
	}

	free(text);
	qsort(list->data, list->size, sizeof(TAG), tagcmp);
	return list;
}

TAGLIST *patch_read_define(char *path) {
	char *text = file_read(path);
	char *c = text;
	TAGLIST *list = taglist_alloc();

	while ((c = strchr(c, '#'))) {
		/* Read define name. */
		int size;
		++c;
		size = strchr(c, ' ') - c;
		taglist_append(list, strncln(c, size), atoi(c + size + 1));
		c += size + 1;
	}

	free(text);
	qsort(list->data, list->size, sizeof(TAG), tagcmp);
	return list;
}

void patch_write_raw(char *path, TAGLIST *patch) {
	FILE *file;
	char *text = file_read(path);
	char *wstart = text, *c = text;
	TAG *tag, *subtag, *entity = NULL;
	
	/* Open output file for writing. */
	file = fopen(path, "w");

	while ((c = strchr(c, '['))) {
		char* end = strchr(++c, ']');
		*end = '\0';
		if ((tag = bsearch(c, patch->data, patch->size, sizeof(TAG), entcmp))) {
			/* Found a new entity. */
			if (entity && entity->mod) {
				/* Rewind to last entity and write file so far. */
				while(*(--c) != ']');
				fwrite(wstart, 1, c - wstart + 1, file);
				wstart = ++c;

				/* Append new tags. */
				subtag = entity->mod;
				while (subtag) {
					fprintf(file, "\n\t[%s]", subtag->name);
					subtag = subtag->mod;
				}
			}
			entity = tag;
		} else if (entity) {
			/* Search linked list for applicable modification. */
			tag = entity;
			while (tag->mod && rawcmp(c, tag->mod)) {
				tag = tag->mod;
			}

			if (tag->mod) {
				/* Write file so far and tag. */
				fprintf(file, "%.*s[%s]", (int)(c - wstart - 1), wstart, tag->mod->name);
				wstart = end + 1;

				/* Remove tag from linked list. */
				subtag = tag->mod->mod;
				free(tag->mod->name);
				free(tag->mod);
				tag->mod = subtag;
			}
		}
		*end = ']';
		c = end + 1;
	}

	/* Write rest of file and append remaining tags. */
	fwrite(wstart, 1, strlen(wstart), file);
	subtag = entity->mod;
	while (subtag) {
		fprintf(file, "\n\t[%s]", subtag->name);
		subtag = subtag->mod;
	}

	fclose(file);
	free(text);
}

void patch_write_init(char *path, TAGLIST *patch) {
	FILE *file;
	char *text = file_read(path);
	char *wstart = text, *c = text;
	
	/* Open output file for writing. */
	file = fopen(path, "w");
	while ((c = strchr(c, '['))) {
		TAG *tag;
		if ((tag = bsearch(++c, patch->data, patch->size, sizeof(TAG), rawcmp))) {
			/* Write file so far and tag. */
			fprintf(file, "%.*s[%s]", (int)(c - 1 - wstart), wstart, tag->name);
			wstart = strchr(c, ']') + 1;
		}
	}

	fwrite(wstart, 1, strlen(wstart), file);
	fclose(file);
	free(text);
}