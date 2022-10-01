/**
 * @file game_logic.c
 * @author Thomas Bujaki
 */

#include "game_logic.h"

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

void move_aliens(struct asset_information *asset) {
	change_asset_position(asset, -2, 0);
}