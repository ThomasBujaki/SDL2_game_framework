/**
 * @file asset_management.h
 * @author Thomas Bujaki
 * @brief
 */

#include </opt/homebrew/include/SDL2/SDL_log.h>	 // could remove /opt/homebrew/include/SDL2/

#include "audio_management.h"
#include "graphics_management.h"
#include "input_management.h"

struct position {
	int x;
	int y;
};

struct asset_information {
	int x;
	int y;
	int width;
	int height;
	int angle;
	SDL_Texture *texture;
};

struct text_information {
	TTF_Font *font;
	int text_x;
	int text_y;
	int text_width;
	int text_height;
	int text_font_size;
	SDL_Colour text_colour;
	char text[256];
};

void init_asset_dimensions(struct asset_information *asset, int x, int y, int w, int h, int a);
void init_text_information(struct text_information *text_data, char filepath[256], char text_string[256], SDL_Colour colour, int font, int x, int y, int w, int h);
SDL_Texture *load_texture(struct top_level_window *game_app, char file_path[256]);
void update_asset_size(struct asset_information *asset, struct events_data *user_input);
void change_angle(struct asset_information *asset, struct events_data *user_input, int magnitude);
void set_asset_position(struct asset_information *asset, int x, int y);
void change_asset_position(struct asset_information *asset, int delta_x, int delta_y);
void draw_texture(struct top_level_window *game_app, struct asset_information *asset);
void draw_text(struct top_level_window *game_app, struct text_information *display_text);