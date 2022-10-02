/**
 * @file asset_management.c
 * @author Thomas Bujaki
 * @brief
 */

#include "asset_management.h"

void init_asset_dimensions(struct asset_information *asset, int x, int y, int w_x, int w_y, int w, int h, int a, char str[64], bool asset_in_world, int speed, int vit) {
	asset->x = x;
	asset->y = y;
	asset->world_x = w_x;
	asset->world_y = w_y;
	asset->width = w;
	asset->height = h;
	asset->angle = a;
	strncpy(asset->description, str, 31);
	asset->does_exist = asset_in_world;
	asset->speed = speed;
	asset->vitality = vit;
}

void init_text_information(struct text_information *text_data, char filepath[256], char text_string[256], SDL_Colour colour, int font, int x, int y, int w, int h) {
	//	TTF_Font *times_font = TTF_OpenFont("times.ttf", display_text->text_font_size);
	strncpy(text_data->text, text_string, 255);
	text_data->text_colour = colour;
	text_data->text_font_size = font;
	text_data->font = TTF_OpenFont(filepath, text_data->text_font_size);
	text_data->text_x = x;
	text_data->text_y = y;
	text_data->text_width = w;
	text_data->text_height = h;
}

SDL_Texture *load_texture(struct top_level_window *game_app, char file_path[256]) {
	SDL_Texture *texture;
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", file_path);
	texture = IMG_LoadTexture(game_app->renderer, file_path);
	return texture;
}

void update_asset_size(struct asset_information *asset, struct events_data *user_input) {
	if (user_input->mouse_scrolled == true) {
		if ((asset->height > 5 && asset->width > 5) || (asset->height < 500 && asset->width < 500)) {
			asset->width -= user_input->mouse_scrolled_magnitude;
			asset->height -= user_input->mouse_scrolled_magnitude;
		}
		user_input->mouse_scrolled = false;
	}
}

void set_asset_position(struct asset_information *asset, int x, int y) {
	asset->x = x;
	asset->y = y;
}

void change_asset_world_position(struct asset_information *asset, int delta_x, int delta_y) {
	asset->world_x += delta_x;
	asset->world_y += delta_y;
}

void change_asset_position(struct asset_information *asset, int delta_x, int delta_y) {
	asset->world_x += delta_x;
	asset->world_y += delta_y;
}

void draw_texture(struct top_level_window *game_app, struct asset_information *asset) {
	SDL_Rect asset_dimensions;

	asset_dimensions.x = asset->x;
	asset_dimensions.y = asset->y;

	// SDL_QueryTexture(asset->texture, NULL, NULL, &asset_dimensions.w, &asset_dimensions.h);
	asset_dimensions.w = asset->width;
	asset_dimensions.h = asset->height;
	asset->is_drawn = true;
	// todo: replace with SDL_RenderCopyEx for rotation and extra features
	SDL_RenderCopyEx(game_app->renderer, asset->texture, NULL, &asset_dimensions, asset->angle, NULL, SDL_FLIP_NONE);
}

void draw_text(struct top_level_window *game_app, struct text_information *display_text) {
	SDL_Surface *surfaceMessage = TTF_RenderText_Solid(display_text->font, display_text->text, display_text->text_colour);

	SDL_Texture *Message = SDL_CreateTextureFromSurface(game_app->renderer, surfaceMessage);

	SDL_Rect Message_rect;
	Message_rect.x = display_text->text_x;
	Message_rect.y = display_text->text_y;
	Message_rect.w = display_text->text_width;
	Message_rect.h = display_text->text_height;

	SDL_RenderCopy(game_app->renderer, Message, NULL, &Message_rect);

	//	TTF_CloseFont(times_font);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}