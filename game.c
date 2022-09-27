/**
 * @file game.c
 * @author Thomas Bujaki
 * @brief Main file for simple setup for game creation
 * @version 0.1
 * @date 2022-09-25
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asset_management.h"

/*todo implement collision detection
 */

struct argument_parameters {};

struct frame_time_info {
    struct timespec prev_time;
    double frame_rate_target;
    long ns_per_frame;
    int num_updates;
    int current_framerate;
};

void init_frame_rate_parameters(struct frame_time_info *frames_data) {
    frames_data->frame_rate_target = 60;
    frames_data->ns_per_frame = 1000000000 / frames_data->frame_rate_target;
}

void init_SDL_settings() {}

int init_arguments() { return 0; }

int process_arguments(int ac, char **av,
                      struct argument_parameters *arguments) {
    return 0;
}

long ONE_NANOSECOND = 1000000000;

long nano_difference(struct timespec a, struct timespec b) {
    return a.tv_nsec - b.tv_nsec + (a.tv_sec - b.tv_sec) * ONE_NANOSECOND;
}

struct timespec nano_add(struct timespec time, long nanos) {
    struct timespec new_time = {};
    new_time.tv_sec = time.tv_sec + (nanos / ONE_NANOSECOND);
    new_time.tv_nsec = time.tv_nsec + (nanos % ONE_NANOSECOND);
    if (new_time.tv_nsec >= ONE_NANOSECOND) {
        new_time.tv_nsec -= ONE_NANOSECOND;
        new_time.tv_sec++;
    }
    return new_time;
}

void calculate_framerate(struct frame_time_info *frames_data) {
    frames_data->num_updates++;
    struct timespec right_now = {};
    clock_gettime(CLOCK_MONOTONIC, &right_now);

    if (nano_difference(right_now, frames_data->prev_time) >= 1000000000) {
        clock_gettime(CLOCK_MONOTONIC, &frames_data->prev_time);
        frames_data->current_framerate = frames_data->num_updates;
        frames_data->num_updates = 0;
    }
}

// todo: change all timekeeping to nanoseconds.
void frame_rate_cap(struct frame_time_info *frames_data) {
    struct timespec wait = {};
    struct timespec right_now = {};

    struct timespec expected_frame_end_time =
        nano_add(frames_data->prev_time,
                 frames_data->num_updates * frames_data->ns_per_frame);
    clock_gettime(CLOCK_MONOTONIC, &right_now);
    wait.tv_nsec = nano_difference(expected_frame_end_time, right_now);

    if (wait.tv_nsec > 0) {
        nanosleep(&wait, NULL);
    }
}

int main(int argc, char *argv[]) {
    struct top_level_window game_app = {};
    struct events_data user_input = {};
    struct audio_assets audio = {};

    // FPS
    struct timespec tstart = {0, 0}, tend = {0, 0};

    struct frame_time_info frames_data = {};
    init_frame_rate_parameters(&frames_data);

    // struct mouse_data mouse_input = {};

    top_level_window_ptr =
        &game_app;  // no touching this pointer unless in atexit >:(
    init_window(&game_app);
    atexit(cleanup);

    // Assets
    struct asset_information asset_1 = {};

    struct text_information text_1 = {};
    struct text_information frames_text = {};

    // init Assets
    init_asset_dimensions(&asset_1, 100, 100, 100, 100);
    SDL_Colour colour = {0, 0, 0};
    init_text_information(
        &text_1, "It was the beepst of times, it was the boopst of times.\0",
        colour, 24, 200, 500, 750, 100);
    init_text_information(&frames_text, "\0", colour, 128, 900, 0, 300, 120);

    struct asset_information asset_2[50000] = {};
    init_asset_dimensions(&asset_2[0], rand() % 1280, 0, 5, 5);
    asset_2[0].asset_texture =
        load_asset(&game_app, "Texture_assets/point.png");
    int i;
    for (i = 1; i < 50000; i++) {
        init_asset_dimensions(&asset_2[i], rand() % 1280, 0, 5, 5);

        asset_2[i].asset_texture = asset_2[0].asset_texture;
    }

    asset_1.asset_texture =
        load_asset(&game_app, "Texture_assets/Hello_World.png");
    init_audio(&audio);

    // play_music(audio.music, -1);

    clock_gettime(CLOCK_MONOTONIC, &frames_data.prev_time);
    // main loop
    while (1) {
        // clock_gettime(CLOCK_MONOTONIC, &tstart);

        prep_screen(&game_app);

        process_input(&user_input);
        if (user_input.mouse_clicked == true) {
            play_sound_effects(audio.sound_effects[0],
                               0);  // change the tailing 0 to change the
                                    // channel that audio is played from
            user_input.mouse_clicked = false;
        }

        update_asset_location_user_input(&asset_1, &user_input, 5);  //
        update_asset_size(&asset_1, &user_input);

        for (i = 0; i < 40000; i++) {
            set_asset_position(&asset_2[i], asset_2[i].asset_x,
                               asset_2[i].asset_y + rand() % 10);
            draw_texture(&game_app, &asset_2[i]);
            if (asset_2[i].asset_y > 500) {
                asset_2[i].asset_y = 0;
            }
        }

        draw_texture(&game_app, &asset_1);
        draw_text(&game_app, &text_1);
        draw_text(&game_app, &frames_text);

        present_screen(&game_app);

        sprintf(frames_text.text, "%d FPS", frames_data.current_framerate);

        calculate_framerate(&frames_data);
        frame_rate_cap(&frames_data);

        // clock_gettime(CLOCK_MONOTONIC, &tend);
        /*printf("some_long_computation took about %.5f seconds\n",
               ((double)tend.tv_sec + 1.0e-9 * tend.tv_nsec) -
                   ((double)tstart.tv_sec + 1.0e-9 * tstart.tv_nsec));*/
    }

    return 0;
}