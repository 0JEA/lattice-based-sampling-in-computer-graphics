#ifndef COLOR_H
#define COLOR_H

#include "vector.h"

#include <iostream>

using color = vector;

#define MAX_RGB 255 // 8 bits per channel

void write_color(std::ostream &output, float red, float green, float blue);

#endif
