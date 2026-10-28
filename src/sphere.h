#ifndef SPHERE_H
#define SPHERE_H
#define PI 3.14159265358979323846

#include "object.h"
#include "ray.h"
#include "vector.h"
#include <string>

struct sphere : object {
  float radius;

  sphere();
  sphere(float radius);
  sphere(float radius, point origin);
  sphere(float radius, point origin, color base_color);

  float getArea() override;
  float hit(const ray &r) const override;
  vector normal_at(point P) const override;
};

#endif
