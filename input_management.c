/**
 * @file input_management.c
 * @author Thomas Bujaki
 * @brief
 */

#include "input_management.h"

void process_key(SDL_KeyboardEvent *event, int event_type, struct events_data *user_input) {
	// event type 1 is pressed, 2 is lifted
	if (event_type == 1) {
		switch (event->keysym.scancode) {
			case 82:  // up
				user_input->keyboard_events[up_key] = true;
				break;
			case 81:  // down
				user_input->keyboard_events[down_key] = true;
				break;
			case 80:  // left
				user_input->keyboard_events[left_key] = true;
				break;
			case 79:  // right
				user_input->keyboard_events[right_key] = true;
				break;
			default:
				break;
		}
	} else if (event_type == 2) {
		switch (event->keysym.scancode) {
			case 82:  // up
				user_input->keyboard_events[up_key] = false;
				break;
			case 81:  // down
				user_input->keyboard_events[down_key] = false;
				break;
			case 80:  // left
				user_input->keyboard_events[left_key] = false;
				break;
			case 79:  // right
				user_input->keyboard_events[right_key] = false;
				break;
			default:
				break;
		}
	}
}

void process_controller_joystick(SDL_ControllerAxisEvent *joy_stick_events, struct events_data *user_input) {
	printf("axis: %d\n", joy_stick_events->axis);
	printf("type: %d\n", joy_stick_events->type);
	printf("value: %d\n", joy_stick_events->value);
	printf("which: %d\n", joy_stick_events->which);
	printf("timestamp: %d\n", joy_stick_events->timestamp);
}

void process_controller_buttons() {
}

void process_mouse_click(struct events_data *user_input) {
	printf("Mouse Clicked\n");
	user_input->mouse_clicked = true;
}

void process_mousewheel(SDL_MouseWheelEvent *wheel, struct events_data *user_input) {
	printf("wheel scrolled: %d %d\n", wheel->x, wheel->y);

	user_input->mouse_scrolled = true;
	user_input->mouse_scrolled_magnitude = wheel->x + wheel->y;
}

void process_gamepad_inout() {
}

void process_input(struct events_data *user_input) {
	SDL_Event event;
	//	SDL_GameController *T;	// todo clean up this controller stuff
	//	T = SDL_GameControllerOpen(0);
	//	SDL_GameControllerAxis axis;
	//	SDL_GameControllerButtonBind A = SDL_GameControllerGetBindForAxis(T, axis);
	//	printf("axis? %d\n", A.value.axis);
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_KEYDOWN:
				process_key(&event.key, 1, user_input);
				break;
			case SDL_KEYUP:
				process_key(&event.key, 2, user_input);
				break;
			case SDL_MOUSEBUTTONDOWN:
				process_mouse_click(user_input);
				break;
			case SDL_MOUSEWHEEL:
				process_mousewheel(&event.wheel, user_input);
				break;
				// todo: below doesn't work
			case SDL_CONTROLLERBUTTONDOWN:
				// process_controller(&event.cbutton, user_input);
				//	printf("HELLO\n");
				break;
			case SDL_CONTROLLERAXISMOTION:
				// printf("JOY\n");
				//	process_controller_joystick(&event.caxis, user_input);
				break;
			default:
				break;
		}
	}
	//	SDL_GameControllerClose(T);
}