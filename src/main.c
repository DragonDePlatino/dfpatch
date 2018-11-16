#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "console.h"
#include "file.h"
#include "patch.h"
#include "strmod.h"

void copy(char *path, char *newpath) {
	if (!path_isdir(path)) {
		printf("Could not find %s\n", path);
	} else if (!path_isdir(newpath)) {
		printf("Could not find %s\n", newpath);
	} else {
		printf("Copying %s...\n", path);
		dir_copy(path, newpath);
	}
}

void patch_file(char *path, char *newpath, TAGLIST* deflist, patch_cb patchfunc) {
	if (!path_isfile(path)) {
		printf("Could not find %s\n", path);
	} else if (!path_isfile(newpath)) {
		printf("Could not find %s\n", newpath);
	} else {
		TAGLIST *rawlist = patch_read_raw(path, deflist);
		printf("Patching %s...\n", strchr(path, '/') + 1);
		patchfunc(newpath, rawlist);
		taglist_free(rawlist);
	}
}

void patch_files(char *path, char *newpath, char *defpath, patch_cb patchfunc) {
	char **inpath;
	TAGLIST *deflist = NULL;
	int i = 0;

	if (path_isdir(path)) {
		inpath = dirlist_alloc(path, false);
	} else {
		printf("Could not find %s\n", path);
		return;
	}

	if (!inpath[i]) {
		printf("No files found in %s\n", path);
		dirlist_free(inpath);
		return;
	}

	if (path_isfile(defpath)) {
		deflist = patch_read_define(defpath);
	}

	while (inpath[i]) {
		char *outpath = concat(newpath, strchr(inpath[i], '/') + 1);
		patch_file(inpath[i], outpath, deflist, patchfunc);
		free(outpath);
		++i;
	}

	dirlist_free(inpath);
	if(deflist) {
		taglist_free(deflist);
	}
}

int main(void) {
	if(path_isfile("info.txt")) {
		file_print("info.txt");
	}

	puts("All of your raws will be copied into the backup folder.");
	if (console_yn("Continue with installation?") == 'y') {
		dir_create("backup");
		copy("../raw", "backup");
		copy("art", "../data");
		copy("graphics", "../raw");
		copy("init", "../data");
		copy("objects", "../raw");
		puts("");

		if (path_isfile("../dfhack.init")) {
			file_append("../dfhack.init", "twbt redraw_all 1");
		}

		patch_files("objects_patch/", "../raw/objects/", "defines.txt", patch_write_raw);
		patch_files("init_patch/", "../data/init/", "", patch_write_init);
		printf("\nInstallation complete. ");
	} else {
		printf("\nInstallation cancelled. ");
	}

	console_quit("");
	return 0;
}