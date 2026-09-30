#ifndef IMAGE_H
#define IMAGE_H

#include "color.h"

#include <vector>

// A finished picture: its size and its pixels, row by row from the top-left.
// Keeping the size with the pixels means the two can't disagree.
struct image {
  int width;
  int height;
  std::vector<color> pixels;
};

#endif
