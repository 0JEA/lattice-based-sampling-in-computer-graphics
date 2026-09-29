#ifndef PPM_H
#define PPM_H

#include "color.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define MSG_FILECREATED "File created: "
#define MSG_ERROR "ERROR: Could not create or open "

#define FILE_EXTENSION ".ppm"

// Writes the pixels to <filename>.ppm, row by row from the top-left.
// pixels must hold width * height colors.
// Returns false if the file could not be created.
bool file_create_ppm(int width, int height, std::string filename,
                     const std::vector<color>& pixels);

#endif
