#include <omp.h>
#include "raytracer.h"
#include <SDL3/SDL.h>


const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 720;


// ─── LIGHTS ────────────────────────────────────────────────
Light lights[] = {
    { .k = 'a', .i = 0.10, .v = {0, 0, 0} },            // dim ambient — moody
    { .k = 'p', .i = 0.55, .v = { 0.0, 5.0, 10.0} },    // overhead lantern glow
    { .k = 'p', .i = 0.25, .v = {-4.0, 0.5,  6.0} },    // left fill — cool blue
    { .k = 'd', .i = 0.10, .v = { 0.3,-1.0,  0.5} },    // faint downward rim
};
int num_lights = 4;

// ─── HITTABLES ─────────────────────────────────────────────
Hittable hittables[] = {

    // ════════════════════════════════════════════════════════
    //  REFLECTIVE FLOOR — dark marble, two triangles
    // ════════════════════════════════════════════════════════
    { .k='t', .tri={ .a={-12,-2, 1}, .b={ 12,-2, 1}, .c={ 12,-2,30},
                     .color={20,20,28}, .s=120, .rfl=0.65 }},
    { .k='t', .tri={ .a={-12,-2, 1}, .b={ 12,-2,30}, .c={-12,-2,30},
                     .color={20,20,28}, .s=120, .rfl=0.65 }},

    // ════════════════════════════════════════════════════════
    //  CENTREPIECE — large glowing orb IMPALED by a triangle
    //  shard through its core
    // ════════════════════════════════════════════════════════

    // The orb — warm gold, very shiny, moderately reflective
    { .k='s', .sph={ .r=1.2, .c={ 0.0, 0.2, 9.0},
                     .color={255,200,60}, .s=180, .rfl=0.40 }},

    // Shard piercing through — dark obsidian blade
    // enters bottom-left, exits top-right of the orb
    { .k='t', .tri={ .a={-2.5,-2.0, 8.5}, .b={ 2.5, 3.0, 9.5}, .c={ 0.0,-1.5,10.5},
                     .color={15,10,20}, .s=200, .rfl=0.75 }},
    { .k='t', .tri={ .a={-2.5,-2.0, 8.5}, .b={ 0.0, 3.5, 8.8}, .c={ 2.5, 3.0, 9.5},
                     .color={15,10,20}, .s=200, .rfl=0.75 }},

    // ════════════════════════════════════════════════════════
    //  LEFT ALTAR — stone plinth (box from triangles)
    //  topped with a small vivid magenta sphere
    // ════════════════════════════════════════════════════════

    // Plinth front face
    { .k='t', .tri={ .a={-4.5,-2, 6}, .b={-2.5,-2, 6}, .c={-2.5, 0, 6},
                     .color={70,65,80}, .s=10, .rfl=0.05 }},
    { .k='t', .tri={ .a={-4.5,-2, 6}, .b={-2.5, 0, 6}, .c={-4.5, 0, 6},
                     .color={70,65,80}, .s=10, .rfl=0.05 }},
    // Plinth top face
    { .k='t', .tri={ .a={-4.5, 0, 6}, .b={-2.5, 0, 6}, .c={-2.5, 0, 8},
                     .color={90,85,100}, .s=15, .rfl=0.08 }},
    { .k='t', .tri={ .a={-4.5, 0, 6}, .b={-2.5, 0, 8}, .c={-4.5, 0, 8},
                     .color={90,85,100}, .s=15, .rfl=0.08 }},
    // Plinth right face
    { .k='t', .tri={ .a={-2.5,-2, 6}, .b={-2.5,-2, 8}, .c={-2.5, 0, 8},
                     .color={50,45,60}, .s=10, .rfl=0.05 }},
    { .k='t', .tri={ .a={-2.5,-2, 6}, .b={-2.5, 0, 8}, .c={-2.5, 0, 6},
                     .color={50,45,60}, .s=10, .rfl=0.05 }},

    // Magenta sphere resting on plinth
    { .k='s', .sph={ .r=0.5, .c={-3.5, 0.5, 7.0},
                     .color={230,20,180}, .s=100, .rfl=0.30 }},

    // ════════════════════════════════════════════════════════
    //  RIGHT ALTAR — same plinth, topped with cyan sphere
    // ════════════════════════════════════════════════════════

    // Plinth front face
    { .k='t', .tri={ .a={ 2.5,-2, 6}, .b={ 4.5,-2, 6}, .c={ 4.5, 0, 6},
                     .color={70,65,80}, .s=10, .rfl=0.05 }},
    { .k='t', .tri={ .a={ 2.5,-2, 6}, .b={ 4.5, 0, 6}, .c={ 2.5, 0, 6},
                     .color={70,65,80}, .s=10, .rfl=0.05 }},
    // Plinth top face
    { .k='t', .tri={ .a={ 2.5, 0, 6}, .b={ 4.5, 0, 6}, .c={ 4.5, 0, 8},
                     .color={90,85,100}, .s=15, .rfl=0.08 }},
    { .k='t', .tri={ .a={ 2.5, 0, 6}, .b={ 4.5, 0, 8}, .c={ 2.5, 0, 8},
                     .color={90,85,100}, .s=15, .rfl=0.08 }},
    // Plinth left face
    { .k='t', .tri={ .a={ 2.5,-2, 6}, .b={ 2.5,-2, 8}, .c={ 2.5, 0, 8},
                     .color={50,45,60}, .s=10, .rfl=0.05 }},
    { .k='t', .tri={ .a={ 2.5,-2, 6}, .b={ 2.5, 0, 8}, .c={ 2.5, 0, 6},
                     .color={50,45,60}, .s=10, .rfl=0.05 }},

    // Cyan sphere resting on plinth
    { .k='s', .sph={ .r=0.5, .c={ 3.5, 0.5, 7.0},
                     .color={20,220,210}, .s=100, .rfl=0.30 }},

    // ════════════════════════════════════════════════════════
    //  FLOATING CROWN — ring of 6 triangular spikes
    //  orbiting above the gold orb
    // ════════════════════════════════════════════════════════

    // Each spike: thin isoceles triangle pointing outward/upward
    // arranged in a rough hexagonal halo at y≈3, z≈9

    // Spike 1 — front
    { .k='t', .tri={ .a={ 0.0, 4.5, 9.0}, .b={-0.3, 3.0, 8.2}, .c={ 0.3, 3.0, 8.2},
                     .color={255,240,80}, .s=150, .rfl=0.50 }},
    // Spike 2 — front-right
    { .k='t', .tri={ .a={ 1.3, 4.5, 8.5}, .b={ 0.8, 3.0, 8.0}, .c={ 1.2, 3.0, 9.0},
                     .color={255,200,50}, .s=150, .rfl=0.50 }},
    // Spike 3 — back-right
    { .k='t', .tri={ .a={ 1.3, 4.5, 9.8}, .b={ 0.8, 3.0, 9.2}, .c={ 1.2, 3.0,10.2},
                     .color={255,200,50}, .s=150, .rfl=0.50 }},
    // Spike 4 — back
    { .k='t', .tri={ .a={ 0.0, 4.5, 9.8}, .b={-0.3, 3.0, 9.8}, .c={ 0.3, 3.0, 9.8},
                     .color={255,240,80}, .s=150, .rfl=0.50 }},
    // Spike 5 — back-left
    { .k='t', .tri={ .a={-1.3, 4.5, 9.8}, .b={-1.2, 3.0, 9.2}, .c={-0.8, 3.0,10.2},
                     .color={255,200,50}, .s=150, .rfl=0.50 }},
    // Spike 6 — front-left
    { .k='t', .tri={ .a={-1.3, 4.5, 8.5}, .b={-1.2, 3.0, 8.0}, .c={-0.8, 3.0, 9.0},
                     .color={255,200,50}, .s=150, .rfl=0.50 }},

    // ════════════════════════════════════════════════════════
    //  BACKGROUND — dark void wall + two distant orbs
    // ════════════════════════════════════════════════════════

    // Back wall
    { .k='t', .tri={ .a={-12, 8,28}, .b={ 12, 8,28}, .c={ 12,-2,28},
                     .color={8,6,14}, .s=2, .rfl=0.0 }},
    { .k='t', .tri={ .a={-12, 8,28}, .b={ 12,-2,28}, .c={-12,-2,28},
                     .color={8,6,14}, .s=2, .rfl=0.0 }},

    // Distant left orb — deep blue, like a cold moon
    { .k='s', .sph={ .r=0.8, .c={-5.5, 3.5, 20.0},
                     .color={40,80,200}, .s=60, .rfl=0.20 }},

    // Distant right orb — deep red ember
    { .k='s', .sph={ .r=0.8, .c={ 5.5, 3.5, 20.0},
                     .color={200,40,30}, .s=60, .rfl=0.20 }},

};
int num_hittables = 35;
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



