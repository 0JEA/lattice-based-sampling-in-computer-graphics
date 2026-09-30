#include "camera.h"
#include "color.h"
#include "image.h"
#include "input.h"
#include "ppm.h"
#include "ray.h"
#include "vector.h"
#include <iostream>
#include <string>

#define MSG_WELCOME "Ray Traced PPM File Creator"

color blend(color from, color to, float strength) {
  return from + strength * (to - from);
}

// using: https://tailwindcolor.tools/color-library
color ray_color(const ray &r) {
  color pumpkin = color(255, 117, 24) / 255;
  color tangerine = color(255, 153, 102) / 255;
  color emerald = color(80, 200, 120) / 255;
  color gunmetal = color(42, 52, 57) / 255;
  color white = color(255, 255, 255) / 255;
  color black = color(0, 0, 0) / 255;

  float height = unit_vector(r.direction).y();
  float width = unit_vector(r.direction).x();

  if (r.direction.x() >= 0.75) {
    float start = 0.75;
    float end = 1.5;
    float across = (r.direction.x() - start) / (end - start);
    return blend(black, white, across);
  }

  if (r.direction.y() < 0) {
    float floor_height = 2;
    float steps_to_floor = (floor_height - r.origin.y()) / r.direction.y();
    point landing_spot = r.point_at(steps_to_floor);

    int tile_across = int(std::floor(landing_spot.x()));
    int tile_deep = int(std::floor(landing_spot.z()));

    bool is_even_tile = (tile_across + tile_deep) % 2 == 0;
    return is_even_tile ? white : gunmetal;
  }

  float radius = 0.5;
  if (width * width + height * height <= radius * radius) {
    return pumpkin;
  }

  return blend(emerald, pumpkin, (height + 1) / 2);
}

// Fires one ray per pixel of the camera and returns the finished image.
image render(const camera &cam) {
  image picture{cam.get_resolution_width(), cam.get_resolution_height(), {}};
  picture.pixels.reserve(picture.width * picture.height);

  for (int row = 0; row < picture.height; ++row) {
    for (int column = 0; column < picture.width; ++column) {
      picture.pixels.push_back(ray_color(cam.ray_through_pixel(column, row)));
    }
  }
  return picture;
}

int main() {
  std::cout << MSG_WELCOME << "\n";
  int width = get_input<int>(MSG_WIDTH);
  int height = get_input<int>(MSG_HEIGHT);
  std::string filename = get_input<std::string>(MSG_FILENAME);

  camera cam(width, height);

  image picture = render(cam);
  if (!file_create_ppm(filename, picture)) {
    return 1;
  }
  std::cout << MSG_FILECREATED + filename + FILE_EXTENSION << "\n";
}
