#include <math.h>
#include <omp.h>
#include "raytracer.h"
#include <SDL3/SDL.h>
#include <stdlib.h>


const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 720;

RGB ring_sph(Point p, RGB color){
  if ((int)(sqrt(p.x*p.x + p.z*p.z)) % 2 == 0){
    color = (RGB){.r = 255, .g = 255, .b = 255};
  }
  return color;
}
// for doing color gradient of triangle using barycenteric coordinate
// gb[] is used where gb[0] is gamma, gb[1] is beta
RGB gradient_tri(Point p, RGB color, double gb[]){
  RGB red = (RGB){.r = 254};
  RGB green = (RGB){.g = 254};
  RGB blue = (RGB){.b = 254};
  // doing linear interpolation using alpha, beta and gamma
  color.r = (1-gb[0] -gb[1])*red.r + gb[0]*green.r + gb[1]*blue.r;
  color.g = (1-gb[0] -gb[1])*red.g + gb[0]*green.g + gb[1]*blue.g;
  color.b = (1-gb[0] -gb[1])*red.b + gb[0]*green.b + gb[1]*blue.b;
  return color;  
}


RGB zebra_sph(Point p, RGB color){
  // double theta = acos(p.z/norm(p));
  // double psi = atan2(p.y, p.x);
  // getting u, v coordinates where u,v varies [0, 1]
  double u = (M_PI + atan2(p.y, p.x))/(2*M_PI);
  double v = (M_PI - acos(p.z/fabs(p.x)))/M_PI;
  // scale it by factor
  double factor = 10;
  u *= factor;
  v *= factor;

  
  int nu = (int)u + 1;
  int nv = (int)v + 1;
  if ((nv+nu) % 2 == 0)
    return (RGB){.r = 5, .g = 5, .b = 5};
  else
    return (RGB){.r = 255, .g = 255, .b = 255};
}
// checker for sphere using spherical coordinates
RGB checkered_sph(Point p, RGB color){
  // double theta = acos(p.z/norm(p));
  // double psi = atan2(p.y, p.x);
  // getting u, v coordinates where u,v varies [0, 1]
  double u = (M_PI + atan2(p.y, p.x))/(2*M_PI);
  double v = (M_PI - acos(p.z/fabs(p.x)))/M_PI;
  // scale it by factor
  double factor = 12;
  u *= factor;
  v *= factor;

  int nu = (int)u + 1;
  int nv = (int)v + 1;

  if ( ((int)(factor*sqrt(p.x*p.x + p.z*p.z)) + nv + nu) % 2 == 0)
    return (RGB){.r = 5, .g = 5, .b = 5};
  else
    return (RGB){.r = 255, .g = 255, .b = 255};
}
RGB checkered(Point p, RGB color){
  int x  = (int)p.x + 1;
  int z  = (int)p.z + 1;
  if ( (abs(x) % 2 == 1 && abs(z) % 2 == 1) || ( x % 2 == 0 && z % 2 == 0) )
    color = (RGB){.r = 0, .g = 0, .b = 0};
  else
    color = (RGB){.r = 255, .g = 255, .b = 255};
  return color;
}
// Hittable objects array
Hittable hittables[] = {
  // Ground plane (XY plane at z = -5)
  {
    .k = 'p',
    .pln = {
      .n = {0, -0.8, 0.2},           // normal pointing towards camera
      .d = -5,                  // distance from origin
      .color = {100, 100, 100}, // gray
      .s = 20,                  // moderate shininess
      .rfl = 0.3,                // slightly reflective
      .pattern = checkered
    }
  },

  // Large red sphere (left side)
  {
    .k = 's',
    .sph = {
      .c = {-3, 36, 100},
      .r = 35,
      .color = {255, 50, 50},   // red
      .s = 30,                  // shiny
      .rfl = 0.0,                // reflective
      .pattern = ring_sph  
    }
  },

  // Blue sphere (center)
  {
    .k = 's',
    .sph = {
      .c = {0, 0, 8},
      .r = 2.0,
      .color = {50, 100, 255},  // blue
      .s = 25,
      .rfl = 0.4,
      .pattern = checkered_sph  
    }
  },

  // Green sphere (right side)
  {
    .k = 's',
    .sph = {
      .c = {3, 2, 6},
      .r = 1.2,
      .color = {50, 200, 50},   // green
      .s = 15,
      .rfl = 0.2,
      .pattern = checkered
    }
  },

  // Triangle (reflective, tilted)
  {
    .k = 't',
    .tri = {
      .a = {4, -2, 8},
      .b = {2, -2, 4},
      .c = {0, 2, 5},
      .color = {200, 200, 50},  // yellow
      .s = 20,
      .rfl = 0.35,
      .pattern = gradient_tri
    }
  }
};

// Lights array
Light lights[] = {
  // Ambient light (provides base illumination)
  {
    .k = 'a',
    .i = 0.3,                   // 30% ambient
    .v = {0, 0, 0}              // unused for ambient
  },

  // Directional light (from upper right, behind)
  {
    .k = 'd',
    .i = 0.5,                   // 50% of total light
    .v = {1, 1, -1}             // direction (will need normalization in your code)
  },

  // Point light source (above center)
  {
    .k = 'p',
    .i = 0.2,                   // 20% of total light
    .v = {0, 0, 10}             // position in 3D space
  }
};
int num_lights = 3;
int num_hittables = 5;

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

    SDL_Window *window = SDL_CreateWindow("c-rt", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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



