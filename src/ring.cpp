#include "ring.h"

float steps_to_ring(const ring &hoop, const ray &light_ray) {
  const point &Q = hoop.center;
  const vector &n = hoop.normal;
  const point &R = light_ray.origin;
  const vector &d = light_ray.direction;

  float facing = dot(d, n);
  if (facing == 0) {
    return -1;
  }

  float t = dot(Q - R, n) / facing;
  if (t <= 0) {
    return -1;
  }

  float distance = (light_ray.point_at(t) - Q).length();
  if (distance < hoop.inner_radius || distance > hoop.outer_radius) {
    return -1;
  }

  return t;
}
