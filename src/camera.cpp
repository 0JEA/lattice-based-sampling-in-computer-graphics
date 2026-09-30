#include "camera.h"

#include <cmath>

const float PI = 3.14159265358979f;

// Defaults: 640x480, focal length 1, 90 degree vertical field of view.
camera::camera() : camera(640, 480, 1.0f, 90.0f) {}

camera::camera(int resolution_width, int resolution_height)
    : camera(resolution_width, resolution_height, 1.0f, 90.0f) {}

camera::camera(int resolution_width, int resolution_height, float focal_length,
               float vertical_fov_degrees)
    : resolution_width(resolution_width), resolution_height(resolution_height),
      focal_length(focal_length), vertical_fov_degrees(vertical_fov_degrees),
      position(0, 0, 0) {
  recompute();
}

ray camera::ray_through_pixel(int column, int row) const {
  point pixel_center =
      top_left_pixel_center + column * pixel_step_right + row * pixel_step_down;
  vector direction = pixel_center - position;
  return ray{position, direction};
}

void camera::move_to(point new_position) {
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

void camera::set_vertical_fov(float degrees) {
  vertical_fov_degrees = degrees;
  recompute();
}

int camera::get_resolution_width() const { return resolution_width; }
int camera::get_resolution_height() const { return resolution_height; }
float camera::get_aspect_ratio() const { return aspect_ratio; }
float camera::get_focal_length() const { return focal_length; }
float camera::get_vertical_fov() const { return vertical_fov_degrees; }
point camera::get_position() const { return position; }
float camera::get_sensor_height() const { return sensor_height; }
float camera::get_sensor_width() const { return sensor_width; }
point camera::get_sensor_center() const { return sensor_center; }
point camera::get_top_left_pixel_center() const {
  return top_left_pixel_center;
}
vector camera::get_pixel_step_right() const { return pixel_step_right; }
vector camera::get_pixel_step_down() const { return pixel_step_down; }

// I didnt come up with all these formulas, I did need AI's help for these.
// Ive given them all some good thought and drawn them, I belive they make
// sense.
void camera::recompute() {
  // Half the sensor height over the focal length is tan(half the fov), so
  // sensor_height = 2 * focal_length * tan(fov / 2).
  float vertical_fov_radians =
      vertical_fov_degrees * PI / 180.0f;
  sensor_height = 2.0f * focal_length * std::tan(vertical_fov_radians / 2.0f);

  aspect_ratio = static_cast<float>(resolution_width) / resolution_height;
  sensor_center = position - vector(0, 0, focal_length);
  sensor_width = sensor_height * aspect_ratio;
  pixel_step_right = vector(sensor_width / resolution_width, 0, 0);
  pixel_step_down = vector(0, -sensor_height / resolution_height, 0);

  // Top-left corner of the sensor: half a width left, half a height up.
  point top_left_corner = sensor_center - vector(sensor_width / 2, 0, 0) +
                          vector(0, sensor_height / 2, 0);

  // Center of pixel (0, 0): half a pixel in from the corner.
  // Here we're looking at a pixel as a square and this is the middle of that.
  top_left_pixel_center =
      top_left_corner + 0.5f * (pixel_step_right + pixel_step_down);
}
