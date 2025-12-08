#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL.h>

#include "display.h"


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

void update(void) {
    // TODO:
}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_grid();
    draw_rectangle(100, 200, 300, 150, 0xFFFF00FF);
    draw_pixel(20, 20, 0xFFFFFF00);

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

