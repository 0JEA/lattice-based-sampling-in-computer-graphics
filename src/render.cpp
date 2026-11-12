#include "camera.h"
#include "input.h"
#include "object.h"
#include "plane.h"
#include "ppm.h"
#include "ray.h"
#include "sphere.h"
#include "vector.h"
#include <cmath>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#define MSG_WELCOME "Ray Traced PPM File Creator"

// using: https://tailwindcolor.tools/color-library
// abstracting this out would be a good idea.
const color pumpkin = color(255, 117, 24) / 255;
const color tangerine = color(255, 153, 102) / 255;
const color emerald = color(80, 200, 120) / 255;
const color gunmetal = color(42, 52, 57) / 255;
const color white = color(255, 255, 255) / 255;
const color red = color(255, 0, 0) / 255;
const color violet = color(139, 92, 246) / 255;
const color sky_blue = color(56, 189, 248) / 255;
const color lapis_lazuli = color(38, 97, 156) / 255;
const color cherry_blossom = color(255, 183, 197) / 255;
const color watermelon = color(253, 70, 89) / 255;
const color papaya = color(255, 159, 91) / 255;

struct light{
  point position;
  float size {0.0f};
  float intensity {25.0f};
};

struct scene{
  float light_intensity {25.0f};
  float ambient_light {0.125f};
  float shadow_epsilon {0.001f};
  int aa_grid = {1};
  int shadow_samples_per_side {4};
  std::vector<object *> objects;
  light sun;
};

color blend(color from, color to, float strength) {
  return from + strength * (to - from);
}

color shade(color base, vector n, point P, const ray &r,
            scene &active_scene) {

  if (dot(r.direction, n) > 0) {
    n = -n;
  }


  int samples_per_side = active_scene.shadow_samples_per_side;
  int unblocked_rays = 0;

  for (int height_step = 0; height_step < samples_per_side; ++height_step) {
    for (int around_step = 0; around_step < samples_per_side; ++around_step) {
      float height_fraction = (height_step + 0.5f) / samples_per_side;
      float around_fraction = (around_step + 0.5f) / samples_per_side;

      float height = 1.0f - 2.0f * height_fraction;
      float angle_around = 2.0f * PI * around_fraction;
      float ring_radius = std::sqrt(1.0f - height * height);

      point light_point =
          active_scene.sun.position +
          active_scene.sun.size * vector(ring_radius * std::cos(angle_around),
                                         ring_radius * std::sin(angle_around),
                                         height);

      ray shadow{P, light_point - P};
      bool blocked = false;
      for (const object *thing : active_scene.objects) {
        float t = thing->hit(shadow);
        if (t > active_scene.shadow_epsilon && t < 1.0f) {
          blocked = true;
          break;
        }
      }

      if (!blocked) {
        unblocked_rays++;
      }
    }
  }

  float light_visible =
      float(unblocked_rays) / (samples_per_side * samples_per_side);

  vector to_light = active_scene.sun.position - P;
  float distance = to_light.length();
  vector toward_light = to_light / distance;

  float falloff = active_scene.light_intensity / (distance * distance);
  float brightness = std::fmax(0.0f, dot(n, toward_light)) * falloff;
  brightness = std::fmin(1.0f, brightness) * light_visible;
  return blend(color(0, 0, 0), base,
               active_scene.ambient_light +
                   (1 - active_scene.ambient_light) * brightness);
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

color ray_color(const ray &r, scene &active_scene) {

  hit_record hit = closest_hit(r, active_scene.objects);
  if (hit.thing) {
    point P = r.point_at(hit.t);
    return shade(hit.thing->base_color, hit.thing->normal_at(P), P, r, active_scene);
  }

  return color(20,20,20) / 255;
}

ppm render(const camera &cam, scene &active_scene) {
  ppm picture{.pixel_columns = cam.get_resolution_width(),
              .pixel_rows = cam.get_resolution_height(),
              .pixels = {},
              .filename = {}};

  picture.pixels.reserve(picture.pixel_columns * picture.pixel_rows);
  // INFO: Reserve is just to allocate the space for the vector.

  for (int row = 0; row < picture.pixel_rows; ++row) {
    for (int column = 0; column < picture.pixel_columns; ++column) {
      // picture.pixels.push_back(
      //     ray_color(cam.ray_through_pixel(column, row), scene));

      // INFO: now for AA!
      // will need to abstract this out for sure.
      // its soo expensive right now.

      // starting at black, then append each rays result
      color total(0, 0, 0);
      for (int i = 0; i < active_scene.aa_grid; ++i) {
        for (int j = 0; j < active_scene.aa_grid; ++j) {
          float offset_x = (j + 0.5f) / active_scene.aa_grid - 0.5f;
          float offset_y = (i + 0.5f) / active_scene.aa_grid - 0.5f;
          ray r = cam.ray_through_pixel(column, row, offset_x, offset_y);
          total = total + ray_color(r, active_scene);
        }
      }
      picture.pixels.push_back(total / (active_scene.aa_grid * active_scene.aa_grid));
      // INFO: returning the averaged result from the offsets
      //       for a now aliased pixel!
      // INFO: push_back just adds to the end of the vector.
    }
  }
  return picture;
}

int main() {
  std::cout << MSG_WELCOME << "\n";

  // int width = get_input<int>(MSG_WIDTH);
  // int height = get_input<int>(MSG_HEIGHT);
  // std::string filename = get_input<std::string>(MSG_FILENAME);

  // TODO: Add a scene struct.

  camera camera1(200, 200);
  camera1.move_to(vector(0, 0, 3));
  camera1.set_vertical_fov(50);

  camera camera2(200,200);
  camera2.move_to(vector(0,0,1.1));

  camera camera3(200, 200);
  camera3.move_to(vector(0, 0, 1.2));
  camera3.set_vertical_fov(120);

 camera2.set_vertical_fov(150);

  light light1 {.position = point( 3, -0.1,  2), .size = 0.5f};
  light light2 {.position = point(-3,  4,  2), .size = 1.0f};
  light light3 {.position = point( 0, 10, 3.2), .size = 2.0f};
  light light4 {.position = point( 2,  0,  1.2), .size = 0.5f};

  sphere red_ball       = sphere(1, point(0, 0, 0), red);
  sphere pumpkin_ball   = sphere(1, point(0, 0, 0), pumpkin);
  sphere gunmetal_ball  = sphere(1, point(0, 0, 0), emerald);
  sphere violet_ball    = sphere(0.75, point(0, 0.5, 0), violet);

  sphere red_ball2      = sphere(0.50, point(0.65, -0.7, 0), red);
  sphere pumpkin_ball2  = sphere(0.50, point(-0.65, -0.7, 0), pumpkin);

  plane floor = plane(point(0,-1,0), vector(0,1,0), lapis_lazuli);
  plane floor2 = plane(point(0,-1,0), vector(0,1,0), cherry_blossom);
  plane floor3 = plane(point(0,-1.2,0), vector(0,1,0), watermelon);
  plane floor4 = plane(point(0,-0.4,0), vector(0,1,0), papaya);

  plane wall1 = plane(point(0,-1,-4), vector(0,0,1), emerald);

  scene one{.light_intensity = 25,
            .ambient_light = 0.2,
            .shadow_samples_per_side = 4,
            .objects = {&floor, &red_ball}, 
            .sun = {light1}};

  scene two{.light_intensity = 400,
            .ambient_light = 0.2,
            .aa_grid = 12,
            .shadow_samples_per_side = 4,
            .objects = {&floor3, &violet_ball, &red_ball2, &pumpkin_ball2},
            .sun = {light3}};

  scene three{.light_intensity = 20,
              .aa_grid = 8, 
              .shadow_samples_per_side = 4,
              .objects = {&floor2, &wall1, &pumpkin_ball},
              .sun = {light2}};

  scene four{.light_intensity = 30,
            .ambient_light = 0.1,
            .shadow_samples_per_side = 4,
            .objects = {&floor4, &gunmetal_ball}, 
            .sun = {light4}};

  ppm pic_red_ball = render(camera1, one);
  ppm pic_violet_ball = render(camera3, two);
  ppm pic_pumpkin_ball = render(camera1, three);
  ppm pic_gunmetal_ball = render(camera2, four);


  // Playing with 2D lines below.
  // point a = point(red_ball.origijjhh + red_ball.radius)
  // point b = point(200,200,0);
  // ray line {a, b - a};

  // for (int i = 0; i <= 200; ij++){
  //   point p = line.point_at(i / 200.0f);
  //   int column = std::lround(p.x());
  //   int row = std::lround(p.y());
  //   pic_red_ball.pixels[row * pic_red_ball.pixel_columns + column] = white;
  // }

  ppm horizontal = ppm::combine_images(pic_red_ball, pic_pumpkin_ball);
  ppm horizontal2 = ppm::combine_images(pic_violet_ball, pic_gunmetal_ball);
  ppm list = ppm::combine_images(horizontal, horizontal2);

  list.filename = "something";
  list.write_file();
}
