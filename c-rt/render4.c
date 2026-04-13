
#include "raytracer.h"
#include <SDL3/SDL.h>
#include <omp.h>

const int WINDOW_WIDTH =  700 ;
const int WINDOW_HEIGHT = 700;


// === LIGHTS ===
Light lights[] = {
    // Central soft white glow (symmetry axis)
    { .k = 'a', .i = 0.15f, .v = {0.0f, 0.0f, 0.0f} },

    // Left warm amber lamp (mirror of right)
    { .k = 'p', .i = 0.8f, .v = {-3.0f, 2.0f, 1.5f} },

    // Right warm amber lamp (mirror of left)
    { .k = 'p', .i = 0.8f, .v = {3.0f, 2.0f, 1.5f} },

    // Left cool blue accent (mirror of right)
    { .k = 'd', .i = 0.4f, .v = {-1.0f, -0.5f, 2.5f} },

    // Right cool blue accent (mirror of left)
    { .k = 'd', .i = 0.4f, .v = {1.0f, -0.5f, 2.5f} },
};
int num_lights = sizeof(lights) / sizeof(Light);


// === SPHERES ===
Sphere spheres[] = {
    // Central golden orb (axis point)
    {
        .c     = {0.0f, 1.5f, 4.0f},
        .r     = 0.7f,
        .color = {255, 220, 0},
        .s     = 4000,
        .rfl   = 0.85f
    },

    // Left silver sphere (mirror of right)
    {
        .c     = {-2.0f, 1.2f, 3.5f},
        .r     = 0.6f,
        .color = {200, 200, 200},
        .s     = 3000,
        .rfl   = 0.75f
    },

    // Right silver sphere (mirror of left)
    {
        .c     = {2.0f, 1.2f, 3.5f},
        .r     = 0.6f,
        .color = {200, 200, 200},
        .s     = 3000,
        .rfl   = 0.75f
    },

    // Left mossy stone (mirror of right)
    {
        .c     = {-0.5f, 0.3f, 2.8f},
        .r     = 0.5f,
        .color = {80, 120, 60},
        .s     = 150,
        .rfl   = 0.3f
    },

    // Right mossy stone (mirror of left)
    {
        .c     = {0.5f, 0.3f, 2.8f},
        .r     = 0.5f,
        .color = {80, 120, 60},
        .s     = 150,
        .rfl   = 0.3f
    },

    // Reflective floor (symmetrical base)
    {
        .c     = {0.0f, -5001.0f, 0.0f},
        .r     = 5000.0f,
        .color = {180, 180, 180},
        .s     = 1000,
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
            // Example: red gradient

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



