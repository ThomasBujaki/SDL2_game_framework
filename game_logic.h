/**
 * @file game_logic.h
 * @author Thomas Bujaki
 */

#include "asset_management.h"

bool collision_detection_with_screen(struct asset_information *obj_1, struct asset_information *obj_2);
bool collision_detection(struct asset_information *obj_1, struct asset_information *obj_2);
bool is_moused_over(struct asset_information *asset, struct events_data *user_input);
void move_aliens(int asset_num, struct asset_information *all_assets[asset_num], struct asset_information *asset, struct asset_information *player_asset);
int do_attack_animation(struct top_level_window *game_app, struct asset_information *player_asset, struct events_data *user_input, struct asset_information *sword_asset, int num_frames);
void change_angle_to_mouse_dir(struct asset_information *asset, struct events_data *user_input, struct asset_information *crosshair);
bool did_player_kill_alien(struct asset_information *asset, struct asset_information *sword_asset);
int do_gun_animation(struct top_level_window *game_app, struct asset_information *player_asset, struct events_data *user_input, int num_frames, struct asset_information *gun_asset, bool *have_shot);
int shoot_bullets(struct asset_information *gun_asset, int bullet_number, struct asset_information *bullet_asset, bool *have_shot);
int did_bullet_kill_enemy(struct asset_information *asset, struct asset_information *bullet_asset, struct asset_information *gun_asset);