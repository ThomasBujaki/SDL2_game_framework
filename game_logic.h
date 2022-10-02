/**
 * @file game_logic.h
 * @author Thomas Bujaki
 */

#include "asset_management.h"

bool collision_detection_with_screen(struct asset_information *obj_1, struct asset_information *obj_2);
bool collision_detection(struct asset_information *obj_1, struct asset_information *obj_2);
bool is_moused_over(struct asset_information *asset, struct events_data *user_input);
void move_aliens(int asset_num, struct asset_information *all_assets[asset_num], struct asset_information *asset, struct asset_information *player_asset);