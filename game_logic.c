/**
 * @file game_logic.c
 * @author Thomas Bujaki
 */

#include "game_logic.h"

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
	if ((asset->world_x + asset->width / 2 < player_asset->x + player_asset->width / 2 + 1000 && asset->world_y + asset->height / 2 < player_asset->y + player_asset->height / 2 + 1000) ||	 //
		(asset->world_x + asset->width / 2 < player_asset->x + player_asset->width / 2 - 1000 && asset->world_y + asset->height / 2 < player_asset->y + player_asset->height / 2 + 1000) ||	 //
		(asset->world_x + asset->width / 2 < player_asset->x + player_asset->width / 2 + 1000 && asset->world_y + asset->height / 2 < player_asset->y + player_asset->height / 2 - 1000) ||	 //
		(asset->world_x + asset->width / 2 < player_asset->x + player_asset->width / 2 - 1000 && asset->world_y + asset->height / 2 < player_asset->y + player_asset->height / 2 - 1000)) {
		return true;
	}
	return false;
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

void move_aliens(int asset_num, struct asset_information *all_assets[asset_num], struct asset_information *asset, struct asset_information *player_asset) {
	// move in random direction unless alien is in vision of player

	struct alien_motion x_y_to_move = {};
	if (currently_colliding(asset_num, all_assets, asset, &x_y_to_move)) {
		// printf("%d %d\n", x_y_to_move.x, x_y_to_move.y);
		// change_asset_position(asset, x_y_to_move.x, x_y_to_move.y);
	} else {
		if (is_alien_in_vision_of_player(asset, player_asset) == true) {
			which_way_is_player(asset, player_asset, &x_y_to_move);
		}
		if (will_collide(asset_num, all_assets, asset, &x_y_to_move)) {
			x_y_to_move.x = 0;
			x_y_to_move.y = 0;
		}
	}
	change_asset_position(asset, x_y_to_move.x, x_y_to_move.y);

	//	int i;
	// for
	//	change_asset_position(asset, -2, 0);
}