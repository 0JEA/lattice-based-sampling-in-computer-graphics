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
