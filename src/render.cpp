#include "camera.h"
#include "color.h"
#include "input.h"
#include "ppm.h"
#include "ray.h"
#include "vector.h"
#include <iostream>
#include <string>
#include <vector>

#define MSG_WELCOME "Ray Traced PPM File Creator"

// The color a ray sees. For now that's only the sky: a blend from white at
// the bottom to blue straight up, based on how far up the ray points.
color ray_color(const ray &r) {
  const color horizon_color(1.0f, 0.0f, 0.0f);
  const color zenith_color(0.0f, 0.0f, 1.0f);

  vector unit_direction = unit_vector(r.direction);

  // y() runs from -1 (straight down) to 1 (straight up).
  // Shift it to run from 0 to 1 so it can be used as a blend amount.
  float upness = 0.5f * (unit_direction.y() + 1.0f);

  return (1.0f - upness) * horizon_color + upness * zenith_color;
}

// Fires one ray per pixel and returns the colors, row by row from the
// top-left.
std::vector<color> render(int width, int height) {
  camera cam(width, height);
  std::vector<color> pixels;

  for (int row = 0; row < height; ++row) {
    for (int column = 0; column < width; ++column) {
      pixels.push_back(ray_color(cam.ray_through_pixel(column, row)));
    }
  }
  return pixels;
}

int main() {
  std::cout << MSG_WELCOME << "\n";

  int width = get_input<int>(MSG_WIDTH);
  std::string filename = get_input<std::string>(MSG_FILENAME);

  int height = width * 3 / 4; // 4:3 image

  if (!file_create_ppm(width, height, filename, render(width, height))) {
    return 1;
  }
  std::cout << MSG_FILECREATED + filename + FILE_EXTENSION << "\n";
}
