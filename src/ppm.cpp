#include "ppm.h"

bool file_create_ppm(const std::string &filename, const ppm &picture) {

  std::ofstream ppm_file(filename + FILE_EXTENSION);

  if (ppm_file.is_open()) {
    ppm_file << "P3\n"
        << picture.pixel_columns << ' ' << picture.pixel_rows << "\n"
        << MAX_RGB << "\n";
    for (const color& pixel : picture.pixels) {
      write_color(ppm_file, pixel[0], pixel[1], pixel[2]);
    }
  } else {
    std::cerr << MSG_ERROR << filename + FILE_EXTENSION + '\n';
    return false;
  }

  return true;
}
