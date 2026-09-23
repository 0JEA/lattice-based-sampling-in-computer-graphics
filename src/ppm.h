#ifndef PPM_H
#define PPM_H

#include <fstream>
#include <iostream>
#include <string>

#define MSG_FILECREATED "File created: "
#define MSG_ERROR "ERROR: Could not create or open "

#define FILE_EXTENSION ".ppm"
#define MAX_RGB 255 // 8 bits per channel

void ppm_write_pixels(std::ostream& output, int width, int height);
int file_create_ppm(int width, int height, std::string filename);

#endif
