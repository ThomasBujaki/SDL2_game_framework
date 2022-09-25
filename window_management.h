/**
 * @file window_management.h
 * @author Thomas Bujaki
 * @brief
 */

#ifndef included
#include "common_include.h"
#endif

#include </opt/homebrew/include/SDL2/SDL_image.h>
#include </opt/homebrew/include/SDL2/SDL_mixer.h>
#include </opt/homebrew/include/SDL2/SDL_ttf.h>

struct top_level_window {
	SDL_Renderer *renderer;
	SDL_Window *window;
	int window_height;
	int window_width;
};

struct top_level_window *top_level_window_ptr;

void init_window(struct top_level_window *game_app);
void cleanup();
