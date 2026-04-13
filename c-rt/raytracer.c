#include "raytracer.h"

#include <math.h>
#include <stdlib.h>
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

// norm of a 3D vector
double norm(Point p){
  return sqrt(dot(p,p));
}

Point reflectedRay(Point ncap, Point l){
  return sub3(scale(2*dot(l, ncap), ncap), l);
}
/* Solution of Quadratic Equation */
// returns pointer two roots if solution is real else NULL
double* quad(double a, double b, double c){
  double d = b*b - 4*a*c;
  double* roots = NULL;
  if (d >= 0){
    roots = (double*)malloc(sizeof(double)*2);
    roots[0] = (-b + sqrt(d)) / (2*a);
    roots[1] = (-b - sqrt(d)) / (2*a);
  }
  return roots;
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
void instersectTriangle(Point o, Point direction, Triangle tri){
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
  double gamma_det = i*(akjb) + h*(jcal) + g*(blkc);
  double beta_det = j*(eihf) + k*(gfdi) + l*(dheg);
  
}
// return sphere's color if its actually sphere else return background color
RGB sphereColor(Sphere sphere, double intensity){
  if (isinf(intensity)){// rtx_inner uses intensity variable of infinite value as a flag
    return (RGB){0,0,0};
  }
  return adjustColor(sphere.color, intensity);
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
// returns parameters t1, t2
double* instersectSphere(Point o, Point d, Sphere s){
  Point co = sub3(o, s.c);
  double a = dot(d,d);
  double b = 2 * dot(d, co);
  double c = dot(co, co) - (s.r * s.r);
  return quad(a,b,c);
}

// closest and visible sphere from the given Point o and its associated parameter t of intersection of sphere with ray from camera
// closest_s should be heap allocated by caller
// closest_t , the parameter associated to closest sphere, it also need to be heap allocated by caller and set to infinity
void closestSphere(Point o, Point d, double tmin, double tmax, int n_spheres, Sphere s_arr[], Sphere* closest_s, double* c_t){
  // initialize the the reference variable for comparision
  Sphere sphere;
  c_t[0] = INFINITY; // c_t having infinity value act as flag
  // iterate over all the spheres check if they intersect and find closest sphere based on parameter t
  for (int i = 0; i < n_spheres; i++){
    sphere = s_arr[i];
    double* t = instersectSphere(o, d, sphere);
    if (t && t[0] >= tmin && t[0] <= tmax && t[0] < c_t[0]){
      c_t[0] = t[0];
      closest_s[0] = sphere;
    }
    
    if (t && t[1] >= tmin && t[1] <= tmax && t[1] < c_t[0]){
      c_t[0] = t[1];
      closest_s[0] = sphere;
    }
    free(t);
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
double tli(Point normal, Point p, Point o, int s, int no_lights, Light l_arr[], int no_spheres, Sphere s_arr[]){
  Light light;
  Point l;
  double tmax;
  double c_intensity = 0;
  Sphere *shadow_s =  (Sphere*) malloc(sizeof(Sphere));
  double* shadow_t = (double*) malloc(sizeof(double));
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
    // check if light is obstructed by some other sphere in the case of point source and directional light
    closestSphere(p, l, 0.000001, tmax, no_spheres, s_arr, shadow_s, shadow_t);
    if (isinf(shadow_t[0])){
      // if not, add that light's contribution 
      c_intensity += specularI(o, p, normal, l, i, s);
      c_intensity += diffuseI(normal, l, i);
    }
  }
  free(shadow_s), free(shadow_t);
  return c_intensity;
}

// its return value intensity having infinite value is used as a flag, this is utilised in sphereColor
// it also computes sphere which is intersected by the ray from the camera and returns it indirectly thorough pointer assignment
double rtx_inner(Point o, Point d, double tmin, double tmax, int no_lights, int n_sphere, Sphere s_arr[], Light l_arr[], int limit,
               Sphere *sphere){
  double objintensity, otherintensity;
  objintensity = otherintensity = 0;
  double* t = (double*) malloc(sizeof(double));
  // find closest sphere from camera in the direction vector d
  closestSphere(o, d, tmin, tmax, n_sphere, s_arr, sphere, t);
  // check if sphere is returned by checking flag stored in parameter t
  if (!isinf(t[0])){
    Point p = add3(o, (scale(t[0], d)));
    Point normal = sub3(p, sphere->c);
    // find out how much it is illuminated directly i.e. object's intensity
    objintensity = tli(normal, p, o, sphere->s, no_lights, l_arr, n_sphere, s_arr);
    // check if object is reflective and recursion limit is reached
    if (sphere->s > 0 && limit > 0){
      // find out reflected ray
      Point reflected = reflectedRay(
        scale(1 / norm(normal), normal),
        scale(-1, d)
      );
      // then compute new intensity contributed by other object 
      Sphere *newsphere = (Sphere*) malloc(sizeof(Sphere));
      otherintensity = rtx_inner(p, reflected, 0.00001, INFINITY, no_lights, n_sphere, s_arr, l_arr, limit-1, newsphere);
      // check if intensity is valid using flag value of inf
      if (!isinf(otherintensity)){
        // if valid then take weighted sum of intensity which will be final intensity
        objintensity = objintensity * (1- sphere->rfl) + otherintensity*sphere->rfl;
      }
      free(newsphere), free(t);
      return objintensity;
    }
  }
  free(t);
  return INFINITY;
}


RGB rtx(Point o, Point d, double tmin, double tmax, int no_lights, int n_sphere, Sphere s_arr[], Light l_arr[]){
  Sphere* sphere = (Sphere*) malloc(sizeof(Sphere));
  double intensity = rtx_inner(o, d, tmin, tmax, no_lights, n_sphere, s_arr, l_arr, 3, sphere);
  RGB rgb = sphereColor(sphere[0], intensity);
  free(sphere);
  return rgb;
}
