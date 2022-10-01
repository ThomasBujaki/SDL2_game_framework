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

#include "world.h"

#define ONE_SEC_AS_NANO 1000000000

struct argument_parameters {
};

struct frame_time_info {
	struct timespec prev_time;
	int frame_rate_target;
	long ns_per_frame;
	int num_updates;
	int current_framerate;
};

void init_frame_rate_parameters(struct frame_time_info *frames_data) {
	frames_data->frame_rate_target = 60;
	frames_data->ns_per_frame = ONE_SEC_AS_NANO / frames_data->frame_rate_target;
}

void init_SDL_settings() {
}

int init_arguments() {
	return 0;
}

int process_arguments(int ac, char **av, struct argument_parameters *arguments) {
	return 0;
}

long nano_difference(struct timespec a, struct timespec b) {
	return a.tv_nsec - b.tv_nsec + (a.tv_sec - b.tv_sec) * ONE_SEC_AS_NANO;
}

struct timespec nano_add(struct timespec time, long nanos) {
	struct timespec new_time = {};
	new_time.tv_sec = time.tv_sec + (nanos / ONE_SEC_AS_NANO);
	new_time.tv_nsec = time.tv_nsec + (nanos % ONE_SEC_AS_NANO);
	if (new_time.tv_nsec >= ONE_SEC_AS_NANO) {
		new_time.tv_nsec -= ONE_SEC_AS_NANO;
		new_time.tv_sec++;
	}
	return new_time;
}

void calculate_framerate(struct frame_time_info *frames_data) {
	struct timespec right_now = {};
	clock_gettime(CLOCK_MONOTONIC, &right_now);

	if (nano_difference(right_now, frames_data->prev_time) >= ONE_SEC_AS_NANO) {
		clock_gettime(CLOCK_MONOTONIC, &frames_data->prev_time);
		frames_data->current_framerate = frames_data->num_updates;
		frames_data->num_updates = 0;
	}
	frames_data->num_updates++;
}

void frame_rate_cap(struct frame_time_info *frames_data) {
	struct timespec wait = {};
	struct timespec right_now = {};

	struct timespec expected_frame_end_time = nano_add(frames_data->prev_time, frames_data->num_updates * frames_data->ns_per_frame);
	clock_gettime(CLOCK_MONOTONIC, &right_now);
	wait.tv_nsec = nano_difference(expected_frame_end_time, right_now);

	if (wait.tv_nsec > 0) {
		nanosleep(&wait, NULL);
	}
}

int main(int argc, char *argv[]) {
	struct top_level_window game_app = {};
	struct events_data user_input = {};
	struct audio_assets audio = {};
	// FPS
	//	struct timespec tstart = {0, 0}, tend = {0, 0};

	struct frame_time_info frames_data = {};
	init_frame_rate_parameters(&frames_data);

	// struct mouse_data mouse_input = {};

	top_level_window_ptr = &game_app;  // no touching this pointer unless in atexit >:(
	init_window(&game_app);
	atexit(cleanup);

	// World
	struct world_assets assets_of_world = {};
	struct world_state state_of_world = {};

	// init Assets
	// todo move inits to a different function
	struct asset_information *asset_list[50000] = {};
	int i = 0, j = 0;
	struct asset_information player_asset = {};

	asset_list[i] = &player_asset;
	init_asset_dimensions(&player_asset, 100, 100, 100, 100, 0);
	i++;

	player_asset.texture = load_texture(&game_app, "Texture_assets/player.png");

	struct asset_information alien_asset = {};
	asset_list[i] = &alien_asset;
	init_asset_dimensions(&alien_asset, 500, 250, 100, 100, 0);
	i++;

	alien_asset.texture = load_texture(&game_app, "Texture_assets/alien.png");

	struct asset_information portal_asset = {};
	asset_list[i] = &portal_asset;
	init_asset_dimensions(&portal_asset, 500, 250, 100, 100, 0);
	i++;

	portal_asset.texture = load_texture(&game_app, "Texture_assets/portal.png");

	assets_of_world.grass_tile = load_texture(&game_app, "Texture_assets/grass.png");

	// todo, make asset for health monitoring

	// init text
	SDL_Colour colour = {0, 0, 0};
	SDL_Colour red = {255, 0, 0};
	struct text_information text_1 = {};
	init_text_information(&text_1, "times.ttf", "It was the beepst of times, it was the boopst of times.\0", colour, 128, 200, 500, 750, 100);

	struct text_information health = {};
	init_text_information(&health, "times.ttf", "15/15\0", red, 64, 0, 50, 50, 50);

	struct text_information frames_text = {};
	init_text_information(&frames_text, "times.ttf", "\0", colour, 128, 900, 0, 300, 120);

	struct text_information collision_text = {};
	init_text_information(&collision_text, "times.ttf", "COLLISION!! LOOK OUT!!!1!\0", colour, 128, 500, 400, 600, 120);

	struct text_information tooltip = {};
	init_text_information(&tooltip, "times.ttf", "\0", colour, 16, 0, 0, 100, 40);

	struct text_information debug_output = {};
	init_text_information(&debug_output, "times.ttf", "\0", colour, 64, 0, 100, 100, 50);

	init_audio(&audio);

	// play_music(audio.music, -1);

	// main loop
	clock_gettime(CLOCK_MONOTONIC, &frames_data.prev_time);
	while (1) {
		//		clock_gettime(CLOCK_MONOTONIC, &tstart);

		prep_screen(&game_app);

		process_input(&user_input);

		if (user_input.mouse_clicked == true) {
			play_sound_effects(audio.sound_effects[0], 0);	// change the tailing 0 to change the channel that audio is played from
			user_input.mouse_clicked = false;
		}

		change_angle(&player_asset, &user_input, 10);
		update_player_location_user_input(&state_of_world, &user_input, 5);
		update_asset_size(&player_asset, &user_input);

		draw_world(&game_app, &state_of_world, &assets_of_world);
		draw_texture(&game_app, &player_asset);
		draw_texture(&game_app, &alien_asset);
		draw_text(&game_app, &text_1);
		draw_text(&game_app, &health);
		draw_text(&game_app, &frames_text);
		for (j = 0; j < i; j++) {
			if (is_moused_over(asset_list[j], &user_input)) {
				printf("AFTER\n");
				draw_text(&game_app, &collision_text);
			}
		}

		sprintf(debug_output.text, "X:%d Y:%d", user_input.mouse.x, user_input.mouse.y);
		draw_text(&game_app, &debug_output);

		if (collision_detection(&player_asset, &alien_asset)) {
			draw_text(&game_app, &collision_text);
		}

		present_screen(&game_app);

		sprintf(frames_text.text, "%d FPS", frames_data.current_framerate);

		calculate_framerate(&frames_data);
		frame_rate_cap(&frames_data);

		//		clock_gettime(CLOCK_MONOTONIC, &tend);
		//		printf("some_long_computation took about %.5f seconds\n",
		//			   ((double)tend.tv_sec + 1.0e-9 * tend.tv_nsec) -
		//				   ((double)tstart.tv_sec + 1.0e-9 * tstart.tv_nsec));
	}

	return 0;
}