#include <omp.h>
#include "raytracer.h"
#include <SDL3/SDL.h>


const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 720;


// === LIGHTS ===
Light lights[] = {
    { .k = 'a', .i = 0.15f, .v = {0.0f, 0.0f, 0.0f} },

    { .k = 'p', .i = 0.8f, .v = {-3.0f, 2.5f, 1.0f} },

    { .k = 'p', .i = 0.7f, .v = {3.0f, 3.0f, 2.0f} },

    { .k = 'd', .i = 0.25f, .v = {0.5f, -1.0f, 2.0f} },
};
int num_lights = sizeof(lights) / sizeof(Light);


// === SPHERES ===
Sphere spheres[] = {
    {
        .c     = {-1.5f, 0.5f, 4.0f},
        .r     = 0.8f,
        .color = {0, 255, 255},
        .s     = 1,
        .rfl   = 0.9f
    },

    {
        .c     = {1.5f, 0.5f, 4.0f},
        .r     = 0.8f,
        .color = {255, 0, 255},
        .s     = 9900,
        .rfl   = 0.f
    },

    {
        .c     = {0.0f, 2.0f, 5.0f},
        .r     = 0.6f,
        .color = {200, 20, 60},
        .s     = 100,
        .rfl   = 0.3f
    },

    {
        .c     = {0.0f, -5001.0f, 0.0f},
        .r     = 5000.0f,
        .color = {10, 10, 20},
        .s     = 2000,
        .rfl   = 0.4f
    },
};
int num_spheres = sizeof(spheres) / sizeof(Sphere);

bool quit()
{
  SDL_Event e;
  if (SDL_PollEvent(&e))
  {
    if (e.type == SDL_EVENT_QUIT)
    {
      return false;
    }
    if (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE)
    {
      return false;
    }
  }

  SDL_Delay(1);
  return true;
}
// Helper: get pointer to a pixel at (x, y)
static inline Uint8* get_pixel_ptr(SDL_Surface *surface, int x, int y) {
    int bpp = SDL_BYTESPERPIXEL(surface->format);
    return (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
}

// Set a pixel at (x, y) to an RGB color
void set_pixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    const SDL_PixelFormatDetails *fmt = SDL_GetPixelFormatDetails(surface->format);
    Uint32 color = SDL_MapRGB(fmt, NULL, r, g, b);
    Uint8 *pixel = get_pixel_ptr(surface, x, y);
    int bpp = SDL_BYTESPERPIXEL(surface->format);
    SDL_memcpy(pixel, &color, bpp); // safe for any bpp
}

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Pixels", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

    int x_offset = (int)WINDOW_WIDTH/2;
    int y_offset = (int)WINDOW_HEIGHT/2;
    Point o = (Point) {0, 0, 0};
    // Iterate over every pixel
    #pragma omp parallel for schedule(dynamic, 4)
    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {

            Point v = g_to_viewport(-x+x_offset, -y+y_offset, WINDOW_WIDTH, WINDOW_HEIGHT);
            Point d = sub3(v,o);
            RGB rgb = rtx(o, d, 1, INFINITY, num_lights, num_spheres, spheres,lights);
            Uint8 r = (Uint8) rgb.r;
            Uint8 g = (Uint8) rgb.g;
            Uint8 b = (Uint8) rgb.b;
            set_pixel(surface, x, y, r, g, b);
        }
    }

    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

    SDL_UpdateWindowSurface(window);
    while (quit());

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}



