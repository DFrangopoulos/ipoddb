# ipoddb

Build the database file for an iPod Shuffle 4th Gen. ONLY MP3s supported.

## Running
1. Clone repo.
2. Mount iPod and completely wipe it.
3. Create Folders:

```bash
mkdir -p <ipod_mountpoint>/iPod_Control/Music && mkdir -p <ipod_mountpoint>/iPod_Control/iTunes
```
4. Place all music (mp3s only) into <ipod_mountpoint>/iPod_Control/Music/
5. Create a list of all tracks using:

```bash
ls <ipod_mountpoint>/iPod_Control/Music/ > ./Music_List.txt
```

6. Run the app

```bash
./ipod_db_builder
```
7. Copy "iTunesSD" into the iPod

```bash
cp iTunesSD <ipod_mountpoint>/iPod_Control/iTunes/iTunesSD
```

#References
```bash
https://github.com/nims11/IPod-Shuffle-4g/blob/master/docs/iTunesSD3gen.md
```
