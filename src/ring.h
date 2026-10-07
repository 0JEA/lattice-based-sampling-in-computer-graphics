#ifndef RING_H
#define RING_H

#include "ray.h"
#include "vector.h"

struct ring {
  point center;
  vector normal;
  float inner_radius;
  float outer_radius;
};

float steps_to_ring(const ring &hoop, const ray &light_ray);

#endif
