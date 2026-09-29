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
         float sensor_height);

  // Returns the ray from the lens center through the given pixel.
  // This is only the line; finding what it hits happens in the scene code.
  ray ray_through_pixel(int column, int row);

  void move_to(vector new_position);
  void move_by(vector offset);
  void set_focal_length(float new_focal_length);
  void set_sensor_height(float height);
  void recompute();

private:
  int resolution_width;  // Pixels across for the output image.
  int resolution_height; // Pixels down for the output image.

  float aspect_ratio; // Sensor width / height, computed from the resolution.
  float focal_length; // Distance from the lens center to the sensor.

  vector position; // The lens center in 3D space.

  float sensor_height; // Called the "viewport" in most ray tracing texts.
  float sensor_width;  // Computed: sensor_height * aspect_ratio.
  vector sensor_center;
  vector top_left_pixel_center;
  vector pixel_step_right; // move one pixel to the right
  vector pixel_step_down;  // move one pixel down
};

#endif
