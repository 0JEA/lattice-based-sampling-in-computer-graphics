#ifndef COLOR_H
#define COLOR_H

#include "vector.h"
#include <iostream>

// Using my own vector class, renaming it to color to avoid confusion in other
// files where its used. Perhaps should instead make a color class that 
// inherits from my vector class?
using color = vector;

// ISSUE:
// HUH PROVE IT
#define MAX_RGB 255 // 8 bits per channel

void write_color(std::ostream &output, float red, float green, float blue);

#endif
