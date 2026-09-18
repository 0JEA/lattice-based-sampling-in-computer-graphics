#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vec3.h"

#define ASPECT_RATIO (16.0 / 9.0)
#define FOCAL_LENGTH 1.0
#define VIEWPORT_HEIGHT 2.0

class camera {
public:
  static int height_for(int width) {
    int height = int(width / ASPECT_RATIO);
    return (height < 1) ? 1 : height;
  }

  camera(int width, int height) {
    center = point3(0, 0, 0);

    auto viewport_height = VIEWPORT_HEIGHT;
    auto viewport_width = viewport_height * (double(width) / height);

    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    pixel_delta_u = viewport_u / width;
    pixel_delta_v = viewport_v / height;

    auto viewport_upper_left =
        center - vec3(0, 0, FOCAL_LENGTH) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
  }

  ray get_ray(int column, int row) const {
    auto pixel_center =
        pixel00_loc + (column * pixel_delta_u) + (row * pixel_delta_v);

    return ray(center, pixel_center - center);
  }

private:
  point3 center;
  point3 pixel00_loc;
  vec3 pixel_delta_u;
  vec3 pixel_delta_v;
};

#endif
