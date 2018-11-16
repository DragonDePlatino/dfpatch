#pragma once

/** A list of patch or variable tags. */
typedef struct TAG TAG;
struct TAG {
	char *name;
	TAG *mod;
	int data;
};

typedef struct {
	TAG *data;
	int size, max;
} TAGLIST;

/** Allocate a new define list. */
TAGLIST *taglist_alloc(void);
/** Free a list of defines and all children. */
void taglist_free(TAGLIST *list);
/** Append a new entry to the end of a taglist. */
void taglist_append(TAGLIST *list, char *name, int data);
/** Substitute list entries into tag if applicable. */
char *taglist_replace(char *tag, TAGLIST *list);