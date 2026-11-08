// Rudametry adapation of main.cc from Chapter 2, 2.1 of
// https://raytracing.github.io/books/RayTracingInOneWeekend.html
// This was made to quickly get a sense of how this can be done fast.
// A better implementation will later be created that includes header files and
// proper error checking and docs.

#include "color.h"
#include "input.h"
#include "ppm.h"
#include <iostream>
#include <string>
#include <vector>

constexpr char MSG_WELCOME[] = "Basic PPM File Creator";

// Also basically copied from the ray tracing in a weekend.
//
// Do note the width > 1 ? ... : 0
// This was to fix a bug I found that would happen when the height or width
// was only 1. The issue is division by zero.
ppm gradient(int width, int height) {
  ppm picture{.pixel_columns = width, .pixel_rows = height, .pixels = {}};

  // BUG: if width or height are ever negative a ton of space will be allocated 
  // as it overflows.
  picture.pixels.reserve(width * height);

  for (int row = 0; row < height; ++row) {
    for (int column = 0; column < width; ++column) {
      float red = width > 1 ? float(column) / (width - 1) : 0.0f;
      float green = height > 1 ? float(row) / (height - 1) : 0.0f;
      float blue = 0.0f;

      picture.pixels.push_back(color(red, green, blue));
    }
  }
  return picture;
}

int main() {
  std::cout << MSG_WELCOME << "\n";

  int width = get_input<int>(MSG_WIDTH);
  int height = get_input<int>(MSG_HEIGHT);
  std::string filename = get_input<std::string>(MSG_FILENAME);

  ppm picture = gradient(width, height);
  if (!file_create_ppm(filename, picture)) {
    return 1;
  }
  std::cout << MSG_FILECREATED + filename + FILE_EXTENSION << "\n";
}
