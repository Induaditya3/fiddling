#include "raytracer.h"

#include <math.h>

/* Vector Operation */

// 3D vector subtraction
Point sub3(Point p1, Point p2){
  Point p;
  p.x = p1.x - p2.x;
  p.y = p1.y - p2.y;
  p.z = p1.z - p2.z;
  return p;
}

// 3D vector addition
Point add3(Point p1, Point p2){
  Point p;
  p.x = p1.x + p2.x;
  p.y = p1.y + p2.y;
  p.z = p1.z + p2.z;
  return p;
}

// scale a 3D vector by constant factor
Point scale(double k, Point p2){
  Point p;
  p.x = k * p2.x;
  p.y = k * p2.y;
  p.z = k * p2.z;
  return p;
}

// dot product of two 3D vector
double dot(Point p1, Point p2){
  return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

// cross product
Point cross(Point p1, Point p2){
  double b = p1.x;
  double e = p1.y;
  double h = p1.z;
  double c = p2.x;
  double f = p2.y;
  double i = p2.z;
  return (Point) {.x = e*i-f*h, .y = h*c-b*i, .z = b*f-e*c};
}
// norm of a 3D vector
double norm(Point p){
  return sqrt(dot(p,p));
}

Point reflectedRay(Point ncap, Point l){
  return sub3(scale(2*dot(l, ncap), ncap), l);
}
/* Solution of Quadratic Equation */
// modifies pointer two roots if solution is real else INF
void quad(double a, double b, double c, double roots[]){
  double d = b*b - 4*a*c;
  roots[0] = INFINITY;
  roots[1] = INFINITY;
  if (d >= 0){
    roots[0] = (-b + sqrt(d)) / (2*a);
    roots[1] = (-b - sqrt(d)) / (2*a);
  }
}

/* Color related functions */
// clamping color channels value to 0 to 255
short int clamp(double x){
  if (x > 255) return 255;
  else if (x < 0) return 0;
  else return (short int) round(x);
}
// adjust brightness of color
RGB adjustColor(RGB rgb, double intensity){
  RGB newrgb;
  newrgb.b = clamp(rgb.b * intensity);
  newrgb.g = clamp(rgb.g * intensity);
  newrgb.r = clamp(rgb.r * intensity);
  return newrgb;  
}
/* Restructuring code for supporting triangle */

// assign parameter t a finite value if triangle is intersected otherwise infinite value
// o is the origin i.e. from where ray originates
// direction is the direction of the ray
// tmin and tmax is the minimum and the maximum range of triangle
// tgb is the array on the storing computed value of t, g and b, t stores INF if intersection doesn't fit the conditions 
void instersectTriangle(Point o, Point direction, Triangle tri, double tmin, double tmax, double tgb[]){
  // factoring repeated terms so that same thing is computed once
  double a = tri.a.x  - tri.b.x  ;
  double b = tri.a.y  - tri.b.y  ;
  double c = tri.a.z  - tri.b.z  ;
  double d = tri.a.x  - tri.c.x  ;
  double e = tri.a.y  - tri.c.y  ;
  double f = tri.a.z  - tri.c.z  ;
  double g = direction.x;
  double h = direction.y;
  double i = direction.z;
  double j = tri.a.x - o.x ;
  double k = tri.a.y - o.y ;
  double l = tri.a.z - o.z ;
  // computing frequently occuring terms
  double eihf = e*i - h*f;
  double gfdi = g*f - d*i;
  double dheg = d*h - e*g;
  double akjb = a*k - j*b;
  double jcal = j*c - a*l;
  double blkc = b*l - k*c;
  // determining determinants for applying cramer's rule
  double A_det = a*(eihf ) + b*(gfdi) + c*(dheg);
  double t_det = -(f*(akjb) + e*(jcal) + d*(blkc));
  // t having INF value will act as flag
  tgb[0] = INFINITY;
  // checking if point on the triangle is within the visible range
  double t = t_det/A_det;
   if (t < tmin || t > tmax){
     tgb[0] = INFINITY;
     return;
   }
   double gamma_det = i*(akjb) + h*(jcal) + g*(blkc);
   double gamma = gamma_det/A_det;
   if (gamma < 0 || gamma > 1){
     tgb[0] = INFINITY;
     return;
   }
   double beta_det = j*(eihf) + k*(gfdi) + l*(dheg);
   double beta = beta_det/A_det;
   if (beta < 0 || beta > 1 - gamma){
     tgb[0] = INFINITY;
     return;
   }
  tgb[0] = t;
  tgb[1] = gamma;
  tgb[2] = beta;   
}

// sets parameter t to inf if intersection is almost parallel or out range [tmin, tmax]
// t is valid if it is finite
void intersectPlane(Point o, Point direction, Plane pl, double tmin, double tmax, double *t){
  // check if almost parallel intersection
  double denominator = dot(pl.n,direction);
  if (fabs(denominator) < EPSILON){
    t[0] = INFINITY;
    return;
  }
  t[0] =  -(dot(pl.n, o)+pl.d)/denominator;
  // check the range of parameter t
  if (t[0] < tmin || t[0] > tmax){
    t[0] = INFINITY;
  }
}
// return surface's color if it intensity is finite else return background color
// Point p is used for texture
RGB surfaceColor(Hittable surface, double intensity, Point p){
  if (isinf(intensity)){// rtx_inner uses intensity variable of infinite value as a flag
    return (RGB){0,0,0};
  }
  RGB color;
  if (surface.k == 's')
    color = surface.sph.color;
  else if (surface.k == 't')
    color = surface.tri.color;
  else if (surface.k == 'p'){
    color = surface.pln.color;
    int odd = (int)p.x + 1;
    if (p.x < odd && (odd % 2 == 1 || odd % 2 == -1))
      color = (RGB) {0,0,0};
    int even = (int)p.z + 1;
    if (p.z < even && even % 2 == 0)
      color = (RGB) {12,23,34};
  }
  return adjustColor(color, intensity);
}
/* Coordinate transformataion */
// transforming coordinate from graphics window to world coordinate (scene)
// setting width vw and vh of width and height of viewport respectively
// z coordinate of viewport is distance from camera(ie origin)

Point g_to_viewport(int gx, int gy, int gw, int gh){
  // setting vw and vh of viewport
  double vw, vh;
  vw = vh = 1;
  Point v;
  // expressing coordinate of viewport in terms of world coordinate
  v.x = (double)gx * vw / (double) gw;
  v.y = (double)gy * vh / (double) gh;
  v.z = 0.58;
  return v;
}

/* Intersection of sphere */
// modifies t for storing  parameters t1, t2
void instersectSphere(Point o, Point d, Sphere s, double roots[]){
  Point co = sub3(o, s.c);
  double a = dot(d,d);
  double b = 2 * dot(d, co);
  double c = dot(co, co) - (s.r * s.r);
  quad(a,b,c, roots);
}

// closest and visible Hittable surface from the given Point o and its associated parameter t of intersection with ray from camera
// closest_s should be heap allocated by caller
// closest_t , the parameter associated to closest surface, it also need to be heap allocated by caller and set to infinity
void closestHittable(Point o, Point d, double tmin, double tmax, int n_surface, Hittable s_arr[], Hittable* closest_s, double* c_t){
  // initialize the the reference variable for comparision
  Hittable surface;
  c_t[0] = INFINITY; // c_t having infinity value act as flag
  // iterate over all the surfaces check if they intersect and find closest surface based on parameter t
  for (int i = 0; i < n_surface; i++){
    surface = s_arr[i];
    // intersection with sphere
    if (surface.k == 's'){
      double t[2];
      instersectSphere(o, d, surface.sph, t);
      if (isfinite(t[0]) && t[0] >= tmin && t[0] <= tmax && t[0] < c_t[0]){
        c_t[0] = t[0];
        closest_s[0] = surface;
      }
      
      if (isfinite(t[0]) && t[1] >= tmin && t[1] <= tmax && t[1] < c_t[0]){
        c_t[0] = t[1];
        closest_s[0] = surface;
      }
    }
    // intersection with triangle
    if (surface.k == 't'){
      double tgb[3];
      instersectTriangle(o, d, surface.tri, tmin, tmax, tgb);
      if (isfinite(tgb[0]) && tgb[0] < c_t[0]){
        c_t[0] = tgb[0];
        closest_s[0] = surface;
      }
    }
    // intersection with plane
    if (surface.k == 'p'){
      double t;
      intersectPlane(o,d,surface.pln,tmin, tmax,&t);
      if (isfinite(t) && t < c_t[0]){
        c_t[0] = t;
        closest_s[0] = surface;
      }
    }
  }
}
// intensity of light contributed by diffuse reflection
double diffuseI(Point normal, Point l, double i){
  double nl = dot(normal,l);
  // check if light is illuminating surface which is visible to camera
  if (nl > 0)
    return i * nl / (norm(l) * norm(normal));
  return 0;
}
// intensity of light contributed by specular reflection
double specularI(Point o, Point p, Point normal, Point l, double i, int s){
  Point reflected = reflectedRay(normal, l);
  Point view = sub3(o, p);
  double rv = dot(reflected, view);
  // check if light is illuminating surface which is visible to camera
  if (rv > 0 && s > 0) {
    return i * pow(rv / (norm(reflected)*norm(view)), s);
  }
  return 0;
}

// total light intensity after reflection at point p
double tli(Point normal, Point p, Point o, int s, int no_lights, Light l_arr[], int no_surface, Hittable s_arr[]){
  Light light;
  Point l;
  double tmax;
  double c_intensity = 0;
  Hittable shadow_s;
  double shadow_t;
  // iterate over all the lights and 
  for (int i = 0; i < no_lights; i++){
    light = l_arr[i];
    // check for each kind of light
    // if light is ambient then add its contribution and consider next light
    if (light.k == 'a'){
      c_intensity += light.i;
      continue;
    }
    // if light is point source or directional then set light vector and tmax parameter accordingly
    else if (light.k == 'p'){
      l = sub3(light.v, p);
      tmax = 1;
    }
    else if (light.k == 'd'){
      l = light.v;
      tmax = INFINITY;
    }
    // check if light is obstructed by some other surface in the case of point source and directional light
    closestHittable(p, l, EPSILON, tmax, no_surface, s_arr, &shadow_s, &shadow_t);
    if (isinf(shadow_t)){
      // if not, add that light's contribution 
      c_intensity += specularI(o, p, normal, l, i, s);
      c_intensity += diffuseI(normal, l, i);
    }
  }
  return c_intensity;
}

// its return value intensity having infinite value is used as a flag, this is utilised in surfaceColor
// it also computes surface which is intersected by the ray from the camera and returns it indirectly thorough pointer assignment
double rtx_inner(Point o, Point d, double tmin, double tmax, int no_lights, int n_surface, Hittable s_arr[], Light l_arr[], int limit,
               Hittable *surface, Point *p){
  double objintensity, otherintensity;
  objintensity = otherintensity = 0;
  double t;
  // find closest surface from camera in the direction vector d
  closestHittable(o, d, tmin, tmax, n_surface, s_arr, surface, &t);
  // check if surface is valid by checking flag stored in parameter t
  if (isfinite(t)){
    p[0] = add3(o, (scale(t, d)));
    int shinesss;
    Point normal;
    double rfl;
    if (surface->k == 's'){
      normal = sub3(p[0], surface->sph.c);
      shinesss = surface->sph.s;
      rfl = surface->sph.rfl;
    }
    if (surface->k == 't'){
      normal = cross(
         sub3(surface->tri.a, p[0]),
         sub3(surface->tri.b, p[0])
        );
      shinesss = surface->tri.s;
      rfl = surface->tri.rfl;
    }
    if (surface->k == 'p'){
      normal = surface->pln.n;
      shinesss = surface->pln.s;
      rfl = surface->pln.rfl;
    }
    // find out how much it is illuminated directly i.e. object's intensity
    objintensity = tli(normal, p[0], o, shinesss, no_lights, l_arr, n_surface, s_arr);
    // check if object is reflective and recursion limit is reached
    if (shinesss > 0 && limit > 0){
      // find out reflected ray
      Point reflected = reflectedRay(
        scale(1 / norm(normal), normal),
        scale(-1, d)
      );
      // then compute new intensity contributed by other object 
      Hittable newsurface;
      Point newp;
      otherintensity = rtx_inner(p[0], reflected, EPSILON, INFINITY, no_lights, n_surface, s_arr, l_arr, limit-1, &newsurface, &newp);
      // check if intensity is valid using flag value of inf
      if (isfinite(otherintensity)){
        // if valid then take weighted sum of intensity which will be final intensity
        objintensity = objintensity * (1- rfl) + otherintensity*rfl;
      }
      return objintensity;
    }
  }
  return INFINITY;
}


RGB rtx(Point o, Point d, double tmin, double tmax, int no_lights, int n_surface, Hittable s_arr[], Light l_arr[]){
  Hittable surface;
  Point p;
  double intensity = rtx_inner(o, d, tmin, tmax, no_lights, n_surface, s_arr, l_arr, 3, &surface,&p);
  RGB rgb = surfaceColor(surface, intensity,p);
  return rgb;
}
