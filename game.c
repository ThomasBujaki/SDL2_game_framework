/**
 * @file game.c
 * @author Thomas Bujaki
 * @brief Main file for simple setup for game creation
 * @version 0.1
 * @date 2022-09-25
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asset_management.h"

struct argument_parameters {
	int temp;
};

struct frame_time_info {
	long current_tick_time;
	long previous_tick_time;
	double frame_rate_target;
	double ms_per_frame;
	double ms_remainder;  // remainder of time caused by dividing 1000 by the frame rate target
	int num_updates;
	int ticks_passed;
	double current_framerate;
};

void init_frame_rate_parameters(struct frame_time_info *frames_data) {
	frames_data->frame_rate_target = 60;
	frames_data->ms_per_frame = 1000 / frames_data->frame_rate_target;
}

void init_SDL_settings() {
}

int init_arguments() {
	return 0;
}

int process_arguments(int ac, char **av, struct argument_parameters *arguments) {
	return 0;
}

void calculate_framerate(struct frame_time_info *frames_data) {
	frames_data->num_updates++;
	frames_data->ticks_passed += (SDL_GetTicks() - frames_data->previous_tick_time);
	if (frames_data->ticks_passed > 500) {
		frames_data->current_framerate = 1000 / ((double)frames_data->ticks_passed / frames_data->num_updates);

		frames_data->num_updates = 0;
		frames_data->ticks_passed = 0;
	}
}

void frame_rate_cap(struct frame_time_info *frames_data) {
	frames_data->current_tick_time = SDL_GetTicks();

	// minus 1 since we have a sdl delay of 1 in the main loop just in case the program is behind it allows some delay
	int wait = (int)frames_data->ms_per_frame - 1 - (frames_data->current_tick_time - frames_data->previous_tick_time);
	frames_data->ms_remainder += frames_data->ms_per_frame - (int)frames_data->ms_per_frame;

	if (wait < 0) {	 // already behind time, dont wait before the next update
		return;
	}
	if (frames_data->ms_remainder > 1) {
		wait += (int)frames_data->ms_remainder;
		frames_data->ms_remainder -= (int)frames_data->ms_remainder;
	}
	SDL_Delay(wait);
}

int main(int argc, char *argv[]) {
	struct top_level_window game_app = {};
	struct events_data user_input = {};
	struct audio_assets audio = {};

	// FPS
	struct frame_time_info frames_data = {};
	init_frame_rate_parameters(&frames_data);

	// struct mouse_data mouse_input = {};

	top_level_window_ptr = &game_app;  // no touching this pointer unless in atexit >:(
	init_window(&game_app);
	atexit(cleanup);

	// Assets
	struct asset_information asset_1 = {};
	struct text_information text_1 = {};
	struct text_information frames_text = {};

	// init Assets
	init_asset_dimensions(&asset_1, 100, 100, 100, 100);
	SDL_Colour colour = {0, 0, 0};
	init_text_information(&text_1, "It was the beepst of times, it was the boopst of times.\0", colour, 24, 200, 500, 750, 100);
	init_text_information(&frames_text, "\0", colour, 128, 900, 0, 300, 120);

	asset_1.asset_texture = load_asset(&game_app, "Texture_assets/Hello_World.png");
	init_audio(&audio);

	// play_music(audio.music, -1);

	// main loop

	while (1) {
		frames_data.previous_tick_time = SDL_GetTicks();  // time at start
		prep_screen(&game_app);

		process_input(&user_input);
		if (user_input.mouse_clicked == true) {
			play_sound_effects(audio.sound_effects[0], 0);	// change the tailing 0 to change the channel that audio is played from
			user_input.mouse_clicked = false;
		}

		update_asset_location_user_input(&asset_1, &user_input, 5);	 //
		update_asset_size(&asset_1, &user_input);

		draw_texture(&game_app, &asset_1);
		draw_text(&game_app, &text_1);
		draw_text(&game_app, &frames_text);

		present_screen(&game_app);

		SDL_Delay(1);
		sprintf(frames_text.text, "%.2lf FPS", frames_data.current_framerate);

		frame_rate_cap(&frames_data);
		calculate_framerate(&frames_data);
	}

	return 0;
}

// get tick
// run program
//