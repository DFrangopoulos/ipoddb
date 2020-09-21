#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//Shuffle Database
struct shdb {
	uint32_t header_id;					
	uint32_t unknown_1;					
	uint32_t header_length;					
	uint32_t total_no_of_tracks;					
	uint32_t total_no_of_playlists;
	uint64_t unknown_2;
	uint8_t max_volume;
	uint8_t voiceover_enabled;
	uint16_t unknown_3;
	uint32_t total_no_of_tracks2;
	uint32_t track_header_chunk_offset;
	uint32_t playlist_header_chunk_offset;
	uint8_t unknown_4[20];	
};

//Shuffle Track Header
struct shth {
	uint32_t header_id;
	uint32_t total_length;
	uint32_t number_of_tracks;
	uint64_t unknown_1;
	uint32_t offset_of_track_chunk_X[1000];
};

//Shuffle TrackX
struct shtr {
	uint32_t header_id;
	uint32_t total_length;
	uint32_t start_at_pos_ms;
	uint32_t stop_at_pos_ms;
	uint32_t volume_gain;
	uint32_t filetype;
	uint8_t filename[256];
	uint32_t bookmark;
	uint8_t dont_skip_on_shuffle;
	uint8_t remember_playing_pos;
	uint8_t part_of_uninterruptable_album;
	uint8_t unknown_1;
	uint32_t pregap;
	uint32_t postgap;
	uint32_t number_of_samples;
	uint32_t unknown_file_related_data1;
	uint32_t gapless_data;
	uint32_t unknown_file_related_data2;
	uint32_t album_id;
	uint16_t track_number;
	uint16_t disc_number;
	uint64_t unknown_2;
	uint64_t dbid;
	uint32_t artist_id;
	uint8_t unknown_3[32];
};


//Shuffle Playlist Header
struct shph {
	uint32_t header_id;
	uint32_t total_length;
	uint32_t number_of_playlists;
	uint16_t number_of_playlists_1;
	uint16_t number_of_playlists_2;
	uint16_t number_of_playlists_3;
	uint16_t unknown_2;
	uint32_t offset_of_playlist_X;
};

//Shuffle PlaylistX
struct shpl {
	uint32_t header_id;
	uint32_t total_length;
	uint32_t number_of_songs;
	uint32_t number_of_songs2;
	uint64_t dbid;
	uint32_t type;
	uint8_t unknown_1[16];
	uint32_t playlist_track_X[1000];
};

/*Reads all track name and generates a track "file" for each song
 * while populating the filename section and returns the number of 
 * detected tracks*/
 
void ingest(FILE *file, struct shtr *shtr_all, uint32_t *nb_tracks);
void create_shth(struct shth *shth1, uint32_t *nb_tracks);
void create_shtrX(struct shtr *shtrX, uint32_t *nb_tracks);
void create_shpl(struct shpl *shpl1, uint32_t *nb_tracks);
void create_shph(struct shph *shph1, uint32_t *nb_tracks);
void create_shdb(struct shdb *shdb1, uint32_t *nb_tracks);
void print_shdb(FILE *file, struct shdb shdb_master);
void print_shth(FILE *file, struct shth x, uint32_t *nb_tracks);
void print_shtr(FILE *file, struct shtr x);
void print_shph(FILE *file, struct shph x);
void print_shpl(FILE *file, struct shpl x, uint32_t *nb_tracks);
