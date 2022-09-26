/**
 * @file asset_management.h
 * @author Thomas Bujaki
 * @brief
 */

#include </opt/homebrew/include/SDL2/SDL_log.h>	 // could remove /opt/homebrew/include/SDL2/

#include "audio_management.h"
#include "graphics_management.h"
#include "input_management.h"

struct asset_information {
	int asset_x;
	int asset_y;
	int asset_width;
	int asset_height;
	SDL_Texture *asset_texture;
};

struct text_information {
	int text_x;
	int text_y;
	int text_width;
	int text_height;
	int text_font_size;
	SDL_Colour text_colour;
	char text[256];
};

void init_asset_dimensions(struct asset_information *asset, int x, int y, int w, int h);
void init_text_information(struct text_information *text_data, char text_string[256], SDL_Colour colour, int font, int x, int y, int w, int h);
SDL_Texture *load_asset(struct top_level_window *game_app, char file_path[256]);
void update_asset_size(struct asset_information *asset, struct events_data *user_input);
void set_asset_position(struct asset_information *asset, int x, int y);
void change_asset_position(struct asset_information *asset, int delta_x, int delta_y);
void update_asset_location_user_input(struct asset_information *asset, struct events_data *user_input, int magnitude);
void draw_texture(struct top_level_window *game_app, struct asset_information *asset);
void draw_text(struct top_level_window *game_app, struct text_information *display_text);