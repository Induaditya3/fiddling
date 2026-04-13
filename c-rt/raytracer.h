#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <math.h>
#include <stdlib.h>
/* Various Custom Struct */

// Point
// it will also serve as 3D vector
typedef struct {
  double x,y,z;
}Point;

// Color
typedef struct {
  short int r,g,b;
}RGB;

/* Restructuring code for supporting triangle */

typedef struct {
  Point a, b, c; // coordinates of location of vertices
  // Point n; // normal
  // double gama, beta; // barycentric coordinate at a intersection point
  int s; // shininess
  double rfl;
  RGB color;
}Triangle;

// Sphere
typedef struct {
  double r; // radius
  Point c; // center coordinate
  RGB color; // color of the sphere
  int s; // shininess of the sphere - ranges 0 on up , -ve means not shiny
  double rfl; // reflectiveness of the sphere - ranges from 0 to 1
}Sphere;

typedef struct {
  char k; // kind of hittable object
  union {
    Sphere sph; // sphere
    Triangle tri; // triangle
  };
}Hittable;

// Light
typedef struct {
  char k; // kind of light - 'a' means ambient, 'd' means directional light, 'p' means point source
  double i; // intensity of light - sum all intensity of all light sources should be 1
  Point v; // light vector
}Light;

/* Vector Operation */

// 3D vector subtraction
Point sub3(Point p1, Point p2);
// 3D vector addition
Point add3(Point p1, Point p2);
// scale a 3D vector by constant factor
Point scale(double k, Point p2);
// dot product of two 3D vector
double dot(Point p1, Point p2);
// cross product
Point cross(Point p1, Point p2);
// norm of a 3D vector
double norm(Point p);
Point reflectedRay(Point ncap, Point l);
/* Solution of Quadratic Equation */
// modifies pointer two roots if solution is real else INF
void quad(double a, double b, double c, double roots[]);
/* Color related functions */
// clamping color channels value to 0 to 255
short int clamp(double x);
// adjust brightness of color
RGB adjustColor(RGB rgb, double intensity);
// return sphere's color if its actually sphere else return background color
RGB surfaceColor(Hittable surface, double intensity);
/* Coordinate transformataion */
// transforming coordinate from graphics window to world coordinate (scene)
// setting width vw and vh of width and height of viewport respectively
// z coordinate of viewport is distance from camera(ie origin)

Point g_to_viewport(int gx, int gy, int gw, int gh);
/* Intersection of sphere */
// modifies t for storing  parameters t1, t2
void instersectSphere(Point o, Point d, Sphere s, double t[]);
// assign parameter t a finite value if triangle is intersected otherwise infinite value
// o is the origin i.e. from where ray originates
// direction is the direction of the ray
// tmin and tmax is the minimum and the maximum range of triangle
// tgb is the array on the storing computed value of t, g and b, t stores INF if intersection doesn't fit the conditions 
void instersectTriangle(Point o, Point direction, Triangle tri, double tmin, double tmax, double tgb[]);

// closest and visible Hittable surface from the given Point o and its associated parameter t of intersection with ray from camera
// closest_s should be heap allocated by caller
// closest_t , the parameter associated to closest sphere, it also need to be heap allocated by caller and set to infinity
void closestHittable(Point o, Point d, double tmin, double tmax, int n_spheres, Hittable s_arr[], Hittable* closest_s, double* c_t);
// intensity of light contributed by diffuse reflection
double diffuseI(Point normal, Point l, double i);
// intensity of light contributed by specular reflection
double specularI(Point o, Point p, Point normal, Point l, double i, int s);
// total light intensity after reflection
double tli(Point normal, Point p, Point o, int s, int no_lights, Light l_arr[], int no_spheres, Hittable s_arr[]);
double rtx_inner(Point o, Point d, double tmin, double tmax, int no_lights, int n_sphere, Hittable s_arr[], Light l_arr[], int limit,
               Hittable *surface);
RGB rtx(Point o, Point d, double tmin, double tmax, int no_lights, int n_sphere, Hittable s_arr[], Light l_arr[]);

#endif
