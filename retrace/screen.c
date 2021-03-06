#include "core/screen.h"

#ifdef RETRACE_SDL

#include "core/version.h"
#include "core/msg.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;

static int32_t texture_width;
static int32_t texture_height;
static bool fullscreen;

void screen_init(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);

    window = SDL_CreateWindow(
        CORE_NAME,                  // window title
        SDL_WINDOWPOS_CENTERED,     // initial x position
        SDL_WINDOWPOS_CENTERED,     // initial y position
        640, 480,                   // width and height, in pixels
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL    // flags
    );

    if (!window) {
        msg_error("Can't create main window: %s", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        msg_error("Can't create renderer: %s", SDL_GetError());
    }

    // reset state
    texture_width = 0;
    texture_height = 0;
}

void screen_swap(void)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void screen_upload(int32_t* buffer, int32_t width, int32_t height, int32_t pitch, int32_t output_height)
{
    SDL_RenderSetLogicalSize(renderer, width, output_height);

    if (texture_width != width || texture_height != height) {
        texture_width = width;
        texture_height = height;

        SDL_DisplayMode mode;
        SDL_GetDisplayMode(0, 0, &mode);

        // update window size and position
        SDL_SetWindowSize(window, width, output_height);
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

        // (re)create frame buffer texture
        if (texture) {
            SDL_DestroyTexture(texture);
        }

        texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            texture_width,
            texture_height
        );

        if (!texture) {
            msg_error("Can't create texture: %s", SDL_GetError());
        }
    }

    SDL_UpdateTexture(texture, NULL, buffer, pitch * sizeof(*buffer));
}

void screen_set_fullscreen(bool _fullscreen)
{
    uint32_t flags = _fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
    SDL_SetWindowFullscreen(window, flags);
    fullscreen = _fullscreen;
}

bool screen_get_fullscreen(void)
{
    return fullscreen;
}

void screen_close(void)
{
    SDL_DestroyTexture(texture);
    texture = NULL;

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;
}

#else // ifdef RETRACE_SDL

static bool fullscreen;

void screen_init(void)
{
}

void screen_swap(void)
{
}

void screen_upload(int32_t* buffer, int32_t width, int32_t height, int32_t pitch, int32_t output_height)
{
}

void screen_set_fullscreen(bool _fullscreen)
{
    fullscreen = _fullscreen;
}

bool screen_get_fullscreen(void)
{
    return fullscreen;
}

void screen_close(void)
{
}

#endif
