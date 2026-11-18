#ifndef SCENE_GLSL
#define SCENE_GLSL

#include "shapes.glsl"

const vec3 white = vec3(255, 255, 255) / 255.0;
const vec3 red = vec3(255, 0, 0) / 255.0;
const vec3 green = vec3(0, 255, 0) / 255.0;
const vec3 emerald = vec3(80, 200, 120) / 255.0;
const vec3 pumpkin = vec3(255, 117, 24) / 255.0;
const vec3 violet = vec3(139, 92, 246) / 255.0;
const vec3 lapis_lazuli = vec3(38, 97, 156) / 255.0;
const vec3 background = vec3(20, 20, 20) / 255.0;

const float light_intensity = 120.0;
const float ambient_light = 0.08;
const float shadow_epsilon = 0.001;

const vec3 light_position = vec3(0.0, 14.8, 3.5);
const float light_size = 8.0;

const vec3 camera_position = vec3(0.0, 7.0, 25.0);
const float camera_focal_length = 1.0;
const float camera_vertical_fov = 60.0;

const int PLANE_COUNT = 5;
const Plane planes[PLANE_COUNT] = Plane[PLANE_COUNT](
    Plane(vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), white),
    Plane(vec3(0.0, 15.0, 0.0), vec3(0.0, -1.0, 0.0), white),
    Plane(vec3(0.0, 0.0, -10.0), vec3(0.0, 0.0, 1.0), white),
    Plane(vec3(-10.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0), red),
    Plane(vec3(10.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0), green));

const int SPHERE_COUNT = 4;
const Sphere spheres[SPHERE_COUNT] = Sphere[SPHERE_COUNT](
    Sphere(4.0, vec3(-4.0, 4.0, -6.0), emerald),
    Sphere(2.0, vec3(3.0, 3.0, 7.0), pumpkin),
    Sphere(0.35, vec3(-2.1, 11.1, 1.0), violet),
    Sphere(1.1, vec3(5.0, 8.0, 1.5), lapis_lazuli));

#endif
