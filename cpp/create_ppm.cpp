// Rudametry adapation of main.cc from Chapter 2, 2.1 of
// https://raytracing.github.io/books/RayTracingInOneWeekend.html
// This was made to quickly get a sense of how this can be done fast.
// A better implementation will later be created that includes header files and
// proper error checking and docs.

#include "input.h"
#include "ppm.h"
#include <iostream>
#include <string>

#define MSG_WELCOME "Basic PPM File Creator"

// Copied directly from the ray tracing in a weekend. I intend to explore
// whats happening here in more detail at a later time.
//
// Seems to be related to rgb of 255 vs 256.
void write_color(std::ostream &output, double red, double green, double blue) {
  int red_byte = int(255.999 * red);
  int green_byte = int(255.999 * green);
  int blue_byte = int(255.999 * blue);

  output << red_byte << ' ' << green_byte << ' ' << blue_byte << '\n';
}

// Also basically copied from the ray tracing in a weekend.
//
// Do note the width > 1 ? ... : 0
// This was to fix a bug I found that would happen when the height or width
// was only 1. The issue is division by zero.
void ppm_write_pixels(std::ostream &output, int width, int height) {
  for (int row = 0; row < height; ++row) {
    for (int column = 0; column < width; ++column) {
      double red = width > 1 ? double(column) / (width - 1) : 0;
      double green = height > 1 ? double(row) / (height - 1) : 0;
      double blue = 0.0;

      write_color(output, red, green, blue);
    }
  }
}

int main() {
  std::cout << MSG_WELCOME << "\n";

  int width = get_input<int>(MSG_WIDTH);
  int height = get_input<int>(MSG_HEIGHT);
  std::string filename = get_input<std::string>(MSG_FILENAME);

  file_create_ppm(width, height, filename);
  std::cout << MSG_FILECREATED + filename + FILE_EXTENSION << "\n";
}
