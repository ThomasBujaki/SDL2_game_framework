/**
 * @file input_management.h
 * @author your name (you@domain.com)
 * @brief
 */

#include <stdbool.h>
#include <stdio.h>

enum keyboard_inputs { up_key = 0,
					   down_key = 1,
					   left_key = 2,
					   right_key = 3 };

struct events_data {
	bool keyboard_events[4];  // up, down, left, right
	bool mouse_clicked;
	bool mouse_scrolled;
	int mouse_scrolled_magnitude;
};

struct mouse_event_data {
	int x;
	int y;
};

void process_input(struct events_data *user_input);