#include "CC_Audio.h"
#include <stdio.h>

Music currentMusic;

char* musics[] = {
    "audio/music/vanillaSky.ogg",
    "audio/music/bluescreen.flac",
    "audio/music/melee.ogg",
    "audio/music/Rez_and_Kenet_-_Unreeeal_superhero_3.xm",
    "audio/music/01 - LaCrême, Matild - Fuis-Moi, Je Te Suis (Original Mix).mp3",
    "audio/music/mini1111.xm",
    "audio/music/starpathandmotherchip_etherwind.xm",
    "audio/music/4mat_-_eternity.xm"

};

char* kiringtone_files[] = {
"audio/sfx/kiringtone-split/chunk0.wav",
"audio/sfx/kiringtone-split/chunk1.wav",
"audio/sfx/kiringtone-split/chunk2.wav",
"audio/sfx/kiringtone-split/chunk3.wav",
"audio/sfx/kiringtone-split/chunk4.wav",
"audio/sfx/kiringtone-split/chunk5.wav",
"audio/sfx/kiringtone-split/chunk6.wav",
"audio/sfx/kiringtone-split/chunk7.wav",
"audio/sfx/kiringtone-split/chunk8.wav",
"audio/sfx/kiringtone-split/chunk9.wav",
"audio/sfx/kiringtone-split/chunk10.wav",
"audio/sfx/kiringtone-split/chunk11.wav",
"audio/sfx/kiringtone-split/chunk12.wav",
"audio/sfx/kiringtone-split/chunk13.wav",
"audio/sfx/kiringtone-split/chunk14.wav",
"audio/sfx/kiringtone-split/chunk15.wav",
"audio/sfx/kiringtone-split/chunk16.wav",
"audio/sfx/kiringtone-split/chunk17.wav",
"audio/sfx/kiringtone-split/chunk18.wav",
"audio/sfx/kiringtone-split/chunk19.wav",
"audio/sfx/kiringtone-split/chunk20.wav",
"audio/sfx/kiringtone-split/chunk21.wav",
"audio/sfx/kiringtone-split/chunk22.wav",
"audio/sfx/kiringtone-split/chunk23.wav",
"audio/sfx/kiringtone-split/chunk24.wav",
"audio/sfx/kiringtone-split/chunk25.wav",
"audio/sfx/kiringtone-split/chunk26.wav",
"audio/sfx/kiringtone-split/chunk27.wav",
"audio/sfx/kiringtone-split/chunk28.wav",
"audio/sfx/kiringtone-split/chunk29.wav",
"audio/sfx/kiringtone-split/chunk30.wav",
"audio/sfx/kiringtone-split/chunk31.wav",
"audio/sfx/kiringtone-split/chunk32.wav",
"audio/sfx/kiringtone-split/chunk33.wav",
"audio/sfx/kiringtone-split/chunk34.wav",
"audio/sfx/kiringtone-split/chunk35.wav",
"audio/sfx/kiringtone-split/chunk36.wav",
"audio/sfx/kiringtone-split/chunk37.wav",
"audio/sfx/kiringtone-split/chunk38.wav",
"audio/sfx/kiringtone-split/chunk39.wav",
"audio/sfx/kiringtone-split/chunk40.wav",
"audio/sfx/kiringtone-split/chunk41.wav",
"audio/sfx/kiringtone-split/chunk42.wav",
"audio/sfx/kiringtone-split/chunk43.wav",
"audio/sfx/kiringtone-split/chunk44.wav",
"audio/sfx/kiringtone-split/chunk45.wav",
"audio/sfx/kiringtone-split/chunk46.wav",
"audio/sfx/kiringtone-split/chunk47.wav",
"audio/sfx/kiringtone-split/chunk48.wav",
"audio/sfx/kiringtone-split/chunk49.wav",
"audio/sfx/kiringtone-split/chunk50.wav",
"audio/sfx/kiringtone-split/chunk51.wav",
"audio/sfx/kiringtone-split/chunk52.wav",
"audio/sfx/kiringtone-split/chunk53.wav",
"audio/sfx/kiringtone-split/chunk54.wav",
"audio/sfx/kiringtone-split/chunk55.wav",
"audio/sfx/kiringtone-split/chunk56.wav",
"audio/sfx/kiringtone-split/chunk57.wav",
"audio/sfx/kiringtone-split/chunk58.wav",
"audio/sfx/kiringtone-split/chunk59.wav",
"audio/sfx/kiringtone-split/chunk60.wav",
"audio/sfx/kiringtone-split/chunk61.wav",
"audio/sfx/kiringtone-split/chunk62.wav",
"audio/sfx/kiringtone-split/chunk63.wav",
"audio/sfx/kiringtone-split/chunk64.wav",
"audio/sfx/kiringtone-split/chunk65.wav",
"audio/sfx/kiringtone-split/chunk66.wav",
"audio/sfx/kiringtone-split/chunk67.wav",
"audio/sfx/kiringtone-split/chunk68.wav",
"audio/sfx/kiringtone-split/chunk69.wav",
"audio/sfx/kiringtone-split/chunk70.wav",
"audio/sfx/kiringtone-split/chunk71.wav",
};

Sound kirins[kiringtone_files_len];
int kiringtone_current = 0;

Sound buttyes;
Sound buttno;


// TODO(#5): Read audio settings from save file
float volumeMusic = 0;
float volumeSfx = 0;
float volumeVocal = 0;

bool CC_Audio_Init() {
    InitAudioDevice();
    if (IsAudioDeviceReady()) {
        printf("Audio ready\n");
    }
    else{
        printf("Audio borked\n");
        return false;
    }
    buttyes = LoadSound("audio/Access_Denied_High_DDM16_quieter.wav");
    buttno =  LoadSound("audio/Cancel Action_3.wav");
    for(int i = 0; i < kiringtone_files_len; i++) {
        kirins[i] = LoadSound(kiringtone_files[i]);
    }
    return true;
}

void CC_Audio_Play_Music(char* path) {
    currentMusic = LoadMusicStream(path);
    PlayMusicStream(currentMusic);
    SetMusicVolume(currentMusic, volumeMusic/100);
    printf("is music playing? %d\n", IsMusicStreamPlaying(currentMusic));
}

void CC_Audio_Play_SFX(Sound sfx) {
    SetSoundVolume(sfx, volumeSfx/100);
    PlaySound(sfx);
}

void CC_Audio_Play_VOX(Sound vox) {
    SetSoundVolume(vox, volumeVocal/100);
    PlaySound(vox);
}
