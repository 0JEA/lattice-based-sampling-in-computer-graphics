#ifndef SHAPES_GLSL
#define SHAPES_GLSL

struct Ray {
  vec3 origin;
  vec3 direction;
};

struct Sphere {
  float radius;
  vec3 origin;
  vec3 base_color;
};

struct Plane {
  vec3 origin;
  vec3 normal;
  vec3 base_color;
};

vec3 point_at(Ray r, float t) {
  return r.origin + t * r.direction;
}

float hit_sphere(Sphere s, Ray r) {
  vec3 OC = s.origin - r.origin;
  float a = dot(r.direction, r.direction);
  float b = -2.0 * dot(r.direction, OC);
  float c = dot(OC, OC) - s.radius * s.radius;
  float discriminant = b * b - 4.0 * a * c;
  if (discriminant < 0.0) {
    return -1.0;
  }

  return (-b - sqrt(discriminant)) / (2.0 * a);
}

vec3 sphere_normal_at(Sphere s, vec3 P) {
  return (P - s.origin) / s.radius;
}

float hit_plane(Plane p, Ray r) {
  float denominator = dot(r.direction, p.normal);
  if (abs(denominator) < 1e-6) {
    return -1.0;
  }

  return dot(p.origin - r.origin, p.normal) / denominator;
}

#endif
