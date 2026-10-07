#include "sphere.h"

#include <cmath>

bool sphere_crossings(const sphere &ball, const ray &light_ray, float &t_minus,
                      float &t_plus) {
  const point &C = ball.center;
  const float r = ball.radius;
  const point &R = light_ray.origin;
  const vector &d = light_ray.direction;

  vector O = R - C;

  float a = dot(d, d);
  float b = 2.0f * dot(d, O);
  float c = dot(O, O) - r * r;

  float discriminant = b * b - 4.0f * a * c;
  if (discriminant < 0) {
    return false;
  }

  float root = std::sqrt(discriminant);
  t_minus = (-b - root) / (2.0f * a);
  t_plus = (-b + root) / (2.0f * a);
  return true;
}

float steps_to_sphere(const sphere &ball, const ray &light_ray) {
  float t_minus, t_plus;
  if (!sphere_crossings(ball, light_ray, t_minus, t_plus)) {
    return -1;
  }

  if (t_minus > 0) {
    return t_minus;
  }

  if (t_plus > 0) {
    return t_plus;
  }

  return -1;
}

float steps_to_bowl(const sphere &bowl, const ray &light_ray) {
  float t_minus, t_plus;
  if (!sphere_crossings(bowl, light_ray, t_minus, t_plus)) {
    return -1;
  }

  float cut_height = bowl.center.y();

  if (t_minus > 0 && light_ray.point_at(t_minus).y() <= cut_height) {
    return t_minus;
  }

  if (t_plus > 0 && light_ray.point_at(t_plus).y() <= cut_height) {
    return t_plus;
  }

  return -1;
}
