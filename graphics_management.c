/**
 * @file graphics_management.c
 * @author Thomas Bujaki
 * @brief
 */

#include "graphics_management.h"

void prep_screen(struct top_level_window *game_app) {
	SDL_SetRenderDrawColor(game_app->renderer, 255, 255, 255, 255);
	SDL_RenderClear(game_app->renderer);
}

void present_screen(struct top_level_window *game_app) {
	SDL_RenderPresent(game_app->renderer);
}