#include "plane.h"
#include <cmath>

plane::plane(point origin, vector normal, color base_color) {
  this->name = "";
  this->origin = origin;
  this->normal = unit_vector(normal);
  this->base_color = base_color;
}

float plane::getArea() { return 0; }

float plane::hit(const ray &r) const {
  float denominator = dot(r.direction, normal);
  if (std::fabs(denominator) < 1e-6f) {
    return -1;
  }

  return dot(origin - r.origin, normal) / denominator;
}

vector plane::normal_at(point) const { return normal; }
