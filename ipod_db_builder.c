#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

/* ONLY TRACKS AND MP3s*/

/*Shuffle Database*/
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

/*Shuffle Track Header*/
struct shth {
	
	uint32_t header_id;
	uint32_t total_length;
	uint32_t number_of_tracks;
	uint64_t unknown_1;
	uint32_t offset_of_track_chunk_X[1000];
	/*....*/
};


/*Shuffle TrackX*/
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


/*Shuffle Playlist Header*/
struct shph {
	
	uint32_t header_id;
	uint32_t total_length;
	uint32_t number_of_playlists;
	uint16_t number_of_playlists_1;
	uint16_t number_of_playlists_2;
	uint16_t number_of_playlists_3;
	uint16_t unknown_2;
	uint32_t offset_of_playlist_X;
	/*....*/
};

/*Shuffle PlaylistX*/
struct shpl {
	
	uint32_t header_id;
	uint32_t total_length;
	uint32_t number_of_songs;
	uint32_t number_of_songs2;
	uint64_t dbid;
	uint32_t type;
	uint8_t unknown_1[16];
	uint32_t playlist_track_X[1000];
	/*....*/
};




/*Reads all track name and generates a track "file" for each song
 * while populating the filename section and returns the number of 
 * detected tracks*/
void ingest(FILE *file, struct shtr *shtr_all, uint32_t *nb_tracks);

void create_shth(struct shth *shth1,uint32_t *nb_tracks);

void create_shtrX(struct shtr *shtrX,uint32_t *nb_tracks);

void create_shpl(struct shpl *shpl1,uint32_t *nb_tracks);

void create_shph(struct shph *shph1,uint32_t *nb_tracks);

void create_shdb(struct shdb *shdb1,uint32_t *nb_tracks);


void print_shdb(FILE *file, struct shdb shdb_master);
void print_shth(FILE *file, struct shth x, uint32_t *nb_tracks);
void print_shtr(FILE *file, struct shtr x);
void print_shph(FILE *file, struct shph x);
void print_shpl(FILE *file, struct shpl x, uint32_t *nb_tracks);

int main()

{
	struct shdb shdb1[1];
	struct shth shth1[1];
	struct shtr shtrX[1000];
	struct shph shph1[1];
	struct shpl shpl1[1];
	
	uint32_t nb_tracks = 0;
	
	FILE *in_file = fopen("Music_List.txt", "r");
	FILE *out_file = fopen("iTunesSD", "w");
	if (in_file == NULL || out_file == NULL) 
	{   
	  printf("Error! Could not open file\n"); 
	  exit(-1);
	} 
	
	/*Read Tracks*/
	ingest(in_file, shtrX, &nb_tracks);
	
	/*Create Track Header Data*/
	create_shth(shth1,&nb_tracks);
	
	/*Create All Track Data*/
	create_shtrX(shtrX,&nb_tracks);
	
	/*Create Master Playlist Data */
	create_shpl(shpl1,&nb_tracks);
	
	/*Create Master Playlist Header Data*/
	create_shph(shph1,&nb_tracks);
	
	/*Create Database Data*/
	create_shdb(shdb1,&nb_tracks);
	
	
	print_shdb(out_file, shdb1[0]);
	print_shth(out_file, shth1[0], &nb_tracks);
	for(int i=0;i<nb_tracks;i++)
	{
		print_shtr(out_file, shtrX[i]);
	}
	print_shph(out_file, shph1[0]);
	print_shpl(out_file, shpl1[0], &nb_tracks);
	
	for(int i=0;i<nb_tracks;i++)
	{
		printf("%s\n",shtrX[i].filename);
	}
	printf("iTunesSD Ready!\n");


	fclose(out_file);
	return 0;

}


void ingest(FILE *file, struct shtr *shtr_all, uint32_t *nb_tracks)
{
	char intro[20] = {'/','i','P','o','d','_','C','o','n','t','r','o','l','/','M','u','s','i','c','/'};
	
	char temp = 0x00;
	char buffer[236];
	
	while (temp != EOF)
	{
		for (int i=0; i<236; i++)
		{
			buffer[i]=0x00;
		}
		
		for (int i=0; i<236; i++)
		{
			temp = fgetc(file);
			if ((temp != 0x0a) && (temp != EOF)) {buffer[i]=temp;}
			else {break;}
		}
		if(temp != EOF)
		{
			for (int i=0; i<20; i++)
			{
				shtr_all[*nb_tracks].filename[i]=intro[i];
			}
			for (int i=0; i<236; i++)
			{
				shtr_all[*nb_tracks].filename[20+i]=buffer[i];
			}
			*nb_tracks=*nb_tracks+1;			
		}
	}
}

void create_shth(struct shth *shth1,uint32_t *nb_tracks)
{
	
	shth1[0].header_id=0x73687468;
	shth1[0].total_length=20+4*(*nb_tracks);
	shth1[0].number_of_tracks=*nb_tracks;
	shth1[0].unknown_1=0x0000000000000000;
	
	for (int i=0; i<(*nb_tracks) ; i++)
	{
		shth1[0].offset_of_track_chunk_X[i]=64+shth1[0].total_length+i*372;
	}
	
}

void create_shtrX(struct shtr *shtrX,uint32_t *nb_tracks)
{
	for (int i=0; i<(*nb_tracks) ; i++)
	{
		shtrX[i].header_id=0x73687472;
		shtrX[i].total_length=0x00000174;
		shtrX[i].start_at_pos_ms=0x00000000;
		shtrX[i].stop_at_pos_ms=0x00000000;
		shtrX[i].volume_gain=0x00000000;
		shtrX[i].filetype=0x00000001;
		shtrX[i].bookmark=0x00000000;
		shtrX[i].dont_skip_on_shuffle=0x01;
		shtrX[i].remember_playing_pos=0x00;
		shtrX[i].part_of_uninterruptable_album=0x00;
		shtrX[i].unknown_1=0x00;
		shtrX[i].pregap=0x00000200;
		shtrX[i].postgap=0x00000200;
		shtrX[i].number_of_samples=0x00000000;
		shtrX[i].unknown_file_related_data1=0x00000000;
		shtrX[i].gapless_data=0x00000000;
		shtrX[i].unknown_file_related_data2=0x00000000;
		shtrX[i].album_id=0x00000000;
		shtrX[i].track_number=0x0001;
		shtrX[i].disc_number=0x0000;
		shtrX[i].unknown_2=0x0000000000000000;
		shtrX[i].dbid=0x0000000000000000;
		shtrX[i].artist_id=0x00000000;
		for(int j=0; j<32; j++)
		{
			shtrX[i].unknown_3[j]=0x00;
		}
	}
}

void create_shpl(struct shpl *shpl1,uint32_t *nb_tracks)
{
	shpl1[0].header_id = 0x7368706c;
	shpl1[0].total_length=44+4*(*nb_tracks);
	shpl1[0].number_of_songs=*nb_tracks;
	shpl1[0].number_of_songs2=*nb_tracks;
	shpl1[0].dbid=0x0000000000000000;
	shpl1[0].type=0x00000001;
	for (int i=0; i<16; i++)
	{
		shpl1[0].unknown_1[i]=0x00;
	}
	for (int i=0; i<(*nb_tracks); i++)
	{
		shpl1[0].playlist_track_X[i]=i;
	}
}

void create_shph(struct shph *shph1,uint32_t *nb_tracks)
{
	shph1[0].header_id=0x73687068;
	shph1[0].total_length=0x00000018;
	shph1[0].number_of_playlists=0x00000001;
	shph1[0].number_of_playlists_1=0xffff;
	shph1[0].number_of_playlists_2=0x0001;
	shph1[0].number_of_playlists_3=0xffff;
	shph1[0].unknown_2=0x0000;
	shph1[0].offset_of_playlist_X=64+20+4*(*nb_tracks)+372*(*nb_tracks)+24;
}

void create_shdb(struct shdb *shdb1,uint32_t *nb_tracks)
{
		
	shdb1[0].header_id=0x73686462;					
	shdb1[0].unknown_1=0x02000003;					
	shdb1[0].header_length=0x00000040;					
	shdb1[0].total_no_of_tracks=*nb_tracks;					
	shdb1[0].total_no_of_playlists=0x00000001;
	shdb1[0].unknown_2=0x0000000000000000;
	shdb1[0].max_volume=0x00;
	shdb1[0].voiceover_enabled=0x00;
	shdb1[0].unknown_3=0x0000;
	shdb1[0].total_no_of_tracks2=*nb_tracks;
	shdb1[0].track_header_chunk_offset=0x00000040;
	shdb1[0].playlist_header_chunk_offset=64+20+4*(*nb_tracks)+372*(*nb_tracks);
	for(int i=0; i<20;i++)
	{
		shdb1[0].unknown_4[i]=0x00;
	}
}


void print_shdb(FILE *file, struct shdb shdb_master)
{
	
	fwrite(&shdb_master.header_id, sizeof(shdb_master.header_id), 1, file);
	fwrite(&shdb_master.unknown_1, sizeof(shdb_master.unknown_1), 1, file);
	fwrite(&shdb_master.header_length, sizeof(shdb_master.header_length), 1, file);
	fwrite(&shdb_master.total_no_of_tracks, sizeof(shdb_master.total_no_of_tracks), 1, file);
	fwrite(&shdb_master.total_no_of_playlists, sizeof(shdb_master.total_no_of_playlists), 1, file);
	fwrite(&shdb_master.unknown_2, sizeof(shdb_master.unknown_2), 1, file);
	fwrite(&shdb_master.max_volume, sizeof(shdb_master.max_volume), 1, file);
	fwrite(&shdb_master.voiceover_enabled, sizeof(shdb_master.voiceover_enabled), 1, file);
	fwrite(&shdb_master.unknown_3, sizeof(shdb_master.unknown_3), 1, file);
	fwrite(&shdb_master.total_no_of_tracks2, sizeof(shdb_master.total_no_of_tracks2), 1, file);
	fwrite(&shdb_master.track_header_chunk_offset, sizeof(shdb_master.track_header_chunk_offset), 1, file);
	fwrite(&shdb_master.playlist_header_chunk_offset, sizeof(shdb_master.playlist_header_chunk_offset), 1, file);
	fwrite(&shdb_master.unknown_4, sizeof(shdb_master.unknown_4), 1, file);	
}

void print_shth(FILE *file, struct shth x, uint32_t *nb_tracks)
{
	fwrite(&x.header_id, sizeof(x.header_id), 1, file);
	fwrite(&x.total_length, sizeof(x.total_length), 1, file);
	fwrite(&x.number_of_tracks, sizeof(x.number_of_tracks), 1, file);
	fwrite(&x.unknown_1, sizeof(x.unknown_1), 1, file);
	
	for(int i=0;i<(*nb_tracks);i++)
	{
		fwrite(&x.offset_of_track_chunk_X[i], sizeof(x.offset_of_track_chunk_X[i]), 1, file);
	}
}

void print_shtr(FILE *file, struct shtr x)
{
	fwrite(&x.header_id, sizeof(x.header_id), 1, file);
	fwrite(&x.total_length, sizeof(x.total_length), 1, file);
	fwrite(&x.start_at_pos_ms, sizeof(x.start_at_pos_ms), 1, file);
	fwrite(&x.stop_at_pos_ms, sizeof(x.stop_at_pos_ms), 1, file);
	fwrite(&x.volume_gain, sizeof(x.volume_gain), 1, file);
	fwrite(&x.filetype, sizeof(x.filetype), 1, file);
	fwrite(&x.filename, sizeof(x.filename), 1, file);
	fwrite(&x.bookmark, sizeof(x.bookmark), 1, file);
	fwrite(&x.dont_skip_on_shuffle, sizeof(x.dont_skip_on_shuffle), 1, file);
	fwrite(&x.remember_playing_pos, sizeof(x.remember_playing_pos), 1, file);
	fwrite(&x.part_of_uninterruptable_album, sizeof(x.part_of_uninterruptable_album), 1, file);
	fwrite(&x.unknown_1, sizeof(x.unknown_1), 1, file);
	fwrite(&x.pregap, sizeof(x.pregap), 1, file);
	fwrite(&x.postgap, sizeof(x.postgap), 1, file);
	fwrite(&x.number_of_samples, sizeof(x.number_of_samples), 1, file);
	fwrite(&x.unknown_file_related_data1, sizeof(x.unknown_file_related_data1), 1, file);
	fwrite(&x.gapless_data, sizeof(x.gapless_data), 1, file);
	fwrite(&x.unknown_file_related_data2, sizeof(x.unknown_file_related_data2), 1, file);
	fwrite(&x.album_id, sizeof(x.album_id), 1, file);
	fwrite(&x.track_number, sizeof(x.track_number), 1, file);
	fwrite(&x.disc_number, sizeof(x.disc_number), 1, file);
	fwrite(&x.unknown_2, sizeof(x.unknown_2), 1, file);
	fwrite(&x.dbid, sizeof(x.dbid), 1, file);
	fwrite(&x.artist_id, sizeof(x.artist_id), 1, file);
	fwrite(&x.unknown_3, sizeof(x.unknown_3), 1, file);
}


void print_shph(FILE *file, struct shph x)
{
	fwrite(&x.header_id, sizeof(x.header_id), 1, file);
	fwrite(&x.total_length, sizeof(x.total_length), 1, file);
	fwrite(&x.number_of_playlists, sizeof(x.number_of_playlists), 1, file);
	fwrite(&x.number_of_playlists_1, sizeof(x.number_of_playlists_1), 1, file);
	fwrite(&x.number_of_playlists_2, sizeof(x.number_of_playlists_2), 1, file);
	fwrite(&x.number_of_playlists_3, sizeof(x.number_of_playlists_3), 1, file);
	fwrite(&x.unknown_2, sizeof(x.unknown_2), 1, file);
	fwrite(&x.offset_of_playlist_X, sizeof(x.offset_of_playlist_X), 1, file);
}


void print_shpl(FILE *file, struct shpl x, uint32_t *nb_tracks)
{
	
	fwrite(&x.header_id, sizeof(x.header_id), 1, file);
	fwrite(&x.total_length, sizeof(x.total_length), 1, file);
	fwrite(&x.number_of_songs, sizeof(x.number_of_songs), 1, file);
	fwrite(&x.number_of_songs2, sizeof(x.number_of_songs2), 1, file);
	fwrite(&x.dbid, sizeof(x.dbid), 1, file);
	fwrite(&x.type, sizeof(x.type), 1, file);
	fwrite(&x.unknown_1, sizeof(x.unknown_1), 1, file);
	for(int i=0;i<(*nb_tracks);i++)
	{
		fwrite(&x.playlist_track_X[i], sizeof(x.playlist_track_X[i]), 1, file);
	}
}


