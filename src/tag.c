#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strmod.h"
#include "tag.h"

#define INT_STRLEN 11

TAGLIST *taglist_alloc(void) {
	TAGLIST *list = malloc(sizeof(TAGLIST));
	list->size = 0;
	list->max = 1;
	list->data = malloc(sizeof(TAG) * list->max);

	return list;
}

void taglist_free(TAGLIST *list) {
	int i;
	for (i = 0; i < list->size; ++i) {
		TAG *child = list->data[i].mod;
		while (child) {
			/* Free linked list of modifications. */
			TAG *next = child->mod;
			free(child->name);
			free(child);
			child = next;
		}
	}

	free(list->data);
	free(list);
}

void taglist_append(TAGLIST *list, char *name, int data) {
	memset(list->data + list->size, 0, sizeof(TAG));
	list->data[list->size].name = name;
	list->data[list->size].data = data;

	if (++list->size == list->max) {
		/* Resize define list. */
		list->data = realloc(list->data, sizeof(TAG) * (list->max *= 2));
	}
}

char *taglist_replace(char *tag, TAGLIST *list) {
	int i;
	for (i = 0; list && i < list->size; ++i) {
		char *substr;
		if ((substr = strstr(tag, list->data[i].name))) {
			/* Replace substring with list entry. */
			int prelen, midlen, postlen;
			char *newtag;

			/* Get length before and after replacement. */
			prelen = substr - tag;
			midlen = intlen(list->data[i].data);
			postlen = strlen(tag) - prelen - midlen;

			/* Create new string from characters before, in and after list entry. */
			newtag = malloc(prelen + INT_STRLEN + postlen);
			sprintf(newtag, "%.*s%d%s", prelen, tag, list->data[i].data, tag + prelen + strlen(list->data[i].name));
			free(tag);
			tag = newtag;
			--i;
		}
	}
	return tag;
}