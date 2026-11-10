#include "ppm.h"
#include <algorithm>
#include <fstream>

color ppm::to_bytes(const color &linear) {
  const float scale = MAX_RGB + 0.999f;
  color bytes;

  for (int i = 0; i < 3; i++) {
    bytes[i] = int(scale * std::clamp(linear[i], 0.0f, 1.0f));
  }

  return bytes;
}

void ppm::write_header(std::ostream &output) {
  output
    << "P" << format << '\n'
    << pixel_columns
    << ' '
    << pixel_rows << '\n'
    << MAX_RGB << '\n';
}

void ppm::write_pixel(std::ostream &output, const color &pixel) {
  output << int(pixel[0]) << ' ' << int(pixel[1]) << ' ' << int(pixel[2])
         << '\n';
}

bool ppm::write_file() {
  std::ofstream ppm_file(filename + FILE_EXTENSION);

  if (!ppm_file.is_open()) {
    std::cerr << MSG_FILE_ERROR << filename + FILE_EXTENSION + '\n';
    return false;
  }

  write_header(ppm_file);

  for (const color& pixel : pixels) {
    write_pixel(ppm_file, to_bytes(pixel));
  }

  return true;
}

ppm ppm::combine_images(const ppm &image1, const ppm &image2,
                        const direction orientation,
                        const int border_thickness,
                        const color border_color) {
  ppm new_file = {
    .pixel_columns = 0,
    .pixel_rows = 0,
    .pixels = {},
    .filename = image1.filename
  };

  if (image1.pixel_columns != image2.pixel_columns) return new_file;
  if (image1.pixel_rows != image2.pixel_rows) return new_file;

  switch (orientation) {
    case direction::horizontal:
      new_file.pixel_columns =
          image1.pixel_columns + border_thickness + image2.pixel_columns;
      new_file.pixel_rows = image1.pixel_rows;
      new_file.pixels.reserve(new_file.pixel_columns * new_file.pixel_rows);

      for (int row = 0; row < new_file.pixel_rows; row++) {
        int beginning_of_row = image1.pixel_columns * row;
        int ending_of_row = beginning_of_row + image1.pixel_columns;

        new_file.pixels.insert(new_file.pixels.end(),
                               image1.pixels.begin() + beginning_of_row,
                               image1.pixels.begin() + ending_of_row);
        new_file.pixels.insert(new_file.pixels.end(), border_thickness,
                               border_color);
        new_file.pixels.insert(new_file.pixels.end(),
                               image2.pixels.begin() + beginning_of_row,
                               image2.pixels.begin() + ending_of_row);
      }

      return new_file;
    case direction::vertical:
      new_file.pixel_columns = image1.pixel_columns;
      new_file.pixel_rows =
          image1.pixel_rows + border_thickness + image2.pixel_rows;
      new_file.pixels.insert(new_file.pixels.end(), image1.pixels.begin(),
                             image1.pixels.end());
      new_file.pixels.insert(new_file.pixels.end(),
                             border_thickness * image1.pixel_columns,
                             border_color);
      new_file.pixels.insert(new_file.pixels.end(), image2.pixels.begin(),
                             image2.pixels.end());
      return new_file;
  }

  return new_file;
}
