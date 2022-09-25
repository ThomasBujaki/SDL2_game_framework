/**
 * @file audio_management.c
 * @author Thomas Bujaki
 * @brief
 */

#include "audio_management.h"

void load_sound_effects(struct audio_assets *audio, char filepath[128]) {
	audio->sound_effects[0] = Mix_LoadWAV(filepath);
}

void load_music(struct audio_assets *audio, char filename[128]) {
	if (audio->music != NULL) {
		Mix_HaltMusic();
		Mix_FreeMusic(audio->music);
		audio->music = NULL;
	}

	audio->music = Mix_LoadMUS(filename);
}

void init_audio(struct audio_assets *audio) {
	// load_music(audio, "Audio_assets/MUSIC.ogg");

	load_sound_effects(audio, "Audio_assets/BEEP.ogg");
}

void play_music(Mix_Music *music, int loop) {
	Mix_PlayMusic(music, loop);
}

void play_sound_effects(Mix_Chunk *sound_effect, int channel) {
	Mix_PlayChannel(channel, sound_effect, 0);
}