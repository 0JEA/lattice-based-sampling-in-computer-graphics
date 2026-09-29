#include "ppm.h"

bool file_create_ppm(int width, int height, std::string filename,
                     const std::vector<color>& pixels) {

  std::ofstream ppm(filename + FILE_EXTENSION);

  if (ppm.is_open()) {
    ppm << "P3\n" << width << ' ' << height << "\n" << MAX_RGB << "\n";
    for (const color& pixel : pixels) {
      write_color(ppm, pixel[0], pixel[1], pixel[2]);
    }
  } else {
    std::cerr << MSG_ERROR << filename + FILE_EXTENSION;
    return false;
  }

  return true;
}
