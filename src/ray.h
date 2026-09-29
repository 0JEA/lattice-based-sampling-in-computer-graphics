#ifndef RAY_H
#define RAY_H

// Lets avoid the short "vec" name that was used, I'd prefer verbose.
#include "vector.h"

/*
I'm not sure why in the exmaple online they way over complicated the ray
type, by making it a class with private data members and getters. Perhaps ill
run into the reason further in. For now this is much more simple.
 */

// point_at returns the point a distance t along the ray.
struct ray {
  vector origin;
  vector direction;
  vector point_at(float t) const { return origin + t * direction; }
};

#endif
