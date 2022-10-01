/**
 * @file game_logic.h
 * @author Thomas Bujaki
 */

#include "asset_management.h"

bool collision_detection(struct asset_information *obj_1, struct asset_information *obj_2);
bool is_moused_over(struct asset_information *asset, struct events_data *user_input);