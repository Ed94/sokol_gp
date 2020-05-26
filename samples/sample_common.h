#define FLEXTGL_IMPL
#include "flextgl.h"
#define NANOGCTX_IMPL
#include "nanogctx.h"
#define NANOGP_IMPL
#include "nanogp.h"
#include <SDL2/SDL.h>
#include <math.h>

int sample_app(void (*draw)(ng_context* ngctx, ngp_context* ngp)) {
    // initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // setup context attributes before window and context creation
    ngctx_prepare_attributes(&(ng_context_desc){.sample_count = 4});

    // create window
    SDL_Window *window = SDL_CreateWindow("GP Clear",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(!window) {
        fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
        return 1;
    }

    // create graphics context
    ng_context ngctx = {0};
    if(!ngctx_create(&ngctx, window)) {
        fprintf(stderr, "Failed to create NGCTX context: %s\n", ngctx_get_error(&ngctx));
        return 1;
    }
    ngctx_set_swap_interval(&ngctx, 1);

    // create NanoGP context
    ngp_context ngp = {0};
    if(!ngp_create(&ngp, &(ngp_desc){0})) {
        fprintf(stderr, "Failed to create NGP context: %s\n", ngp_get_error(&ngp));
        return 1;
    }
    ngp_set_clear_color(&ngp, (ngp_color){0.0f, 0.0f, 1.0f, 1.0f});

    // run loop
    while(!SDL_QuitRequested()) {
        // poll events
        SDL_Event event;
        while(SDL_PollEvent(&event)) { }

        draw(&ngctx, &ngp);

        // print FPS
        static uint32_t fps = 0;
        static uint32_t last = 0;
        uint32_t now = SDL_GetTicks();
        fps++;
        if(now >= last + 1000) {
            printf("FPS: %d\n", fps);
            last = now;
            fps = 0;
        }
    }

    // destroy
    ngp_destroy(&ngp);
    ngctx_destroy(&ngctx);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}