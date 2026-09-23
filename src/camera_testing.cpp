#include "camera.h"
#include "color.h"
#include "input.h"
#include "ppm.h"
#include "ray.h"
#include "vector.h"
#include <iostream>
#include <string>

#define MSG_WELCOME "Ray Traced PPM File Creator"

color ray_color(const ray &r) {
  vector unit_direction = unit_vector(r.direction);
  auto a = 0.5 * (unit_direction[1] + 1.0);
  return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

void ppm_write_pixels(std::ostream &output, int width, int height) {
  camera cam(width, height);

  for (int row = 0; row < height; ++row) {
    for (int column = 0; column < width; ++column) {
      color pixel = ray_color(cam.get_ray(column, row));
      write_color(output, pixel[0], pixel[1], pixel[2]);
    }
  }
}

int main() {
  std::cout << MSG_WELCOME << "\n";

  int width = get_input<int>(MSG_WIDTH);
  std::string filename = get_input<std::string>(MSG_FILENAME);

  int height = camera::height_for(width);

  file_create_ppm(width, height, filename);
  std::cout << MSG_FILECREATED + filename + FILE_EXTENSION << "\n";
}
