/**
 * @file ppm.h
 * @brief Image storage and plain P3 PPM file writing.
 */

#ifndef PPM_H
#define PPM_H

#include "vector.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

/* INFO: I'm switching to using inline constexpr instead of the #define macro
*        for c++ in this case because using constexpr makes them real variables,
*        allowing stronger typing and allowing for analysis tools to see them.
*/

// TODO: Make a namespace for these.
inline constexpr char MSG_FILE_CREATED[] = "File created: ";
inline constexpr char MSG_FILE_ERROR[] = "ERROR: Could not create or open ";
inline constexpr char FILE_EXTENSION[] = ".ppm";
inline constexpr int DEFAULT_PPM_FORMAT = 3;
inline constexpr int MAX_RGB = 255; ///< 8 bits per channel.

// INFO:  https://netpbm.sourceforge.net/doc/ppm.html
//        https://paulbourke.net/dataformats/ppm/

/**
 * @class ppm
 * @brief An image held in memory that can be saved as a PPM P3 file.
 *
 * `pixels` holds linear colors (each channel usually in [0, 1]) stored row by
 * row: top row first, left to right within a row, the same order PPM writes
 * them. It must contain exactly `pixel_columns * pixel_rows` elements.
 *
 * @code
 * ppm picture {.pixel_columns = 2, .pixel_rows = 1,
 *              .pixels = {color(0,0,1), color(1,0,0)},
 *              .filename = "out"};
 * picture.write_file(); // Writes out.ppm
 * @endcode
 */
class ppm{
public:
  /// PPM magic number. Only 3 (P3) is supported.
  int format = DEFAULT_PPM_FORMAT;
  int pixel_columns;          ///< Image width in pixels.
  int pixel_rows;             ///< Image height in pixels.
  std::vector<color> pixels;  ///< Row by row, top-left first.
                              ///< Size must be `pixel_columns * pixel_rows`.
  std::string filename;       ///< Output file name without `FILE_EXTENSION`.

  /**
   * @brief Writes the image to `filename` + `FILE_EXTENSION` as plain P3.
   *
   * @return true on success, false if the file could not be opened.
   */
  bool write_file();

private:
  /**
   * @brief Writes the PPM header: magic number, size, and max channel value.
   *
   * Must be called before any pixels are written.
   * For a 2x1 image this function writes:
   *
   * @code
   * P3
   * 2 1
   * 255
   * @endcode
   *
   * @param output Stream to write to, usually the open .ppm file.
   */
  void write_header(std::ostream &output);

  /**
   * @brief Writes one pixel as a "R G B" line.
   *
   * @param output Stream to write to.
   * @param pixel Channel values already in [0, 255], from `to_bytes`.
   */
  void write_pixel(std::ostream &output, const color &pixel);

  /**
   * @brief Converts a linear color into bytes.
   *
   * [0.1, 0.6, 0.4] -> [25, 153, 102]
   *
   * @param linear The linear color represented as three floats in [0, 1].
   * @return A color with each channel as a whole number in bytes.
   */
  static color to_bytes(const color &linear);
};

bool file_create_multi_ppm(const std::string &filename, const std::vector<ppm> &pictures);

#endif
