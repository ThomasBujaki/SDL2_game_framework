/**
 * @file audio_management.c
 * @author Thomas Bujaki
 * @brief
 */

#include "audio_management.h"

void load_sound_effects(struct audio_assets *audio, char filepath[128]) {
	audio->sound_effects[0] = Mix_LoadWAV(filepath);
	audio->sound_effects[1] = Mix_LoadWAV(filepath);
}

void load_music(struct audio_assets *audio, char filename[128], int track) {
	if (audio->music[track] != NULL) {
		Mix_HaltMusic();
		Mix_FreeMusic(audio->music[track]);
		audio->music[track] = NULL;
	}

	audio->music[track] = Mix_LoadMUS(filename);
}

void init_audio(struct audio_assets *audio) {
	int track = 0;
	load_music(audio, "Audio_assets/Atmosphere_1.ogg", track);
	track++;
	load_music(audio, "Audio_assets/Atmo_2.ogg", track);
	track++;
	load_music(audio, "Audio_assets/Atmo_3.ogg", track);
	track++;
	load_music(audio, "Audio_assets/Atmo_4.ogg", track);
	track++;
	load_music(audio, "Audio_assets/Atmo_5.ogg", track);
	track++;
	load_music(audio, "Audio_assets/bare_atmo.ogg", track);
	track++;
	load_music(audio, "Audio_assets/Dissonant.ogg", track);
	track++;
	load_music(audio, "Audio_assets/fast_dissonant.ogg", track);
	track++;
	load_music(audio, "Audio_assets/fast_pattern.ogg", track);
	track++;
	load_music(audio, "Audio_assets/medium_atmo.ogg", track);

	audio->sound_effects[0] = Mix_LoadWAV("Audio_assets/sword.ogg");
	audio->sound_effects[1] = Mix_LoadWAV("Audio_assets/shot.ogg");
	// load_sound_effects(audio, "Audio_assets/sword.ogg");
	// load_sound_effects(audio, "Audio_assets/shoot.ogg");
}

void change_music(struct audio_assets *audio, char filename[128], int track) {
	load_music(audio, filename, track);
}

void play_music(Mix_Music *music, int loop) {
	Mix_PlayMusic(music, loop);
}

void play_sound_effects(Mix_Chunk *sound_effect, int channel) {
	Mix_PlayChannel(channel, sound_effect, 0);
}