#include <omp.h>
#include "raytracer.h"
#include <SDL3/SDL.h>


const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 720;

// === LIGHTS ===
Light lights[] = {
    // Ambient light
    { .k = 'a', .i = 0.2f, .v = {0.0f, 0.0f, 0.0f} },

    // Point light
    { .k = 'p', .i = 0.6f, .v = {2.0f, 1.0f, 0.0f} },

    // Directional light
    { .k = 'd', .i = 0.2f, .v = {1.0f, 4.0f, 4.0f} },
};
int num_lights = sizeof(lights) / sizeof(Light);


// === SPHERES ===
Sphere spheres[] = {
    // s1 - Red sphere
    {
        .c     = {0.0f, -1.0f, 3.0f},
        .r     = 1.0f,
        .color = {255, 0, 0},
        .s     = 9900,
        .rfl   = 0.2f
    },

    // s2 - Blue sphere
    {
        .c     = {-2.0f, 1.0f, 3.0f},
        .r     = 1.0f,
        .color = {0, 0, 255},
        .s     = 5,
        .rfl   = 0.0f
    },

    // s3 - Green sphere
    {
        .c     = {2.0f, 1.0f, 3.0f},
        .r     = 1.0f,
        .color = {0, 255, 0},
        .s     = 50,
        .rfl   = 0.5f
    },

    // s4 - Yellow floor sphere
    {
        .c     = {0.0f, -5001.0f, 0.0f},
        .r     = 5000.0f,
        .color = {255, 255, 0},
        .s     = 1000,
        .rfl   = 0.01f
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

    SDL_Window *window = SDL_CreateWindow("Ray Tracer By <Name>", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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



