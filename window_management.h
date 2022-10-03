/**
 * @file window_management.h
 * @author Thomas Bujaki
 * @brief
 */

#ifndef included
#include "common_include.h"
#endif

#include <stdbool.h>

#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

struct top_level_window {
	SDL_Renderer *renderer;
	SDL_Window *window;
	int window_height;
	int window_width;
};

struct top_level_window *top_level_window_ptr;

void init_window(struct top_level_window *game_app);
void cleanup();
