#include "sphere.h"
#include "vector.h"
#include <cmath>

sphere::sphere() {
  this->name = "";
  this->radius = 1;
  this->origin = point(0, 0, 0);
};

sphere::sphere(float radius) {
  this->name = "";
  this->radius = radius;
  this->origin = point(0, 0, 0);
};

sphere::sphere(float radius, point origin) {
  this->name = "";
  this->radius = radius;
  this->origin = origin;
};

sphere::sphere(float radius, point origin, color base_color) {
  this->name = "";
  this->radius = radius;
  this->origin = origin;
  this->base_color = base_color;
}

// ISSUE:
// Auto override because h file does and thats ovverirding object?
float sphere::getArea() { return (4 * PI) * (radius * radius); };

float sphere::hit(const ray &r) const {
  vector OC = origin - r.origin;
  float a = dot(r.direction, r.direction);
  float b = -2.0 * dot(r.direction, OC);
  float c = dot(OC, OC) - radius * radius;
  float discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
    return -1;
  }

  return (-b - std::sqrt(discriminant)) / (2 * a);
}

vector sphere::normal_at(point P) const { return (P - origin) / radius; }
