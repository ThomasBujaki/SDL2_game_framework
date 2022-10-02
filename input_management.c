/**
 * @file input_management.c
 * @author Thomas Bujaki
 * @brief
 */

#include "input_management.h"

void process_key(SDL_KeyboardEvent *event, int event_type, struct events_data *user_input) {
	printf("%d\n", event->keysym.scancode);
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
			case 44:
				user_input->keyboard_events[space_key] = true;
				break;
			case 225:
				user_input->keyboard_events[shift_key] = true;
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

void process_controller_button(SDL_ControllerButtonEvent *controller_button, struct events_data *user_input) {
}

void process_controller_joystick(SDL_ControllerAxisEvent *controller_axis, struct events_data *user_input) {
	// printf("value: %d\n", controller_axis->value);
	if (controller_axis->axis == SDL_CONTROLLER_AXIS_LEFTX) {
		if (controller_axis->value < -8000) {
			user_input->keyboard_events[left_key] = true;
		} else if (controller_axis->value > 8000) {
			user_input->keyboard_events[right_key] = true;
		} else {
			user_input->keyboard_events[left_key] = false;
			user_input->keyboard_events[right_key] = false;
		}
	} else if (controller_axis->axis == SDL_CONTROLLER_AXIS_LEFTY) {
		if (controller_axis->value < -8000) {
			user_input->keyboard_events[up_key] = true;
		} else if (controller_axis->value > 8000) {
			user_input->keyboard_events[down_key] = true;
		} else {
			user_input->keyboard_events[up_key] = false;
			user_input->keyboard_events[down_key] = false;
		}
	} else if (controller_axis->axis == SDL_CONTROLLER_AXIS_RIGHTX) {
		if (controller_axis->value < -8000) {
			user_input->angle_change = -1;
		} else if (controller_axis->value > 8000) {
			user_input->angle_change = 1;
		} else {
			user_input->angle_change = 0;
		}
	} else if (controller_axis->axis == SDL_CONTROLLER_AXIS_RIGHTY) {
		if (controller_axis->value < -8000) {
			user_input->mouse_scrolled = true;
			user_input->mouse_scrolled_magnitude = -20;
		} else if (controller_axis->value > 8000) {
			user_input->mouse_scrolled = true;
			user_input->mouse_scrolled_magnitude = 20;
		} else {
			user_input->mouse_scrolled = true;
			user_input->mouse_scrolled_magnitude = 0;
		}
	}
}

void process_controller_buttons() {
}

void process_mouse_click(struct events_data *user_input) {
	printf("Mouse Clicked\n");
	user_input->mouse_clicked = true;
}

void process_mousewheel(SDL_MouseWheelEvent *wheel, struct events_data *user_input) {
	user_input->mouse_scrolled = true;
	user_input->mouse_scrolled_magnitude = wheel->x + wheel->y;
}

void process_gamepad_inout() {
}

void process_input(struct events_data *user_input) {
	SDL_Event event;
	SDL_GameController *controller = SDL_GameControllerOpen(0);
	int i;
	for (i = 0; i < SDL_NumJoysticks(); i++) {
		if (SDL_IsGameController(i)) {
			controller = SDL_GameControllerOpen(i);
		}
	}
	SDL_GetMouseState(&user_input->mouse.x, &user_input->mouse.y);

	// printf("Mapping: %s\n", SDL_GameControllerMapping(controller));

	//	SDL_GameControllerAxis axis;
	//	SDL_GameControllerButtonBind A = SDL_GameControllerGetBindForAxis(T, axis);
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
				// then check for specific buttons
				// process_controller(&event.cbutton, user_input);
				//	printf("HELLO\n");
				break;
			case SDL_CONTROLLERAXISMOTION:
				process_controller_joystick(&event.caxis, user_input);
				break;
			default:
				break;
		}
	}
	SDL_GameControllerClose(controller);
}