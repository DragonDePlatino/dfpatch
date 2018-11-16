#pragma once
#include "tag.h"

typedef void (*patch_cb)(char*, TAGLIST*);

/** Read a patch file into a list of patches. */
TAGLIST *patch_read_raw(char *path, TAGLIST *deflist);
/** Read global defines from file. */
TAGLIST *patch_read_define(char *path);

/** Patch a raw file with a list of patches. */
void patch_write_raw(char *path, TAGLIST *patch);
/** Patch an init file with a list of patches. */
void patch_write_init(char *path, TAGLIST *patch);