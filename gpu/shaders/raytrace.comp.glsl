#version 460
#extension GL_EXT_scalar_block_layout : require
#extension GL_GOOGLE_include_directive : require

#include "shapes.glsl"
#include "camera.glsl"
#include "scene.glsl"

layout(local_size_x = 16, local_size_y = 8, local_size_z = 1) in;

layout(binding = 0, set = 0, scalar) buffer storageBuffer {
  vec3 imageData[];
};

layout(push_constant) uniform Settings {
  uint width;
  uint height;
  int aa_grid;
  int shadow_samples_per_side;
} settings;

const int HIT_NONE = 0;
const int HIT_PLANE = 1;
const int HIT_SPHERE = 2;

struct HitRecord {
  float t;
  int kind;
  int index;
};

vec3 blend(vec3 from, vec3 to, float strength) {
  return from + strength * (to - from);
}

bool blocked(Ray shadow) {
  for (int i = 0; i < PLANE_COUNT; ++i) {
    float t = hit_plane(planes[i], shadow);
    if (t > shadow_epsilon && t < 1.0) {
      return true;
    }
  }
  for (int i = 0; i < SPHERE_COUNT; ++i) {
    float t = hit_sphere(spheres[i], shadow);
    if (t > shadow_epsilon && t < 1.0) {
      return true;
    }
  }
  return false;
}

vec3 shade(vec3 base, vec3 n, vec3 P, Ray r) {
  if (dot(r.direction, n) > 0.0) {
    n = -n;
  }

  int samples_per_side = settings.shadow_samples_per_side;
  int unblocked_rays = 0;

  for (int height_step = 0; height_step < samples_per_side; ++height_step) {
    for (int around_step = 0; around_step < samples_per_side; ++around_step) {
      float height_fraction = (float(height_step) + 0.5) / float(samples_per_side);
      float around_fraction = (float(around_step) + 0.5) / float(samples_per_side);

      vec3 light_point =
          light_position +
          light_size * vec3(around_fraction - 0.5, 0.0, height_fraction - 0.5);

      Ray shadow = Ray(P, light_point - P);
      if (!blocked(shadow)) {
        unblocked_rays++;
      }
    }
  }

  float light_visible =
      float(unblocked_rays) / float(samples_per_side * samples_per_side);

  vec3 to_light = light_position - P;
  float distance = length(to_light);
  vec3 toward_light = to_light / distance;

  float falloff = light_intensity / (distance * distance);
  float brightness = max(0.0, dot(n, toward_light)) * falloff;
  brightness = min(1.0, brightness) * light_visible;
  return blend(vec3(0.0), base, ambient_light + (1.0 - ambient_light) * brightness);
}

HitRecord closest_hit(Ray r) {
  HitRecord closest = HitRecord(1e30, HIT_NONE, -1);
  for (int i = 0; i < PLANE_COUNT; ++i) {
    float t = hit_plane(planes[i], r);
    if (t > 0.0 && t < closest.t) {
      closest = HitRecord(t, HIT_PLANE, i);
    }
  }
  for (int i = 0; i < SPHERE_COUNT; ++i) {
    float t = hit_sphere(spheres[i], r);
    if (t > 0.0 && t < closest.t) {
      closest = HitRecord(t, HIT_SPHERE, i);
    }
  }
  return closest;
}

vec3 ray_color(Ray r) {
  HitRecord hit = closest_hit(r);
  vec3 P = point_at(r, hit.t);

  if (hit.kind == HIT_PLANE) {
    Plane thing = planes[hit.index];
    return shade(thing.base_color, thing.normal, P, r);
  }
  if (hit.kind == HIT_SPHERE) {
    Sphere thing = spheres[hit.index];
    return shade(thing.base_color, sphere_normal_at(thing, P), P, r);
  }

  return background;
}

void main() {
  const uvec2 resolution = uvec2(settings.width, settings.height);
  const uvec2 pixel = gl_GlobalInvocationID.xy;

  if ((pixel.x >= resolution.x) || (pixel.y >= resolution.y)) {
    return;
  }

  Camera cam = make_camera(resolution, camera_position, camera_focal_length,
                           camera_vertical_fov);

  vec3 total = vec3(0.0);
  for (int i = 0; i < settings.aa_grid; ++i) {
    for (int j = 0; j < settings.aa_grid; ++j) {
      float offset_x = (float(j) + 0.5) / float(settings.aa_grid) - 0.5;
      float offset_y = (float(i) + 0.5) / float(settings.aa_grid) - 0.5;
      Ray r = ray_through_pixel(cam, pixel.x, pixel.y, offset_x, offset_y);
      total = total + ray_color(r);
    }
  }

  uint linearIndex = resolution.x * pixel.y + pixel.x;
  imageData[linearIndex] = total / float(settings.aa_grid * settings.aa_grid);
}
