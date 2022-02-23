#ifndef CC_AUDIO_H
#define CC_AUDIO_H

#include <raylib.h>

#define CHANNEL_GROUP_MUS 0
#define CHANNEL_GROUP_SFX 1
#define CHANNEL_GROUP_VOX 2

bool CC_Audio_Init();

extern float volumeMusic;
extern float volumeSfx;
extern float volumeVocal;
extern char* musics[];
extern Sound buttyes;
extern Sound buttno;
#define kiringtone_files_len 72
extern char* kiringtone_files[kiringtone_files_len];
extern Sound kirins[];
extern int kiringtone_current;
extern Music currentMusic;

void CC_Audio_Play_Music(char* path);
void CC_Audio_Play_SFX(Sound sfx);
void CC_Audio_Play_VOX(Sound vox);

#endif