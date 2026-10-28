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

color blend(color from, color to, float strength) {
  return from + strength * (to - from);
}

color shade(color base, vector n, point P, const ray &r,
            const std::vector<object *> &scene) {
  if (dot(r.direction, n) > 0) {
    n = -n;
  }

  point light_position(-3, 4, -2);
  float light_intensity = 25.0f;

  ray shadow{P, light_position - P};
  for (const object *thing : scene) {
    float t = thing->hit(shadow);
    if (t > 0.001f && t < 1.0f) {
      return blend(color(0, 0, 0), base, 0.2f);
    }
  }

  vector to_light = light_position - P;
  float distance = to_light.length();
  vector toward_light = to_light / distance;

  float falloff = light_intensity / (distance * distance);
  float brightness = std::fmax(0.0f, dot(n, toward_light)) * falloff;
  brightness = std::fmin(1.0f, brightness);
  return blend(color(0, 0, 0), base, 0.2f + 0.8f * brightness);
}

// using: https://tailwindcolor.tools/color-library
color ray_color(const ray &r, const std::vector<object *> &scene) {

  // abstracting this out would be a good idea.

  color pumpkin = color(255, 117, 24) / 255;
  color tangerine = color(255, 153, 102) / 255;
  color emerald = color(80, 200, 120) / 255;
  color gunmetal = color(42, 52, 57) / 255;
  color white = color(255, 255, 255) / 255;
  color red = color(255, 0, 0) / 255;
  color violet = color(139, 92, 246) / 255;
  color sky_blue = color(56, 189, 248) / 255;

  float width = unit_vector(r.direction).x();
  float height = unit_vector(r.direction).y();
  float depth = unit_vector(r.direction).z();

  float nearest = INFINITY;
  const object *closest = nullptr;
  for (const object *thing : scene) {
    float t = thing->hit(r);
    if (t > 0 && t < nearest) {
      nearest = t;
      closest = thing;
    }
  }

  if (closest) {
    point P = r.point_at(nearest);
    return shade(closest->base_color, closest->normal_at(P), P, r, scene);
  }

  return emerald;
}

image render(const camera &cam, const std::vector<object *> &scene) {
  image picture{cam.get_resolution_width(), cam.get_resolution_height(), {}};
  picture.pixels.reserve(picture.width * picture.height);
  // INFO: Reserve is just to allocate the space for the vector.

  for (int row = 0; row < picture.height; ++row) {
    for (int column = 0; column < picture.width; ++column) {
      picture.pixels.push_back(
          ray_color(cam.ray_through_pixel(column, row), scene));
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
  int width = 1200;
  int height = 1200;
  std::string filename = "meow";

  camera cam(width, height);
  cam.move_to(vector(0, 1, 0));
  cam.set_vertical_fov(90);

  sphere red_ball = sphere(0.3, point(0.5, 0.5, -6), color(255, 0, 0) / 255);
  sphere violet_ball = sphere(2, point(-2, 1.5, -6), color(139, 92, 246) / 255);

  std::vector<object *> scene = {&red_ball, &violet_ball};

  image picture = render(cam, scene);
  if (!file_create_ppm(filename, picture)) {
    return 1;
  }
  std::cout << MSG_FILECREATED + filename + FILE_EXTENSION << "\n";
}
