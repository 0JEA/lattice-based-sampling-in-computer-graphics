#ifndef PPM_H
#define PPM_H

#include "color.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

/* INFO: I'm switching to using inline constexpr instead of the #define macro 
*        for c++ in this case becuase using constexpr makes them real variables, 
*        allowing stronger typing and allowing for analysis tools to see them. 
*/

// TODO: Make a namespace for these.
inline constexpr char MSG_FILECREATED[] = "File created: ";
inline constexpr char MSG_ERROR[] = "ERROR: Could not create or open ";
inline constexpr char FILE_EXTENSION[] = ".ppm";
inline constexpr int DEFAULT_PPM_FORMAT = 3;

// INFO:  https://netpbm.sourceforge.net/doc/ppm.html
//        https://paulbourke.net/dataformats/ppm/
struct ppm {
  int ppm_format = DEFAULT_PPM_FORMAT; // Declared inline so declaration is ok.
  int pixel_columns;
  int pixel_rows;
  std::vector<color> pixels;
};

// Writes the image to <filename>.ppm, row by row from the top-left.
// Returns false if the file could not be created.
bool file_create_ppm(const std::string &filename, const ppm &picture);

#endif
