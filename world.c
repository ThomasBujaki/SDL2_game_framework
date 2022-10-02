#include "world.h"

// todo change this to move the world
// TODO: This should really be in a different file purely for world state
void update_player_location_user_input(struct world_state *state, struct events_data *user_input, struct asset_information *player_asset) {
	if (user_input->keyboard_events[up_key] == true) {
		state->player_position.y -= player_asset->speed;
		state->world_offset_y = player_asset->speed;
	}
	if (user_input->keyboard_events[down_key] == true) {
		state->player_position.y += player_asset->speed;
		state->world_offset_y = -player_asset->speed;
	}
	if (user_input->keyboard_events[up_key] == false && user_input->keyboard_events[down_key] == false) {
		state->world_offset_y = 0;
	}
	if (user_input->keyboard_events[left_key] == true) {
		state->player_position.x -= player_asset->speed;
		state->world_offset_x = player_asset->speed;
	}
	if (user_input->keyboard_events[right_key] == true) {
		state->player_position.x += player_asset->speed;
		state->world_offset_x = -player_asset->speed;
	}
	if (user_input->keyboard_events[left_key] == false && user_input->keyboard_events[right_key] == false) {
		state->world_offset_x = 0;
	}
}

void draw_tiles(struct top_level_window *game_app, struct world_state *state, struct world_assets *assets) {
	// TODO: Load static world description from a file
	// TODO: Performance: We don't actually need to render the ENTIRE world of tiles, we just need to render the tiles that appear within the "viewport"
	int world_width = 100;
	int world_height = 100;
	for (int x = -world_width; x < world_width; x++) {
		for (int y = -world_height; y < world_height; y++) {
			SDL_Rect tile_rectangle;
			// need to make it so the ground doesn't move
			tile_rectangle.x = x * 100 - (state->player_position.x) + state->world_offset_x;
			tile_rectangle.y = y * 100 - (state->player_position.y) + state->world_offset_y;
			//	printf("%d %d\n", tile_rectangle.x, tile_rectangle.y);
			tile_rectangle.w = 100;
			tile_rectangle.h = 100;
			SDL_RenderCopyEx(game_app->renderer, assets->grass_tile, NULL, &tile_rectangle, 0, NULL, SDL_FLIP_NONE);
		}
	}
}

void draw_world(struct top_level_window *game_app, struct world_state *state, struct world_assets *assets) {
	draw_tiles(game_app, state, assets);
}
