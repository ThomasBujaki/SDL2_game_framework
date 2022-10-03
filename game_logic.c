/**
 * @file game_logic.c
 * @author Thomas Bujaki
 */

#include "game_logic.h"

#define PI 3.14159265358979323846

struct alien_motion {
	int x;
	int y;
};

bool collision_detection_with_screen(struct asset_information *obj_1, struct asset_information *obj_2) {
	if (obj_1->world_x < obj_2->x + obj_2->width &&
		obj_1->world_x + obj_1->width > obj_2->x &&
		obj_1->world_y < obj_2->y + obj_2->height &&
		obj_1->height + obj_1->world_y > obj_2->y) {
		return true;
	}
	return false;
}

bool collision_detection(struct asset_information *obj_1, struct asset_information *obj_2) {
	if (obj_1->x < obj_2->x + obj_2->width &&
		obj_1->x + obj_1->width > obj_2->x &&
		obj_1->y < obj_2->y + obj_2->height &&
		obj_1->height + obj_1->y > obj_2->y) {
		return true;
	}
	return false;
}

bool is_moused_over(struct asset_information *asset, struct events_data *user_input) {
	// todo figure out why assets are off center by 20 pixels
	if (user_input->mouse.x < asset->x + asset->width &&
		user_input->mouse.x > asset->x &&
		user_input->mouse.y < asset->y + asset->height &&
		user_input->mouse.y > asset->y) {
		return true;
	}
	return false;
}

int determine_alien_movement() {
	// check which directions are ok
	// randomly select from the options
	return 0;
}

bool is_alien_in_vision_of_player(struct asset_information *asset, struct asset_information *player_asset) {
	// if ((asset->world_x + asset->width / 2 < player_asset->x + player_asset->width / 2 + 1000 && asset->world_y + asset->height / 2 < player_asset->y + player_asset->height / 2 + 1000) ||	 //
	//	(asset->world_x + asset->width / 2 < player_asset->x + player_asset->width / 2 - 1000 && asset->world_y + asset->height / 2 < player_asset->y + player_asset->height / 2 + 1000) ||	 //
	//	(asset->world_x + asset->width / 2 < player_asset->x + player_asset->width / 2 + 1000 && asset->world_y + asset->height / 2 < player_asset->y + player_asset->height / 2 - 1000) ||	 //
	//	(asset->world_x + asset->width / 2 < player_asset->x + player_asset->width / 2 - 1000 && asset->world_y + asset->height / 2 < player_asset->y + player_asset->height / 2 - 1000)) {
	return true;
	//}
	// return false;
}

void which_way_is_player(struct asset_information *asset, struct asset_information *player_asset, struct alien_motion *x_y_to_move) {
	if ((player_asset->x + player_asset->width / 2) - (asset->world_x + asset->width / 2) <= asset->speed && (player_asset->x + player_asset->width / 2) - (asset->world_x + asset->width / 2) >= -asset->speed) {
		x_y_to_move->x = 0;
	} else {
		if ((player_asset->x + player_asset->width / 2) > (asset->world_x + asset->width / 2)) {
			x_y_to_move->x = asset->speed;
		} else {
			x_y_to_move->x = -asset->speed;
		}
	}
	if ((player_asset->y + player_asset->height / 2) - (asset->world_y + asset->height / 2) <= asset->speed && (player_asset->y + player_asset->height / 2) - (asset->world_y + asset->height / 2) >= -asset->speed) {
		x_y_to_move->y = 0;
	} else {
		if (player_asset->y + player_asset->height / 2 > asset->world_y + asset->height / 2) {
			x_y_to_move->y = asset->speed;
		} else {
			x_y_to_move->y = -asset->speed;
		}
	}
}

void move_away_from_colliding(struct asset_information *asset, struct asset_information *player_asset, struct alien_motion *x_y_to_move) {
	if (player_asset->world_x - asset->world_x <= asset->speed && player_asset->world_y - asset->world_y <= asset->speed) {
		player_asset->world_x += (rand() % 3) - 1;
		player_asset->world_y += (rand() % 3) - 1;
		asset->world_x += (rand() % 3) - 1;
		asset->world_y += (rand() % 3) - 1;
	}
	if ((player_asset->world_x + player_asset->width / 2) - (asset->world_x + asset->width / 2) <= asset->speed && (player_asset->world_x + player_asset->width / 2) - (asset->world_x + asset->width / 2) >= -asset->speed) {
		x_y_to_move->x = 0;
	} else {
		if ((player_asset->world_x + player_asset->width / 2) > (asset->world_x + asset->width / 2)) {
			x_y_to_move->x = -asset->speed;
		} else {
			x_y_to_move->x = asset->speed;
		}
	}
	if ((player_asset->world_y + player_asset->height / 2) - (asset->world_y + asset->height / 2) <= asset->speed && (player_asset->world_y + player_asset->height / 2) - (asset->world_y + asset->height / 2) >= -asset->speed) {
		x_y_to_move->y = 0;
	} else {
		if (player_asset->world_y + player_asset->height / 2 > asset->world_y + asset->height / 2) {
			x_y_to_move->y = -asset->speed;
		} else {
			x_y_to_move->y = asset->speed;
		}
	}
}

bool currently_colliding(int asset_num, struct asset_information *all_assets[asset_num], struct asset_information *moving_asset, struct alien_motion *x_y_to_move) {
	int i;
	for (i = 0; i < asset_num; i++) {
		if (all_assets[i]->does_exist == true) {
			if (all_assets[i] != moving_asset) {
				if (moving_asset->world_x < all_assets[i]->x + all_assets[i]->width &&
					moving_asset->world_x + moving_asset->width > all_assets[i]->x &&
					moving_asset->world_y < all_assets[i]->y + all_assets[i]->height &&
					moving_asset->height + moving_asset->world_y > all_assets[i]->y) {
					move_away_from_colliding(moving_asset, all_assets[i], x_y_to_move);

					return true;
				}
			}
		}
	}
	return false;
}

// could be optimized to run elsewhere so I don't have to iteratre through asset_num assets again
bool will_collide(int asset_num, struct asset_information *all_assets[asset_num], struct asset_information *moving_asset, struct alien_motion *x_y_to_move) {
	int i;
	for (i = 0; i < asset_num; i++) {
		if (all_assets[i]->does_exist == true) {
			if (all_assets[i] != moving_asset) {
				if (moving_asset->world_x + x_y_to_move->x < all_assets[i]->x + all_assets[i]->width &&
					moving_asset->world_x + x_y_to_move->x + moving_asset->width > all_assets[i]->x &&
					moving_asset->world_y + x_y_to_move->y < all_assets[i]->y + all_assets[i]->height &&
					moving_asset->height + moving_asset->world_y + x_y_to_move->y > all_assets[i]->y) {
					return true;
				}
			}
		}
	}
	return false;
}

bool will_collide_with_player(struct asset_information *player_asset, struct asset_information *moving_asset, struct alien_motion *x_y_to_move) {
	if (moving_asset->world_x + x_y_to_move->x < player_asset->x + player_asset->width &&
		moving_asset->world_x + x_y_to_move->x + moving_asset->width > player_asset->x &&
		moving_asset->world_y + x_y_to_move->y < player_asset->y + player_asset->height &&
		moving_asset->height + moving_asset->world_y + x_y_to_move->y > player_asset->y) {
		return true;
	}

	return false;
}

void move_aliens(int asset_num, struct asset_information *all_assets[asset_num], struct asset_information *asset, struct asset_information *player_asset) {
	// move in random direction unless alien is in vision of player

	struct alien_motion x_y_to_move = {};

	if (currently_colliding(asset_num, all_assets, asset, &x_y_to_move)) {
		// printf("%d %d\n", x_y_to_move.x, x_y_to_move.y);
		change_asset_world_position(asset, x_y_to_move.x, x_y_to_move.y);
	} else {
		if (is_alien_in_vision_of_player(asset, player_asset) == true) {
			which_way_is_player(asset, player_asset, &x_y_to_move);
		}

		if (will_collide_with_player(player_asset, asset, &x_y_to_move)) {
			player_asset->vitality -= asset->vitality;
		}

		if (will_collide(asset_num, all_assets, asset, &x_y_to_move)) {
			x_y_to_move.x = 0;
			x_y_to_move.y = 0;
		}
	}
	change_asset_world_position(asset, x_y_to_move.x, x_y_to_move.y);
}

float get_angle(int x1, int y1, int x2, int y2) {
	float angle = -90 + atan2(y1 - y2, x1 - x2) * (180 / PI);
	return angle >= 0 ? angle : 360 + angle;
}

int do_attack_animation(struct top_level_window *game_app, struct asset_information *player_asset, struct events_data *user_input, struct asset_information *sword_asset, int num_frames) {
	// determine which way the player is facing (where the mouse is pointed)
	if (user_input->keyboard_events[space_key] == true) {
		if (sword_asset->is_drawn == false) {
			sword_asset->is_drawn = true;
		}
	}
	if (num_frames > 45) {
		sword_asset->is_drawn = false;
		user_input->keyboard_events[space_key] = false;
		return 0;
	}
	if (sword_asset->is_drawn == true) {
		sword_asset->angle = (int)get_angle(player_asset->x, player_asset->y, user_input->mouse.x, user_input->mouse.y);

		sword_asset->y = player_asset->y + player_asset->height / 4 - sword_asset->height * cos((double)sword_asset->angle * PI / 180) - cos((double)sword_asset->angle * PI / 180) * (player_asset->height / 2);
		sword_asset->x = player_asset->x + player_asset->width / 2 - sword_asset->width / 2 + sin((double)sword_asset->angle * PI / 180) * (player_asset->width);

		draw_texture(game_app, sword_asset);
		return num_frames + 1;
	}

	return 0;
	// get the angle vs the player
	// draw the sword asset
}

void change_angle_to_mouse_dir(struct asset_information *asset, struct events_data *user_input, struct asset_information *crosshair) {
	float angle;
	angle = get_angle(asset->x, asset->y, user_input->mouse.x - crosshair->width / 2, user_input->mouse.y - crosshair->height / 2);
	asset->angle = angle;
}

bool did_player_kill_alien(struct asset_information *asset, struct asset_information *sword_asset) {
	if (sword_asset->is_drawn == true) {
		if (collision_detection(asset, sword_asset) == true) {
			if (strncmp("tree", asset->description, 4)) {
				asset->vitality -= sword_asset->vitality;
				if (asset->vitality <= 0) {
					asset->does_exist = false;
					return true;
				}
			}
		}
	}
	return false;
}

int do_gun_animation(struct top_level_window *game_app, struct asset_information *player_asset, struct events_data *user_input, int num_frames, struct asset_information *gun_asset, bool *have_shot) {
	if (user_input->keyboard_events[shift_key] == true) {
		if (gun_asset->is_drawn == false) {
			gun_asset->is_drawn = true;
		}
	}
	if (num_frames > 45) {
		gun_asset->is_drawn = false;
		user_input->keyboard_events[shift_key] = false;
		*have_shot = false;
		return 0;
	}
	if (gun_asset->is_drawn == true) {
		gun_asset->angle = (int)get_angle(player_asset->x, player_asset->y, user_input->mouse.x, user_input->mouse.y);

		gun_asset->y = player_asset->y + player_asset->height / 4 - gun_asset->height * cos((double)gun_asset->angle * PI / 180) - cos((double)gun_asset->angle * PI / 180) * (player_asset->height / 2);
		gun_asset->x = player_asset->x + player_asset->width / 2 - gun_asset->width / 2 + sin((double)gun_asset->angle * PI / 180) * (player_asset->width);

		draw_texture(game_app, gun_asset);
		return num_frames + 1;
	}
	return 0;
}

int shoot_bullets(struct asset_information *gun_asset, int bullet_number, struct asset_information *bullet_asset, bool *have_shot) {
	bullet_asset->angle = gun_asset->angle;
	bullet_asset->x = gun_asset->x;
	bullet_asset->y = gun_asset->y;
	bullet_asset->does_exist = true;
	*have_shot = true;
	return 1;
}

int did_bullet_kill_enemy(struct asset_information *asset, struct asset_information *bullet_asset, struct asset_information *gun_asset) {
	if (bullet_asset->does_exist == true) {
		if (collision_detection(asset, bullet_asset)) {
			asset->vitality -= bullet_asset->vitality;
			bullet_asset->does_exist = false;
			bullet_asset->x = gun_asset->x;
			bullet_asset->y = gun_asset->y;
			if (asset->vitality <= 0) {
				asset->does_exist = false;
			}
		}
	}
	return 0;
}
