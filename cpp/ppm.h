#ifndef PPM_H
#define PPM_H

#include <fstream>
#include <iostream>
#include <string>

#define MSG_FILECREATED "File created: "
#define FILE_EXTENSION ".ppm"

void ppm_write_pixels(std::ostream &output, int width, int height);

inline int file_create_ppm(int width, int height, std::string filename) {

  std::ofstream ppm(filename + FILE_EXTENSION);

  if (ppm.is_open()) {
    ppm << "P3\n" << width << ' ' << height << "\n" << "255\n";
    ppm_write_pixels(ppm, width, height);
  } else {
    std::cerr << "ERROR: Could not create or open "
              << filename + FILE_EXTENSION;
  }

  return 0;
}

#endif
