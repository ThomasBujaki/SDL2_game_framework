/**
 * @file window_management.c
 * @author Thomas Bujaki
 * @brief This is a file to manage all the aspects of working with a window
 */

#include "window_management.h"

/**
 * todo: setting up the window size?
 * todo: scaling window stuff
 */

void init_window(struct top_level_window *game_app) {
	int renderer_flags, window_flags;
	game_app->window_height = 720;
	game_app->window_width = 1280;

	renderer_flags = SDL_RENDERER_ACCELERATED;

	window_flags = 0;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
		printf("Error: initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024) == -1) {	// my mic is broken on one channel so I need to do mono
		printf("Error: SDL Mixer\n");
		exit(1);
	}
	Mix_AllocateChannels(8);

	game_app->window = SDL_CreateWindow("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, game_app->window_width, game_app->window_height, window_flags);

	if (!game_app->window) {
		printf("Failed to open %d x %d window: %s\n", game_app->window_width, game_app->window_height, SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	renderer_flags = SDL_RENDERER_ACCELERATED;
	game_app->renderer = SDL_CreateRenderer(game_app->window, -1, renderer_flags);

	if (!game_app->renderer) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	TTF_Init();
}

// todo issue with passing variable, do I need to destroyrenderer and destroywindow here for the
void cleanup() {
	SDL_DestroyRenderer(top_level_window_ptr->renderer);

	SDL_DestroyWindow(top_level_window_ptr->window);

	SDL_Quit();
}