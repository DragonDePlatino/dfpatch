This installer aids players and modders by altering the files of [Dwarf Fortress](http://bay12games.com/dwarves/) with user-specified patch files. It works on all init files and raws, even heavily modded ones. Additionally, it can copy configuration files like init.txt, colors.txt and overrides.txt into the correct folders.

## Usage
Move the entire folder into a directory containing a `/data` or `/raw` folder, then run `dfpatch.exe`.

## Files
The following optional files should be included in your mod's download:

```
dfpatch.exe
info.txt
defines.txt
/art
	...
/graphics
	...
/init
	...
/init_patch
	...
/objects
	...
/objects_patch
	...
```

`info.txt` contains user information printed when the installer is run. `defines.txt` contains a list of preprocessor variables used when patching files. The contents of `/init` and `/objects` will be copied verbatim into `../data/init` and `../raw/objects`, respectively. The files in `/init_patch` and `/objects_path` describe a list of changes to make to the files in `../data/init` and `../raw/objects`, respectively.

## Defines
DFPatch offers the option to replace numbers with human-readable values when writing patch files. These are written in `defines.txt` like:

```
#BLACK 0
#GREEN 2
#2LEG_SMALL 66
#BEAST 71
```

Where every identifier is unique and the values can range from -2147483648 to 2147483647.
## Patches
The main feature of DFPatch is to allow modders to specify _changes_ to files rather than entire files. These are written in patch files such as:

```
[CREATURE:HIPPO]
	[CREATURE_TILE:BULKY]
	[COLOR:GRAY:BLACK:BLACK]
[CREATURE:HIPPO_MAN]
	[CREATURE_TILE:BULKY_MAN][COLOR:GRAY:BLACK:BLACK]
	[APPLY_CREATURE_VARIATION:STANDARD_SWIMMING_GAITS|2990:2257:1525:731:4300:6100]
[CREATURE:GIANT_HIPPO]
	[CREATURE_TILE:BULKY]
		[COLOR:GRAY:BLACK:BLACK]
```

The name of these patch files much match the ones in `../data/init` and `../raw/objects`. Entity names are not indented and entity tags must have one or more levels of indentation. Defines can be used in these files and will automatically be replaced with integers during patching. If a define isn't found, it will be left as-is instead of being converted.

By default, when DFPatch is matching tags it will compare the text before the first colon to check for replacement. An example is that `[TAGNAME:321]` in the raws would be patched with `[TAGNAME:400]`. However, There are situations where you need to be more specific. If an entity has `[TAGNAME:VAR1:134]` and `[TAGNAME:VAR2:71]` then the patch `[TAGNAME:VAR2:378]` would replace the first tag starting with `TAGNAME` that it finds. To correct this, the modder can write the patch as `[TAGNAME:VAR2|378]` and it will explicitly match everything before the pipe.

Note that DFPatch has limitations: it cannot remove or check the contents of tags. Any text enclosed in brackets is considered a valid tag, e.g. [a1d2jckf:&12#]. The installer will print a warning if a tag contains line breaks or is missing a bracket, but that's it. DFPatch isn't meant to be feature-rich, just fast and simple!