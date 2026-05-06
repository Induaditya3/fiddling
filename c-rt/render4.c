
#include "raytracer.h"
#include <SDL3/SDL.h>
#include <omp.h>

const int WINDOW_WIDTH =  700 ;
const int WINDOW_HEIGHT = 700;



// ─── LIGHTS ────────────────────────────────────────────────
// Dark ambient — near-black night sky
Light lights[] = {
    { .k = 'a', .i = 0.08, .v = {0, 0, 0} },

    // Neon pink point light — main scene glow (street level)
    { .k = 'p', .i = 0.50, .v = { 0.0,  1.0, 8.0 } },

    // Cyan point light — from the right, cold fill
    { .k = 'p', .i = 0.30, .v = { 4.0,  2.0, 6.0 } },

    // Deep purple directional — top-down moody backlight
    { .k = 'd', .i = 0.12, .v = { 0.0, -1.0, 0.3 } },
};
int num_lights = 4;

// ─── HELPER MACRO ──────────────────────────────────────────
// Just for readability while defining triangles
#define TRI(ax,ay,az, bx,by,bz, cx,cy,cz, R,G,B, shin, refl) \
    { .k = 't', .tri = {                                       \
        .a={ax,ay,az}, .b={bx,by,bz}, .c={cx,cy,cz},          \
        .color={R,G,B}, .s=shin, .rfl=refl }}

// ═══════════════════════════════════════════════════════════
//  SCENE
// ═══════════════════════════════════════════════════════════
Hittable hittables[] = {

    // ── GRID FLOOR ─────────────────────────────────────────
    // Two large triangles forming the ground quad
    // Slightly reflective dark surface with cyan tint (wet street)
    //   (-12,-1.8, 2) --- (12,-1.8, 2)
    //        |                  |
    //   (-12,-1.8,30) --- (12,-1.8,30)

    TRI(-12,-1.8, 2,   12,-1.8, 2,   12,-1.8,30,   10,30,35,  8, 0.55),
    TRI(-12,-1.8, 2,   12,-1.8,30,  -12,-1.8,30,   10,30,35,  8, 0.55),

    // ── LEFT BUILDING — tall slab ──────────────────────────
    // Front face (two triangles = quad)
    //  neon-purple tinted concrete
    TRI(-9.0, 8.0, 18,  -5.5, 8.0, 18,  -5.5,-1.8,18,   40,10,60, 15, 0.10),
    TRI(-9.0, 8.0, 18,  -5.5,-1.8, 18,  -9.0,-1.8,18,   40,10,60, 15, 0.10),

    // Right face (side, darker)
    TRI(-5.5, 8.0, 18,  -5.5, 8.0, 28,  -5.5,-1.8,28,   25, 5,40, 10, 0.08),
    TRI(-5.5, 8.0, 18,  -5.5,-1.8, 28,  -5.5,-1.8,18,   25, 5,40, 10, 0.08),

    // Roof (flat top)
    TRI(-9.0, 8.0, 18,  -5.5, 8.0, 18,  -5.5, 8.0, 28,   60,20,90, 30, 0.20),
    TRI(-9.0, 8.0, 18,  -5.5, 8.0, 28,  -9.0, 8.0, 28,   60,20,90, 30, 0.20),

    // ── RIGHT BUILDING — shorter, wider ───────────────────
    // Front face — hot pink neon tint
    TRI( 5.0, 5.5, 16,   9.5, 5.5, 16,   9.5,-1.8,16,  100, 5,80, 20, 0.12),
    TRI( 5.0, 5.5, 16,   9.5,-1.8, 16,   5.0,-1.8,16,  100, 5,80, 20, 0.12),

    // Left face (side)
    TRI( 5.0, 5.5, 16,   5.0, 5.5, 26,   5.0,-1.8,26,   70, 3,55, 12, 0.08),
    TRI( 5.0, 5.5, 16,   5.0,-1.8, 26,   5.0,-1.8,16,   70, 3,55, 12, 0.08),

    // Roof
    TRI( 5.0, 5.5, 16,   9.5, 5.5, 16,   9.5, 5.5, 26,   130,10,100,40, 0.25),
    TRI( 5.0, 5.5, 16,   9.5, 5.5, 26,   5.0, 5.5, 26,   130,10,100,40, 0.25),

    // ── CENTER BACKGROUND TOWER — thin skyscraper ─────────
    // Front face — cold cyan glass
    TRI(-1.5,14.0, 25,   1.5,14.0, 25,   1.5,-1.8,25,    5,180,200, 80, 0.55),
    TRI(-1.5,14.0, 25,   1.5,-1.8, 25,  -1.5,-1.8,25,    5,180,200, 80, 0.55),

    // Right face
    TRI( 1.5,14.0, 25,   1.5,14.0, 32,   1.5,-1.8,32,    3,120,140, 50, 0.40),
    TRI( 1.5,14.0, 25,   1.5,-1.8, 32,   1.5,-1.8,25,    3,120,140, 50, 0.40),

    // Pointed roof — single triangle spike (cyberpunk antenna)
    TRI(-1.5,14.0, 25,   1.5,14.0, 25,   0.0,18.0,25,    0,255,255,120, 0.60),

    // ── NEON SIGN — floating diamond (4 triangles) ────────
    // A flat diamond shape in front of the left building
    // Glowing hot pink, very shiny
    //        top (−2.5, 1.5, 10)
    //  left (−4,0,10)    right (−1,0,10)
    //        bot (−2.5,−1,10)
    TRI(-2.5, 1.5,10,  -4.0, 0.0,10,  -2.5,-1.0,10,  255, 20,180,200,0.30),
    TRI(-2.5, 1.5,10,  -2.5,-1.0,10,  -1.0, 0.0,10,  255, 20,180,200,0.30),

    // ── NEON SIGN — floating triangle (right side) ────────
    // Equilateral-ish glowing cyan triangle
    TRI( 2.5, 2.5, 9,   4.5,-0.5, 9,   0.5,-0.5, 9,   20,255,230,200,0.35),

    // ── FLYING DEBRIS / SHARDS (foreground atmosphere) ────
    // Small dark angular shards floating in mid-air

    // Shard 1 — left foreground
    TRI(-3.5, 0.5, 4,  -2.8, 1.2, 4,  -3.0, 0.2, 4,   15,15,25, 50, 0.70),

    // Shard 2 — right foreground
    TRI( 3.0, 0.8, 3,   3.8, 0.2, 3,   3.2, 1.4, 3,   15,15,25, 50, 0.70),

    // Shard 3 — center high
    TRI(-0.4, 2.8, 5,   0.4, 2.5, 5,   0.1, 3.4, 5,   20,20,35, 50, 0.65),

    // ── BACK WALL / SKY BACKDROP ──────────────────────────
    // A huge dark quad far back — acts as the night sky
    // Very dark navy, no reflectivity
    TRI(-20,20,35,  20,20,35,  20,-1.8,35,    5, 5,18,  2, 0.0),
    TRI(-20,20,35,  20,-1.8,35, -20,-1.8,35,  5, 5,18,  2, 0.0),

};
int num_hittables = 30;

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



