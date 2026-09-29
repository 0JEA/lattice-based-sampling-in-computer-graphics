#include "camera.h"

// Defaults: 640x480, focal length 1, sensor height 2 (a 90 degree vertical
// field of view).
camera::camera() : camera(640, 480, 1.0f, 2.0f) {}

camera::camera(int resolution_width, int resolution_height)
    : camera(resolution_width, resolution_height, 1.0f, 2.0f) {}

camera::camera(int resolution_width, int resolution_height, float focal_length,
               float sensor_height)
    : resolution_width(resolution_width), resolution_height(resolution_height),
      focal_length(focal_length), position(0, 0, 0),
      sensor_height(sensor_height) {
  recompute();
}

ray camera::ray_through_pixel(int column, int row) {
  vector pixel_center =
      top_left_pixel_center + column * pixel_step_right + row * pixel_step_down;
  vector direction = pixel_center - position;
  return ray{position, direction};
}

void camera::move_to(vector new_position) {
  position = new_position;
  recompute();
}

void camera::move_by(vector offset) {
  position = position + offset;
  recompute();
}

void camera::set_focal_length(float new_focal_length) {
  focal_length = new_focal_length;
  recompute();
}

void camera::set_sensor_height(float height) {
  sensor_height = height;
  recompute();
}

// I didnt come up with all these formulas, I did need AI's help for these.
// Ive given them all some good thought and drawn them, I belive they make
// sense.
void camera::recompute() {
  aspect_ratio = static_cast<float>(resolution_width) / resolution_height;
  sensor_center = position - vector(0, 0, focal_length);
  sensor_width = sensor_height * aspect_ratio;
  pixel_step_right = vector(sensor_width / resolution_width, 0, 0);
  pixel_step_down = vector(0, -sensor_height / resolution_height, 0);

  // Top-left corner of the sensor: half a width left, half a height up.
  vector top_left_corner = sensor_center - vector(sensor_width / 2, 0, 0) +
                           vector(0, sensor_height / 2, 0);

  // Center of pixel (0, 0): half a pixel in from the corner.
  // Here we're looking at a pixel as a square and this is the middle of that.
  top_left_pixel_center =
      top_left_corner + 0.5f * (pixel_step_right + pixel_step_down);
}
