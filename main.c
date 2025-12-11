#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL.h>

#include "display.h"
#include "vector.h"


///////////////////////////////////////////////////////
// Declare an array of 3D vectors/points
///////////////////////////////////////////////////////
#define NUMBER_OF_POINTS (9 * 9 * 9)


vec3_t cube_points[NUMBER_OF_POINTS];
vec2_t project_points[NUMBER_OF_POINTS];

vec3_t camera_position = { .x = 0 , .y = 0, .z = -5 };

float fov_factor = 1024;

bool is_running = false;

void setup(void) {
    // Create a color buffer for the window.
    //
    // Each pixel uses 4 bytes (uint32_t), so the total memory is:
    //   bytes = 4 * window_width * window_height
    //
    // Example for 800x600 window:
    //   bytes = 4 * 800 * 600 = 1,920,000 bytes
    //   kilobytes = 1,920,000 / 1024 ≈ 1,875 KB
    //   megabytes = 1,875 / 1024 ≈ 1.83 MB
    // Allocate the required memory in bytes to hold the color buffer
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

    // Creating an SDL texture that is used to display the color buffer
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );

    int point_count = 0;
    // Start loading my array of vectors
    // From -1 to 1 (in the 9x9x9 cube)
    for (float x = -1; x <= 1; x += 0.25f) {
        for (float y = -1; y <= 1; y += 0.25f) {
            for (float z = -1; z <= 1; z += 0.25f) {
                vec3_t new_point = { .x = x, .y = y, .z = z };
                cube_points[point_count++] = new_point;
            }
        }
    }

}

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                is_running = false;
            }
            break;

        default:
            break;
    }

}

// Function that receives 3D vector and returns a projected 2D point
vec2_t project(vec3_t point) {
    vec2_t projected_point = {
        .x = (fov_factor * point.x) / point.z,
        .y = (fov_factor * point.y) / point.z,
    };

    return projected_point;
}

void update(void) {
    for (int i = 0; i < NUMBER_OF_POINTS; i++) {
        vec3_t point = cube_points[i];

        // Move the points away from the camera
        point.z -= camera_position.z;

        // Project the current point
        vec2_t projected_point = project(point);

        // Save projected 2D vector in the array of projected points
        project_points[i] = projected_point;
    }
}

void render(void) {
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderClear(renderer);

    draw_grid();
    // draw_rectangle(100, 200, 300, 150, 0xFFFF00FF);
    // draw_pixel(20, 20, 0xFFFFFF00);

    // Loop all projected points and render them
    for (int i = 0; i < NUMBER_OF_POINTS; i++) {
        vec2_t projected_point = project_points[i];
        draw_rectangle(
            projected_point.x + (window_width / 2),
            projected_point.y + (window_height / 2),
            4,
            4,
            0xFFFFFF00
        );
    }

    render_color_buffer();

    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    /* Create SDL window */
    is_running = initialize_window();

    /* Game loop */
    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}

