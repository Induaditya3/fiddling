#include <omp.h>
#include "raytracer.h"
#include <SDL3/SDL.h>


const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;

// ─── LIGHTS ────────────────────────────────────────────────
Light lights[] = {
    { .k = 'a', .i = 0.2,  .v = {0, 0, 0} },           // soft ambient
    { .k = 'd', .i = 0.8,  .v = {0.5, 1.0, 0.3} },     // sun from upper-right
};
int num_lights = 2;

// ─── HITTABLES ─────────────────────────────────────────────
Hittable hittables[] = {

    // ── GROUND (two triangles = flat quad) ─────────────────
    // warm grassy green, slight sheen
    { .k='t', .tri={ .a={-10,-1, 2}, .b={ 10,-1, 2}, .c={ 10,-1,20},
                     .color={60,120,40}, .s=5, .rfl=0.05 }},
    { .k='t', .tri={ .a={-10,-1, 2}, .b={ 10,-1,20}, .c={-10,-1,20},
                     .color={60,120,40}, .s=5, .rfl=0.05 }},

    // ── PYRAMID (4 triangular faces) ───────────────────────
    // apex at (0, 2, 10), base corners at y=-1
    // front face — lit, warm orange
    { .k='t', .tri={ .a={ 0, 2,10}, .b={-2,-1, 8}, .c={ 2,-1, 8},
                     .color={210,120,40}, .s=30, .rfl=0.10 }},
    // right face — slightly darker
    { .k='t', .tri={ .a={ 0, 2,10}, .b={ 2,-1, 8}, .c={ 2,-1,12},
                     .color={170, 90,30}, .s=30, .rfl=0.10 }},
    // back face — shadowed
    { .k='t', .tri={ .a={ 0, 2,10}, .b={ 2,-1,12}, .c={-2,-1,12},
                     .color={130, 70,20}, .s=30, .rfl=0.10 }},
    // left face
    { .k='t', .tri={ .a={ 0, 2,10}, .b={-2,-1,12}, .c={-2,-1, 8},
                     .color={160, 85,25}, .s=30, .rfl=0.10 }},

    // ── FLAT MIRROR ON THE GROUND (leaning upright) ────────
    // a single large reflective quad standing to the right
    { .k='t', .tri={ .a={ 4,-1, 9}, .b={ 6,-1, 9}, .c={ 6, 2, 9},
                     .color={200,200,220}, .s=150, .rfl=0.80 }},
    { .k='t', .tri={ .a={ 4,-1, 9}, .b={ 6, 2, 9}, .c={ 4, 2, 9},
                     .color={200,200,220}, .s=150, .rfl=0.80 }},

    // ── SMALL RED FLAG / PENNANT (single triangle) ─────────
    // sits on top-left, like a flag blowing in the wind
    { .k='t', .tri={ .a={-3, 2, 8}, .b={-1, 1, 8}, .c={-3, 0, 8},
                     .color={220,30,30}, .s=10, .rfl=0.02 }},

};
int num_hittables = 9;
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

            Point v = g_to_viewport(x-x_offset, -y+y_offset, WINDOW_WIDTH, WINDOW_HEIGHT);
            Point d = sub3(v,o);
            RGB rgb = rtx(o, d, 1, INFINITY, num_lights, num_hittables, hittables,lights);
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



