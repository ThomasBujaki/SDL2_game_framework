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
	int attack_animation_frames = 0;
	// FPS
	// struct timespec tstart = {0, 0}, tend = {0, 0};

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
	struct asset_information *asset_list[100000] = {};
	int asset_num = 0, j = 0, a = 0;

	struct asset_information player_asset = {};
	asset_list[asset_num] = &player_asset;
	init_asset_dimensions(&player_asset, 0, 0, 0, 0, 30, 30, 0, "player\0", true, 5);
	player_asset.x = game_app.window_width / 2 - player_asset.width / 2;  // put player in the center of the screen
	player_asset.y = game_app.window_height / 2 - player_asset.height / 2;
	player_asset.world_x = player_asset.x;
	player_asset.world_y = player_asset.y;
	asset_num++;
	player_asset.texture = load_texture(&game_app, "Texture_assets/player.png");

	struct asset_information tree_asset = {};
	asset_list[asset_num] = &tree_asset;
	init_asset_dimensions(&tree_asset, 0, 0, 0, 0, 20, 20, 0, "tree\0", true, 5);
	asset_num++;
	tree_asset.texture = load_texture(&game_app, "Texture_assets/tree.png");

	struct asset_information portal_asset = {};
	asset_list[asset_num] = &portal_asset;
	init_asset_dimensions(&portal_asset, 1000, 250, 1000, 250, 100, 100, 0, "portal\0", true, 0);
	asset_num++;
	portal_asset.texture = load_texture(&game_app, "Texture_assets/portal.png");

	struct asset_information alien_asset[64620] = {};  // 64620 is the triangle number of 360
	asset_list[asset_num] = &alien_asset[0];
	init_asset_dimensions(&alien_asset[0], portal_asset.world_x, portal_asset.world_y, portal_asset.world_x, portal_asset.world_y, 20, 20, 0, "alien\0", true, 4);
	asset_num++;
	alien_asset[0].texture = load_texture(&game_app, "Texture_assets/alien.png");
	for (j = 1; j < 64620; j++) {
		asset_list[asset_num] = &alien_asset[j];
		init_asset_dimensions(&alien_asset[j], portal_asset.world_x, portal_asset.world_y, portal_asset.world_x + portal_asset.width / 2, portal_asset.world_y + portal_asset.height / 2, 20, 20, 0, "alien\0", false, 4);
		asset_num++;
		alien_asset[j].texture = alien_asset[0].texture;
	}

	struct asset_information crosshair_asset = {};
	init_asset_dimensions(&crosshair_asset, 0, 0, 0, 0, 20, 20, 0, "\0", true, 0);
	crosshair_asset.texture = load_texture(&game_app, "Texture_assets/crosshair.png");

	struct asset_information sword_asset = {};
	init_asset_dimensions(&sword_asset, 100, 100, 100, 100, 10, 20, 0, "\0", true, 0);
	sword_asset.texture = load_texture(&game_app, "Texture_assets/sword.png");

	struct asset_information on_screen = {};  // I know i probaby should have made a new struct for this
	init_asset_dimensions(&on_screen, 0, 0, 0, 0, game_app.window_width, game_app.window_height, 0, "\0", true, 0);

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

	struct text_information tooltip = {};
	init_text_information(&tooltip, "times.ttf", "\0", colour, 64, game_app.window_width / 2 - 100, 0, 200, 100);

	struct text_information alien_count = {};
	init_text_information(&alien_count, "times.ttf", "\0", colour, 64, 0, 150, 100, 50);

	struct text_information ten_sec_timer = {};
	init_text_information(&ten_sec_timer, "times.ttf", "\0", colour, 64, 0, 100, 50, 50);

	struct text_information debug_output = {};
	init_text_information(&debug_output, "times.ttf", "\0", colour, 64, 0, 100, 100, 50);

	struct text_information world_position = {};
	init_text_information(&world_position, "times.ttf", "\0", colour, 64, 100, 0, 100, 50);

	init_audio(&audio);

	// play_music(audio.music, -1);

	// main loop
	struct timespec ten_second_timekeeping = {};
	clock_gettime(CLOCK_MONOTONIC, &frames_data.prev_time);
	ten_second_timekeeping = frames_data.prev_time;
	int alien_spawn = 0, aliens_alive = 0;
	while (1) {
		//		clock_gettime(CLOCK_MONOTONIC, &tstart);

		prep_screen(&game_app);

		process_input(&user_input);

		if (user_input.mouse_clicked == true) {
			play_sound_effects(audio.sound_effects[0], 0);	// change the tailing 0 to change the channel that audio is played from
			user_input.mouse_clicked = false;
		}
		set_asset_position(&crosshair_asset, user_input.mouse.x - crosshair_asset.width / 2, user_input.mouse.y - crosshair_asset.height / 2);

		change_angle_to_mouse_dir(&player_asset, &user_input, &crosshair_asset);
		update_player_location_user_input(&state_of_world, &user_input, &player_asset);
		update_asset_size(&player_asset, &user_input);
		change_asset_world_position(&on_screen, state_of_world.world_offset_x, state_of_world.world_offset_y);
		draw_world(&game_app, &state_of_world, &assets_of_world);

		aliens_alive = 0;

		change_asset_world_position(&player_asset, state_of_world.world_offset_x, state_of_world.world_offset_y);
		change_asset_world_position(&on_screen, state_of_world.world_offset_x, state_of_world.world_offset_y);

		for (j = 0; j < asset_num; j++) {
			if (asset_list[j]->does_exist == true) {  // if the assets are actually in the world somewhere
				if (j != 0) {
					change_asset_world_position(asset_list[j], state_of_world.world_offset_x, state_of_world.world_offset_y);
				}
				// do updates of asset
				if (alien_asset[j].does_exist == true) {  // technically fine but not great as alien assets are a subset of asset lists
					aliens_alive++;
					move_aliens(asset_num, asset_list, &alien_asset[j], &player_asset);
				}

				if (collision_detection_with_screen(asset_list[j], &on_screen)) {
					asset_list[j]->is_drawn = true;

				} else {
					asset_list[j]->is_drawn = false;
				}
				// todo check if, based on world position, asset needs to be drawn
				if (asset_list[j]->is_drawn == true) {
					if (j != 0) {
						asset_list[j]->x = asset_list[j]->world_x;
						asset_list[j]->y = asset_list[j]->world_y;

						change_asset_position(asset_list[j], state_of_world.world_offset_x, state_of_world.world_offset_y);
					}
					// draw_texture(&game_app, &alien_asset[j]);
					// draw_texture(&game_app, &portal_asset);
					// draw_texture(&game_app, &tree_asset);
					if (j != 0) {
						did_player_kill_alien(asset_list[j], &sword_asset);
						draw_texture(&game_app, asset_list[j]);	 // draw all textures except player
					}
					if (is_moused_over(asset_list[j], &user_input)) {
						sprintf(tooltip.text, "%s", asset_list[j]->description);

						draw_text(&game_app, &tooltip);
					}
				}
			}
		}
		draw_texture(&game_app, &player_asset);
		attack_animation_frames = do_attack_animation(&game_app, &player_asset, &user_input, &sword_asset, attack_animation_frames);

		sprintf(alien_count.text, "Aliens: %d", aliens_alive);
		sprintf(world_position.text, "pos: %d %d", player_asset.world_x, player_asset.world_y);
		draw_text(&game_app, &alien_count);
		draw_text(&game_app, &text_1);
		draw_text(&game_app, &health);
		draw_text(&game_app, &frames_text);
		draw_text(&game_app, &world_position);
		draw_texture(&game_app, &crosshair_asset);

		// sprintf(debug_output.text, "X:%d Y:%d", user_input.mouse.x, user_input.mouse.y);
		// draw_text(&game_app, &debug_output);

		// ten second timekeeping
		if (frames_data.prev_time.tv_sec == ten_second_timekeeping.tv_sec + 10) {
			clock_gettime(CLOCK_MONOTONIC, &ten_second_timekeeping);
			alien_spawn++;
			for (a = aliens_alive; a < alien_spawn + aliens_alive; a++) {
				alien_asset[a].does_exist = true;
				alien_asset[a].world_x = portal_asset.world_x + portal_asset.width / 2;
				alien_asset[a].world_y = portal_asset.world_y + portal_asset.height / 2;
			}
		} else {
			sprintf(ten_sec_timer.text, "%ld", ten_second_timekeeping.tv_sec + 10 - frames_data.prev_time.tv_sec);
		}
		if (frames_data.prev_time.tv_sec > ten_second_timekeeping.tv_sec + 11) {
			clock_gettime(CLOCK_MONOTONIC, &ten_second_timekeeping);
		}
		draw_text(&game_app, &ten_sec_timer);

		// if (collision_detection(&player_asset, &alien_asset)) {
		//	draw_text(&game_app, &tooltip);
		// }

		present_screen(&game_app);

		sprintf(frames_text.text, "%d FPS", frames_data.current_framerate);

		calculate_framerate(&frames_data);
		frame_rate_cap(&frames_data);

		// clock_gettime(CLOCK_MONOTONIC, &tend);
		//  printf("some_long_computation took about %.5f seconds\n", ((double)tend.tv_sec + 1.0e-9 * tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9 * tstart.tv_nsec));
	}

	return 0;
}