/**
 * @file audio_management.h
 * @author Thomas Bujaki
 * @brief
 */
#include "SDL_mixer.h"

#define max_sound_effects 128
#define max_music_tracks 8

struct audio_assets {
	Mix_Chunk *sound_effects[max_sound_effects];
	Mix_Music *music[10];  //[max_music_tracks];
};

void init_audio(struct audio_assets *audio);
void change_music(struct audio_assets *audio, char filename[128], int track);
void play_music(Mix_Music *music, int loop);
void play_sound_effects(Mix_Chunk *sound_effect, int channel);