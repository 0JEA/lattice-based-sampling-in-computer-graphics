#ifndef PLANE_H
#define PLANE_H

#include "object.h"
#include "ray.h"
#include "vector.h"

struct plane : object {
  vector normal;

  plane(point origin, vector normal, color base_color);

  float getArea() override;
  float hit(const ray &r) const override;
  vector normal_at(point P) const override;
};

#endif
