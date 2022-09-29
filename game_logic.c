/**
 * @file game_logic.c
 * @author Thomas Bujaki
 */

#include "game_logic.h"

bool collision_detection(struct asset_information *obj_1, struct asset_information *obj_2) {
	if (obj_1->asset_x < obj_2->asset_x + obj_2->asset_width &&
		obj_1->asset_x + obj_1->asset_width > obj_2->asset_x &&
		obj_1->asset_y < obj_2->asset_y + obj_2->asset_height &&
		obj_1->asset_height + obj_1->asset_y > obj_2->asset_y) {
		return true;
	}
	return false;
}