#ifndef OBJECT
#define OBJECT

#include "color.h"
#include "vector.h"
#include <string>

struct ray;

struct object {
  std::string name;
  point origin;
  vector from_origin;
  color base_color;

  // Pure virtual because it's likely each type of object will have a
  // different formula.
  virtual float getArea() = 0;

  virtual float hit(const ray &r) const = 0;
  virtual vector normal_at(point P) const = 0;
};

#endif
