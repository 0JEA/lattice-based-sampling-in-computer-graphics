#include "ppm.h"

int file_create_ppm(int width, int height, std::string filename) {

  std::ofstream ppm(filename + FILE_EXTENSION);

  if (ppm.is_open()) {
    ppm << "P3\n" << width << ' ' << height << "\n" << MAX_RGB << "\n";
    ppm_write_pixels(ppm, width, height);
  } else {
    std::cerr << MSG_ERROR << filename + FILE_EXTENSION;
  }

  return 0;
}
