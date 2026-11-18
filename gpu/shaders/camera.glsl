#ifndef CAMERA_GLSL
#define CAMERA_GLSL

#include "shapes.glsl"

const float PI = 3.14159265358979;

struct Camera {
  vec3 position;
  vec3 top_left_pixel_center;
  vec3 pixel_step_right;
  vec3 pixel_step_down;
};

Camera make_camera(uvec2 resolution, vec3 position, float focal_length,
                   float vertical_fov_degrees) {
  float vertical_fov_radians = vertical_fov_degrees * PI / 180.0;
  float sensor_height = 2.0 * focal_length * tan(vertical_fov_radians / 2.0);

  float aspect_ratio = float(resolution.x) / float(resolution.y);
  vec3 sensor_center = position - vec3(0.0, 0.0, focal_length);
  float sensor_width = sensor_height * aspect_ratio;

  Camera cam;
  cam.position = position;
  cam.pixel_step_right = vec3(sensor_width / float(resolution.x), 0.0, 0.0);
  cam.pixel_step_down = vec3(0.0, -sensor_height / float(resolution.y), 0.0);

  vec3 top_left_corner = sensor_center - vec3(sensor_width / 2.0, 0.0, 0.0) +
                         vec3(0.0, sensor_height / 2.0, 0.0);

  cam.top_left_pixel_center =
      top_left_corner + 0.5 * (cam.pixel_step_right + cam.pixel_step_down);
  return cam;
}

Ray ray_through_pixel(Camera cam, uint column, uint row, float offset_x,
                      float offset_y) {
  vec3 target = cam.top_left_pixel_center +
                (float(column) + offset_x) * cam.pixel_step_right +
                (float(row) + offset_y) * cam.pixel_step_down;
  return Ray(cam.position, target - cam.position);
}

#endif
