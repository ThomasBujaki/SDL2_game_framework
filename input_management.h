/**
 * @file input_management.h
 * @author your name (you@domain.com)
 * @brief
 */
#include </opt/homebrew/include/SDL2/SDL_events.h>
#include </opt/homebrew/include/SDL2/SDL_gamecontroller.h>
#include <stdbool.h>
#include <stdio.h>

enum keyboard_inputs { up_key = 0,
					   down_key = 1,
					   left_key = 2,
					   right_key = 3,
					   space_key = 4 };

enum controller_inputs { a_button = 0,
						 b_button = 1,
						 x_button = 2,
						 y_button = 3 };

struct mouse_event_data {
	int x;
	int y;
};

// todo change these to use the SDL built in structs for these events
struct events_data {
	bool keyboard_events[5];  // up, down, left, right, space
	bool mouse_clicked;
	bool mouse_scrolled;
	int mouse_scrolled_magnitude;
	bool controller_events[4];	// a, b, x, y
	int angle_change;
	SDL_JoyAxisEvent joy_stick_events;
	struct mouse_event_data mouse;
};

void process_input(struct events_data *user_input);