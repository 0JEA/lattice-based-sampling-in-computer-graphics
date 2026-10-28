#include "camera.h"
#include "color.h"
#include "image.h"
#include "input.h"
#include "ppm.h"
#include "ray.h"
#include "sphere.h"
#include "vector.h"
#include <cmath>
#include <iostream>
#include <math.h>
#include <string>

#define MSG_WELCOME "Ray Traced PPM File Creator"

constexpr float LIGHT_INTENSITY = 25.0f;
constexpr float AMBIENT = 0.2f;
constexpr float SHADOW_EPSILON = 0.001f;
constexpr int AA_GRID = 4;

// abstracting this out would be a good idea.
const color pumpkin = color(255, 117, 24) / 255;
const color tangerine = color(255, 153, 102) / 255;
const color emerald = color(80, 200, 120) / 255;
const color gunmetal = color(42, 52, 57) / 255;
const color white = color(255, 255, 255) / 255;
const color red = color(255, 0, 0) / 255;
const color violet = color(139, 92, 246) / 255;
const color sky_blue = color(56, 189, 248) / 255;

color blend(color from, color to, float strength) {
  return from + strength * (to - from);
}

color shade(color base, vector n, point P, const ray &r,
            const std::vector<object *> &scene) {
  if (dot(r.direction, n) > 0) {
    n = -n;
  }

  point light_position(-3, 4, -2);

  ray shadow{P, light_position - P};
  for (const object *thing : scene) {
    float t = thing->hit(shadow);
    if (t > SHADOW_EPSILON && t < 1.0f) {
      return blend(color(0, 0, 0), base, AMBIENT);
    }
  }

  vector to_light = light_position - P;
  float distance = to_light.length();
  vector toward_light = to_light / distance;

  float falloff = LIGHT_INTENSITY / (distance * distance);
  float brightness = std::fmax(0.0f, dot(n, toward_light)) * falloff;
  brightness = std::fmin(1.0f, brightness);
  return blend(color(0, 0, 0), base, AMBIENT + (1 - AMBIENT) * brightness);
}

struct hit_record {
  float t = INFINITY;
  const object *thing = nullptr;
};

hit_record closest_hit(const ray &r, const std::vector<object *> &scene) {
  hit_record closest;
  for (const object *thing : scene) {
    float t = thing->hit(r);
    if (t > 0 && t < closest.t) {
      closest = {t, thing};
    }
  }
  return closest;
}

// using: https://tailwindcolor.tools/color-library
color ray_color(const ray &r, const std::vector<object *> &scene) {

  hit_record hit = closest_hit(r, scene);
  if (hit.thing) {
    point P = r.point_at(hit.t);
    return shade(hit.thing->base_color, hit.thing->normal_at(P), P, r, scene);
  }

  return emerald;
}

image render(const camera &cam, const std::vector<object *> &scene,
             int grid = 1) {
  image picture{cam.get_resolution_width(), cam.get_resolution_height(), {}};
  picture.pixels.reserve(picture.width * picture.height);
  // INFO: Reserve is just to allocate the space for the vector.

  for (int row = 0; row < picture.height; ++row) {
    for (int column = 0; column < picture.width; ++column) {
      // picture.pixels.push_back(
      //     ray_color(cam.ray_through_pixel(column, row), scene));

      // INFO: now for AA!
      // will need to abstract this out for sure.
      // its soo expensive right now.

      // starting at black, then append each rays result
      color total(0, 0, 0);
      for (int i = 0; i < grid; ++i) {
        for (int j = 0; j < grid; ++j) {
          float offset_x = (j + 0.5f) / grid - 0.5f;
          float offset_y = (i + 0.5f) / grid - 0.5f;
          ray r = cam.ray_through_pixel(column, row, offset_x, offset_y);
          total = total + ray_color(r, scene);
        }
      }
      picture.pixels.push_back(total / (grid * grid));
      // INFO: returning the averaged result from the offsets
      //       for a now aliased pixel!
      // INFO: push_back just adds to the end of the vector.
    }
  }
  return picture;
}

void getInputs() {}

int main() {
  std::cout << MSG_WELCOME << "\n";

  // int width = get_input<int>(MSG_WIDTH);
  // int height = get_input<int>(MSG_HEIGHT);
  // std::string filename = get_input<std::string>(MSG_FILENAME);

  int width = 400;
  int height = 400;
  std::string filename = "meow";

  sphere red_ball = sphere(0.3, point(0, 0, 0), red);
  // sphere violet_ball = sphere(2, point(-2, 1.5, -6), violet);

  camera cam(width, height);
  cam.move_to(vector(0.3, 0, 0.8));
  cam.set_vertical_fov(90);
  std::vector<object *> scene = {&red_ball};

  // std::vector<object *> scene = {&red_ball, &violet_ball};

  image picture = render(cam, scene, AA_GRID);
  if (!file_create_ppm(filename, picture)) {
    return 1;
  }
  std::cout << MSG_FILECREATED + filename + FILE_EXTENSION << "\n";
}
