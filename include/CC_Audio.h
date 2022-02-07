#ifndef CC_AUDIO_H
#define CC_AUDIO_H

#include <SDL2/SDL_mixer.h>

#define CHANNEL_GROUP_MUS 0
#define CHANNEL_GROUP_SFX 1
#define CHANNEL_GROUP_VOX 2

void CC_Audio_Init();

extern float volumeMusic;
extern float volumeSfx;
extern float volumeVocal;
extern char* musics[];
extern Mix_Music* buttyes;
extern Mix_Music* buttno;
#define kiringtone_files_len 72
extern char* kiringtone_files[kiringtone_files_len];
extern Mix_Music* kirins[];
extern int kiringtone_current;

void CC_Audio_Play_Music(char* path);
void CC_Audio_Play_SFX(Mix_Music* sfx);
void CC_Audio_Play_VOX(Mix_Music* vox);

#endif