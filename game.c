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

// todo: make sword sound, make gun sound
int main(int argc, char *argv[]) {
	struct top_level_window game_app = {};
	struct events_data user_input = {};
	struct audio_assets audio = {};
	int attack_animation_frames = 0, bullet_number = 0;
	int l;
	int track = 0;
	bool have_shot = false;
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
	init_asset_dimensions(&player_asset, 0, 0, 0, 0, 30, 30, 0, "player\0", true, 5, 100);
	player_asset.x = game_app.window_width / 2 - player_asset.width / 2;  // put player in the center of the screen
	player_asset.y = game_app.window_height / 2 - player_asset.height / 2;
	player_asset.world_x = player_asset.x;
	player_asset.world_y = player_asset.y;
	asset_num++;
	player_asset.texture = load_texture(&game_app, "Texture_assets/player_new.png");

	struct asset_information tree_asset[200] = {};
	asset_list[asset_num] = &tree_asset[0];
	init_asset_dimensions(&tree_asset[0], 0, 0, rand() % 4000 - 1500, rand() % 4000 - 2000, 40, 40, 0, "\0", true, 0, 10);
	asset_num++;
	tree_asset[0].texture = load_texture(&game_app, "Texture_assets/tree.png");
	for (j = 1; j < 200; j++) {
		init_asset_dimensions(&tree_asset[j], 0, 0, rand() % 4000 - 1500, rand() % 3000 - 1500, 40, 40, 0, "\0", true, 0, 10);
		asset_list[asset_num] = &tree_asset[j];
		asset_num++;
		tree_asset[j].texture = tree_asset[0].texture;
	}

	struct asset_information powerup[4] = {};
	asset_list[asset_num] = &powerup[0];
	init_asset_dimensions(&powerup[0], 100, 100, -1300, -1300, 200, 200, 0, "\0", true, 10, 1);
	asset_num++;
	powerup[0].texture = load_texture(&game_app, "Texture_assets/powerup.png");
	for (j = 1; j < 2; j++) {
		init_asset_dimensions(&powerup[j], 100, 100, 1500, 1500, 200, 200, 0, "\0", true, 10, 1);
		asset_list[asset_num] = &powerup[j];
		asset_num++;
		powerup[j].texture = powerup[0].texture;
	}

	struct asset_information portal_asset = {};
	asset_list[asset_num] = &portal_asset;
	init_asset_dimensions(&portal_asset, 1000, 250, 1000, 250, 100, 100, 0, "portal\0", true, 0, 1000);
	asset_num++;
	portal_asset.texture = load_texture(&game_app, "Texture_assets/portal.png");

	struct asset_information alien_asset[5000] = {};  // 64620 is the triangle number of 360
	asset_list[asset_num] = &alien_asset[0];
	init_asset_dimensions(&alien_asset[0], portal_asset.world_x, portal_asset.world_y, portal_asset.world_x, portal_asset.world_y, 20, 20, 0, "alien\0", true, 3, 1);
	asset_num++;
	alien_asset[0].texture = load_texture(&game_app, "Texture_assets/alien_new.png");
	for (j = 1; j < 5000; j++) {
		asset_list[asset_num] = &alien_asset[j];
		init_asset_dimensions(&alien_asset[j], portal_asset.world_x, portal_asset.world_y, portal_asset.world_x + portal_asset.width / 2, portal_asset.world_y + portal_asset.height / 2, 20, 20, 0, "alien\0", false, 4, 1);
		asset_num++;
		alien_asset[j].texture = alien_asset[0].texture;
	}

	struct asset_information crosshair_asset = {};
	init_asset_dimensions(&crosshair_asset, 0, 0, 0, 0, 20, 20, 0, "\0", true, 0, 0);
	crosshair_asset.texture = load_texture(&game_app, "Texture_assets/crosshair.png");

	struct asset_information sword_asset = {};
	init_asset_dimensions(&sword_asset, 100, 100, 100, 100, 10, 20, 0, "\0", true, 0, 1);
	sword_asset.texture = load_texture(&game_app, "Texture_assets/sword.png");

	struct asset_information gun_asset = {};
	init_asset_dimensions(&gun_asset, 100, 100, 100, 100, 10, 20, 0, "\0", true, 0, 1);
	gun_asset.texture = load_texture(&game_app, "Texture_assets/gun.png");

	struct asset_information bullet_asset[100] = {};
	init_asset_dimensions(&bullet_asset[0], 100, 100, 100, 100, 10, 20, 0, "\0", false, 10, 1);
	bullet_asset[0].texture = load_texture(&game_app, "Texture_assets/bullet.png");
	for (j = 1; j < 100; j++) {
		init_asset_dimensions(&bullet_asset[j], 100, 100, 100, 100, 10, 20, 0, "\0", false, 10, 1);
		bullet_asset[j].texture = bullet_asset[0].texture;
	}

	struct asset_information on_screen = {};  // I know i probaby should have made a new struct for this
	init_asset_dimensions(&on_screen, 0, 0, 0, 0, game_app.window_width, game_app.window_height, 0, "\0", true, 0, 0);

	assets_of_world.grass_tile = load_texture(&game_app, "Texture_assets/grass_new.png");

	// init text
	SDL_Colour colour = {0, 0, 0};
	SDL_Colour red = {255, 0, 0};
	SDL_Colour white = {255, 255, 255};
	struct text_information game_over = {};
	init_text_information(&game_over, "times.ttf", "Game Over\0", colour, 128, 200, 500, 750, 100);

	struct text_information health = {};
	init_text_information(&health, "times.ttf", "100/100\0", red, 64, 0, 50, 100, 50);

	struct text_information frames_text = {};
	init_text_information(&frames_text, "times.ttf", "\0", colour, 128, 900, 0, 300, 120);

	struct text_information tooltip = {};
	init_text_information(&tooltip, "times.ttf", "\0", colour, 64, game_app.window_width / 2 - 100, 0, 200, 100);

	struct text_information alien_count = {};
	init_text_information(&alien_count, "times.ttf", "\0", colour, 64, 0, 175, 100, 50);

	struct text_information ten_sec_timer = {};
	init_text_information(&ten_sec_timer, "times.ttf", "\0", colour, 64, 0, 100, 75, 75);

	struct text_information debug_output = {};
	init_text_information(&debug_output, "times.ttf", "\0", colour, 64, 0, 100, 100, 50);

	struct text_information world_position = {};
	init_text_information(&world_position, "times.ttf", "\0", white, 128, 100, 600, 800, 75);

	init_audio(&audio);

	play_music(audio.music[0], -1);

	// main loop
	struct timespec ten_second_timekeeping = {};
	clock_gettime(CLOCK_MONOTONIC, &frames_data.prev_time);
	ten_second_timekeeping = frames_data.prev_time;
	int alien_spawn = 1, aliens_alive = 0;
	while (1) {
		//		clock_gettime(CLOCK_MONOTONIC, &tstart);

		prep_screen(&game_app);

		process_input(&user_input);

		if (player_asset.world_x < -1225) {
			user_input.keyboard_events[right_key] = false;
		}
		if (player_asset.world_y < -1300) {
			user_input.keyboard_events[down_key] = false;
		}
		if (player_asset.world_x > 2750) {
			user_input.keyboard_events[left_key] = false;
		}
		if (player_asset.world_y > 2700) {
			user_input.keyboard_events[up_key] = false;
		}

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
				if (!strncmp(asset_list[j]->description, "alien", 5)) {
					if (asset_list[j]->does_exist == true) {  // technically fine but not great as alien assets are a subset of asset lists
						aliens_alive++;
						move_aliens(asset_num, asset_list, asset_list[j], &player_asset);
					}
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
						// printf("%s: %d %d\n", asset_list[j]->description, asset_list[j]->x, asset_list[j]->y);
						change_asset_position(asset_list[j], state_of_world.world_offset_x, state_of_world.world_offset_y);
						// printf("\t%d %d\n", state_of_world.world_offset_x, state_of_world.world_offset_y);
						// printf("%s: %d %d\n", asset_list[j]->description, asset_list[j]->x, asset_list[j]->y);
					}
					// draw_texture(&game_app, &alien_asset[j]);
					// draw_texture(&game_app, &portal_asset);
					// draw_texture(&game_app, &tree_asset);
					if (j != 0) {
						if (did_player_kill_alien(asset_list[j], &sword_asset) == true) {
							attack_animation_frames = 40;
						}
						for (l = 0; l < 100; l++) {
							did_bullet_kill_enemy(asset_list[j], &bullet_asset[l], &gun_asset);
						}

						// check each bullet to see if it collided with an alien or the portal
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
		if (user_input.keyboard_events[space_key] == true) {
			attack_animation_frames = do_attack_animation(&game_app, &player_asset, &user_input, &sword_asset, attack_animation_frames);
		} else if (user_input.keyboard_events[shift_key] == true) {
			attack_animation_frames = do_gun_animation(&game_app, &player_asset, &user_input, attack_animation_frames, &gun_asset, &have_shot);
			if (have_shot == false) {
				bullet_number += shoot_bullets(&gun_asset, bullet_number, &bullet_asset[bullet_number], &have_shot);
			}

			if (bullet_number > 99) {
				bullet_number = 1;
			}
			for (l = 0; l < 99; l++) {
				//		printf("Exitsts: %x %x\n", bullet_asset[0], gun_asset);
				if (bullet_asset[l].does_exist == true) {
					if (collision_detection_with_screen(&bullet_asset[l], &on_screen) == true) {
						draw_texture(&game_app, &bullet_asset[l]);
						bullet_asset[l].x += bullet_asset[l].speed * sin((double)gun_asset.angle * 3.14159 / 180);
						bullet_asset[l].y += bullet_asset[l].speed * -cos((double)gun_asset.angle * 3.14159 / 180);
					} else {
						bullet_asset->does_exist = false;
					}
				}
			}
		}

		sprintf(alien_count.text, "Aliens: %d", aliens_alive);
		if (alien_spawn < 2) {
			sprintf(world_position.text, "Break the powerups and destroy the portal! Lshift to shoot, space to meele.");
			draw_text(&game_app, &world_position);
		}
		draw_text(&game_app, &alien_count);
		sprintf(health.text, "%d/100", player_asset.vitality);
		draw_text(&game_app, &health);
		draw_text(&game_app, &frames_text);
		draw_texture(&game_app, &crosshair_asset);

		// sprintf(debug_output.text, "X:%d Y:%d", user_input.mouse.x, user_input.mouse.y);
		// draw_text(&game_app, &debug_output);

		// ten second timekeeping
		if (frames_data.prev_time.tv_sec == ten_second_timekeeping.tv_sec + 10) {
			play_music(audio.music[track], -1);
			track++;
			if (track > 9) {
				track = 0;
			}
			clock_gettime(CLOCK_MONOTONIC, &ten_second_timekeeping);

			if (portal_asset.does_exist == true) {
				alien_spawn += 10;
				for (a = aliens_alive; a < alien_spawn + aliens_alive; a++) {
					alien_asset[a].does_exist = true;
					alien_asset[a].world_x = portal_asset.world_x + portal_asset.width / 2;
					alien_asset[a].world_y = portal_asset.world_y + portal_asset.height / 2;
				}
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
		if (player_asset.vitality <= 0) {
			draw_text(&game_app, &game_over);
			player_asset.speed = 0;
			//	break;
		} else if (portal_asset.does_exist == false) {
			sprintf(game_over.text, "You Win!");
			draw_text(&game_app, &game_over);
		}
		present_screen(&game_app);

		sprintf(frames_text.text, "%d FPS", frames_data.current_framerate);

		calculate_framerate(&frames_data);
		frame_rate_cap(&frames_data);

		if (powerup[0].does_exist == false && powerup[1].does_exist == false) {
			player_asset.vitality = 10000;
			for (j = 0; j < 100; j++) {
				bullet_asset[j].vitality = 1000;
			}
			sword_asset.vitality = 1000;
		}

		// clock_gettime(CLOCK_MONOTONIC, &tend);
		//  printf("some_long_computation took about %.5f seconds\n", ((double)tend.tv_sec + 1.0e-9 * tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9 * tstart.tv_nsec));
	}

	return 0;
}