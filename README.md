# ipoddb

Build the database file for an iPod Shuffle 4th Gen on Linux without the need for iTunes. Only MP3s are currently supported!
This will require a completely wiped and FAT32 formatted iPod Shuffle 4th Gen. PROCEED AT YOUR OWN RISK!

## Quickstart

1. Create the following folders at the root of the iPod:
```bash
mkdir -p <ipod_mountpoint>/iPod_Control/Music && mkdir -p <ipod_mountpoint>/iPod_Control/iTunes
```

2. Place all music (mp3s only) into <ipod_mountpoint>/iPod_Control/Music/

3. Create a list of all tracks using:
```bash
ls <ipod_mountpoint>/iPod_Control/Music/ > ./Music_List.txt
```

4. Run the app which will generate the "iTunesSD" file
```bash
./obj/iPod_DB_Builder
```

5. Copy "iTunesSD" into the iPod
```bash
cp iTunesSD <ipod_mountpoint>/iPod_Control/iTunes/iTunesSD
```

## References
--> Thanks to nims11 for providing the database structure!
```bash
https://github.com/nims11/IPod-Shuffle-4g/blob/master/docs/iTunesSD3gen.md
```
