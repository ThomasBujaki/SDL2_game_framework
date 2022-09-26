/**
 * @file asset_management.c
 * @author Thomas Bujaki
 * @brief
 */

#include "asset_management.h"

void init_asset_dimensions(struct asset_information *asset, int x, int y, int w, int h) {
	asset->asset_x = x;
	asset->asset_y = y;
	asset->asset_width = w;
	asset->asset_height = h;
}

void init_text_information(struct text_information *text_data, char text_string[256], SDL_Colour colour, int font, int x, int y, int w, int h) {
	strncpy(text_data->text, text_string, 255);
	text_data->text_colour = colour;
	text_data->text_font_size = font;
	text_data->text_x = x;
	text_data->text_y = y;
	text_data->text_width = w;
	text_data->text_height = h;
}

SDL_Texture *load_asset(struct top_level_window *game_app, char file_path[256]) {
	SDL_Texture *texture;
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", file_path);
	texture = IMG_LoadTexture(game_app->renderer, file_path);
	return texture;
}

void update_asset_size(struct asset_information *asset, struct events_data *user_input) {
	if (user_input->mouse_scrolled == true) {
		if ((asset->asset_height > 5 && asset->asset_width > 5) || (asset->asset_height < 500 && asset->asset_width < 500)) {
			asset->asset_width -= user_input->mouse_scrolled_magnitude;
			asset->asset_height -= user_input->mouse_scrolled_magnitude;
		}
		user_input->mouse_scrolled = false;
	}
}

void set_asset_position(struct asset_information *asset, int x, int y) {
	asset->asset_x = x;
	asset->asset_y = y;
}

void change_asset_position(struct asset_information *asset, int delta_x, int delta_y) {
	asset->asset_x += delta_x;
	asset->asset_y += delta_y;
}

void update_asset_location_user_input(struct asset_information *asset, struct events_data *user_input, int magnitude) {
	if (user_input->keyboard_events[up_key] == true) {
		asset->asset_y -= magnitude;
	}
	if (user_input->keyboard_events[down_key] == true) {
		asset->asset_y += magnitude;
	}
	if (user_input->keyboard_events[left_key] == true) {
		asset->asset_x -= magnitude;
	}
	if (user_input->keyboard_events[right_key] == true) {
		asset->asset_x += magnitude;
	}
}

void draw_texture(struct top_level_window *game_app, struct asset_information *asset) {
	SDL_Rect asset_dimensions;

	asset_dimensions.x = asset->asset_x;
	asset_dimensions.y = asset->asset_y;

	// SDL_QueryTexture(asset->asset_texture, NULL, NULL, &asset_dimensions.w, &asset_dimensions.h);
	asset_dimensions.w = asset->asset_width;
	asset_dimensions.h = asset->asset_height;
	// todo: replace with SDL_RenderCopyEx for rotation and extra features
	SDL_RenderCopy(game_app->renderer, asset->asset_texture, NULL, &asset_dimensions);
}

void draw_text(struct top_level_window *game_app, struct text_information *display_text) {
	TTF_Font *times_font = TTF_OpenFont("times.ttf", display_text->text_font_size);

	SDL_Surface *surfaceMessage = TTF_RenderText_Solid(times_font, display_text->text, display_text->text_colour);

	SDL_Texture *Message = SDL_CreateTextureFromSurface(game_app->renderer, surfaceMessage);

	SDL_Rect Message_rect;
	Message_rect.x = display_text->text_x;
	Message_rect.y = display_text->text_y;
	Message_rect.w = display_text->text_width;
	Message_rect.h = display_text->text_height;

	SDL_RenderCopy(game_app->renderer, Message, NULL, &Message_rect);

	TTF_CloseFont(times_font);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}