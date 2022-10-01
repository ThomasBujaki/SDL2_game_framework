#include </opt/homebrew/include/SDL2/SDL_render.h>

#include "game_logic.h"

struct world_assets {
	SDL_Texture *grass_tile;
};

struct world_state {
	struct position player_position;
};

void update_player_location_user_input(struct world_state *state, struct events_data *user_input, int magnitude);
void draw_world(struct top_level_window *game_app, struct world_state *state, struct world_assets *assets);