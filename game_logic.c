/**
 * @file game_logic.c
 * @author Thomas Bujaki
 */

#include "game_logic.h"

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
	if (user_input->mouse.x < asset->x + asset->width &&
		user_input->mouse.x > asset->x &&
		user_input->mouse.y < asset->y + asset->width &&
		user_input->mouse.y > asset->y) {
		return true;
	}
	return false;
}