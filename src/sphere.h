#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"
#include "vector.h"

struct sphere {
  point center;
  float radius;
};

bool sphere_crossings(const sphere &ball, const ray &light_ray, float &t_minus,
                      float &t_plus);

float steps_to_sphere(const sphere &ball, const ray &light_ray);

float steps_to_bowl(const sphere &bowl, const ray &light_ray);

#endif
