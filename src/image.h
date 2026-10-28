#ifndef IMAGE_H
#define IMAGE_H

#include "color.h"

#include <vector>

struct image {
  int width;
  int height;
  std::vector<color> pixels;
};

#endif
