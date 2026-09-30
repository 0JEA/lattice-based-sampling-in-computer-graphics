#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vector.h"

// NOTE: World units are all in type float, pixel units are all int.

class camera {
public:
  camera();
  camera(int resolution_width, int resolution_height);
  camera(int resolution_width, int resolution_height, float focal_length,
         float vertical_fov_degrees);

  // Returns the ray from the lens center through the given pixel.
  // This is only the line; finding what it hits happens in the scene code.
  ray ray_through_pixel(int column, int row) const;

  void move_to(point new_position);
  void move_by(vector offset);
  void set_focal_length(float new_focal_length);
  void set_vertical_fov(float degrees);
  void recompute();

  int get_resolution_width() const;
  int get_resolution_height() const;
  float get_aspect_ratio() const;
  float get_focal_length() const;
  float get_vertical_fov() const;
  point get_position() const;
  float get_sensor_height() const;
  float get_sensor_width() const;
  point get_sensor_center() const;
  point get_top_left_pixel_center() const;
  vector get_pixel_step_right() const;
  vector get_pixel_step_down() const;

private:
  int resolution_width;  // Pixels across for the output image.
  int resolution_height; // Pixels down for the output image.

  float aspect_ratio; // Sensor width / height, computed from the resolution.
  float focal_length; // Distance from the lens center to the sensor.
  float vertical_fov_degrees; // How much of the scene fits top to bottom.

  point position; // The lens center in 3D space.

  // Called the "viewport" in most ray tracing texts.
  // Computed from focal_length and vertical_fov_degrees.
  float sensor_height;
  float sensor_width;  // Computed: sensor_height * aspect_ratio.
  point sensor_center;
  point top_left_pixel_center;
  vector pixel_step_right; // move one pixel to the right
  vector pixel_step_down;  // move one pixel down
};

#endif
