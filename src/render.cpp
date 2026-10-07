#include "camera.h"
#include "color.h"
#include "image.h"
#include "input.h"
#include "ppm.h"
#include "ray.h"
#include "ring.h"
#include "sphere.h"
#include "vector.h"
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <string>

#define MSG_WELCOME "Ray Traced PPM File Creator"

color blend(color from, color to, float strength) {
  return from + strength * (to - from);
}

color shade(color base, vector n, const ray &r) {
  if (dot(r.direction, n) > 0) {
    n = -n;
  }

  vector toward_light = unit_vector(vector(-1, 1, 1));
  float brightness = std::fmax(0.0f, dot(n, toward_light));
  return blend(color(0, 0, 0), base, 0.2f + 0.8f * brightness);
}

// using: https://tailwindcolor.tools/color-library
color ray_color(const ray &r) {
  color pumpkin = color(255, 117, 24) / 255;
  color tangerine = color(255, 153, 102) / 255;
  color emerald = color(80, 200, 120) / 255;
  color gunmetal = color(42, 52, 57) / 255;
  color white = color(255, 255, 255) / 255;
  color red = color(255, 0, 0) / 255;
  color violet = color(139, 92, 246) / 255;
  color sky_blue = color(56, 189, 248) / 255;

  float height = unit_vector(r.direction).y();
  float width = unit_vector(r.direction).x();

  sphere ball{point(0, 1, -3), 1};
  sphere bowl{point(0, 3, -3), 0.8f};
  ring hoop{point(0, 3, -3), unit_vector(vector(0.3f, 1, 0.5f)), 1.25f, 1.4f};

  float steps_to_ball = steps_to_sphere(ball, r);
  float steps_to_the_bowl = steps_to_bowl(bowl, r);
  float steps_to_hoop = steps_to_ring(hoop, r);

  float floor_height = 0;
  float steps_to_floor = -1;
  if (r.direction.y() != 0) {
    steps_to_floor = (floor_height - r.origin.y()) / r.direction.y();
  }

  float nearest = INFINITY;
  for (float t :
       {steps_to_ball, steps_to_the_bowl, steps_to_hoop, steps_to_floor}) {
    if (t > 0 && t < nearest) {
      nearest = t;
    }
  }

  if (nearest == steps_to_ball) {
    point P = r.point_at(nearest);
    return shade(emerald, (P - ball.center) / ball.radius, r);
  }

  if (nearest == steps_to_the_bowl) {
    point P = r.point_at(nearest);
    return shade(violet, (P - bowl.center) / bowl.radius, r);
  }

  if (nearest == steps_to_hoop) {
    return shade(sky_blue, hoop.normal, r);
  }

  if (nearest == steps_to_floor) {
    point landing_spot = r.point_at(steps_to_floor);
    int tile_across = int(std::floor(landing_spot.x()));
    int tile_deep = int(std::floor(landing_spot.z()));
    bool is_even_tile = (tile_across + tile_deep) % 2 == 0;

    bool hit_top = r.direction.y() < 0;
    if (hit_top) {
      if (tile_deep == -5) {
        return red;
      }
      return is_even_tile ? white : gunmetal;
    }

    return is_even_tile ? gunmetal : tangerine;
  }

  float radius = 0.5;
  if (width * width + height * height <= radius * radius) {
    return pumpkin;
  }

  return blend(emerald, pumpkin, (height + 1) / 2);
}

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
  cam.move_to(vector(0, 1, 0));
  cam.set_vertical_fov(90);

  image picture = render(cam);
  if (!file_create_ppm(filename, picture)) {
    return 1;
  }
  std::cout << MSG_FILECREATED + filename + FILE_EXTENSION << "\n";
}
