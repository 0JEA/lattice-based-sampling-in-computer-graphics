#ifndef PPM_H
#define PPM_H

#include "color.h"
#include "image.h"

#include <fstream>
#include <iostream>
#include <string>

#define MSG_FILECREATED "File created: "
#define MSG_ERROR "ERROR: Could not create or open "

#define FILE_EXTENSION ".ppm"

// Writes the image to <filename>.ppm, row by row from the top-left.
// Returns false if the file could not be created.
bool file_create_ppm(const std::string &filename, const image &picture);

#endif
