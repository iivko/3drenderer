#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL.h>


bool is_running = false;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;

int window_width;
int window_height;


bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    // Use SDL to query what is the fullscreen max. Width and Height
    SDL_DisplayMode display_mode;
    if (SDL_GetCurrentDisplayMode(0, &display_mode) != 0) {
        fprintf(stderr, "Error getting display mode: %s\n", SDL_GetError());
        return false;
    }

    window_width = display_mode.w;
    window_height = display_mode.h;

    // Create a SDL Window
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_BORDERLESS
    );

    if (!window) {
        fprintf(stderr, "Error creating SDL Window: %s\n", SDL_GetError());
        return false;
    }


    // Create an SDL renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

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

void draw_grid(void) {
    int gap = 10;

    int rows = window_height / gap;
    int cols = window_width / gap;
    // SDL_Log("Rows: %d", rows);
    // SDL_Log("Cols: %d", cols);


    // for (int y = 0; y < window_height; y++) {
    //     for (int x = 0; x < window_width; x++) {
    //         if (x % gap == 0 || y % gap == 0) {
    //             color_buffer[(window_width * y) + x] = 0x808080FF;
    //         }
    //     }
    // }

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            color_buffer[(window_width * (y * gap) + x * gap)] = 0x808080FF;
        }
    }


}

void render_color_buffer(void) {
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int)(window_width * sizeof(uint32_t))
    );
    SDL_RenderCopy(
        renderer,
        color_buffer_texture,
        NULL,
        NULL
    );
}

// color - hex number representing our color
void clear_color_buffer(uint32_t color) {
    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            // Calculate the index of the pixel at column x, row y in the color buffer.
            // The formula is: index = (window_width * row) + column
            color_buffer[(window_width * y) + x] = color;
        }
    }

}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_grid();

    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

void destroy_window(void) {
    free(color_buffer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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

