#include <omp.h>
#include "raytracer.h"
#include <SDL3/SDL.h>


const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 720;


// ─── LIGHTS ────────────────────────────────────────────────
// Ambient light (soft warm fill)
Light lights[] = {
    { .k = 'a', .i = 0.15, .v = {0, 0, 0} },

    // Directional light from upper-left (warm sunset angle)
    { .k = 'd', .i = 0.65, .v = {-0.6, 0.8, 0.3} },

    // Point light — mimics the sun glow up close
    { .k = 'p', .i = 0.20, .v = {0.0, 3.0, 8.0} },
};
int num_lights = 3;

// ─── SPHERES ───────────────────────────────────────────────

// 1. Large SUN sphere — far away, glowing yellow, not shiny, not reflective
const Sphere sun = {
    .r     = 3.5,
    .c     = { 0.0, 4.5, 30.0 },
    .color = { 255, 220, 50 },
    .s     = -1,      // matte (no specular)
    .rfl   = 0.0,
};

// 2. Blue reflective sphere — center stage
const Sphere blue_sphere = {
    .r     = 1.0,
    .c     = { 0.0, 0.0, 6.0 },
    .color = { 50, 120, 255 },
    .s     = 80,
    .rfl   = 0.45,
};

// 3. Red matte sphere — left side
const Sphere red_sphere = {
    .r     = 0.7,
    .c     = { -2.2, -0.3, 5.0 },
    .color = { 220, 50, 50 },
    .s     = 20,
    .rfl   = 0.05,
};

// 4. Small silver mirror sphere — right side
const Sphere silver_sphere = {
    .r     = 0.45,
    .c     = { 2.0, -0.55, 4.5 },
    .color = { 200, 200, 210 },
    .s     = 200,
    .rfl   = 0.85,
};

// 5. Small teal accent sphere — behind and left
const Sphere teal_sphere = {
    .r     = 0.35,
    .c     = { -1.2, -0.65, 4.0 },
    .color = { 30, 200, 180 },
    .s     = 60,
    .rfl   = 0.2,
};

// ─── GROUND PLANE (two triangles forming a large quad) ─────
//
//   D(-15,-1, 0) ──────── C(15,-1, 0)
//        |                     |
//   A(-15,-1,40) ───────  B(15,-1,40)
//
// Triangle 1: A, B, C  (front-right half)
// Triangle 2: A, C, D  (back-left half)
// Ground is a warm sandy/earthy tone

const Triangle ground1 = {
    .a     = { -15.0, -1.0, 40.0 },
    .b     = {  15.0, -1.0, 40.0 },
    .c     = {  15.0, -1.0,  0.0 },
    .s     = 5,
    .rfl   = 0.08,
    .color = { 180, 140, 90 },
};

const Triangle ground2 = {
    .a     = { -15.0, -1.0, 40.0 },
    .b     = {  15.0, -1.0,  0.0 },
    .c     = { -15.0, -1.0,  0.0 },
    .s     = 5,
    .rfl   = 0.08,
    .color = { 180, 140, 90 },
};

// ─── HITTABLES ARRAY ───────────────────────────────────────
Hittable hittables[] = {
    { .k = 's', .sph = sun          },
    { .k = 's', .sph = blue_sphere  },
    { .k = 's', .sph = red_sphere   },
    { .k = 's', .sph = silver_sphere},
    { .k = 's', .sph = teal_sphere  },
    { .k = 't', .tri = ground1      },
    { .k = 't', .tri = ground2      },
};
int num_hittables = 7;

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



