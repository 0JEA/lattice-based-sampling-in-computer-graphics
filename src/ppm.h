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
   * @brief How `combine_images` arranges its two images during layout.
   */
  enum class direction{
    horizontal, ///< Side by side, `image1` on the left.
    vertical    ///< Stacked, `image1` on the top.
  };

  /**
   * @brief Writes the image to `filename` + `FILE_EXTENSION` as plain P3.
   *
   * @return true on success, false if the file could not be opened.
   */
  bool write_file();

  /**
   * @brief Combines two `ppm` objects into a single `ppm`.
   *
   * Both given images must be the same size. If they are not, an empty
   * (0x0) image is returned.
   *
   * @param image1 The first given `ppm` to join with the second.
   * @param image2 The second given `ppm` to join with the first.
   * @param orientation The orientation of the combined images.
   * @param border_thickness Thickness of the border in pixels.
   * @param border_color Color of the border that separates the two images.
   * @return A `ppm` containing both given images separated by a border.
   *
   * @code
   * // Assume image1 and image2 are valid PPM images of identical size.
   * ppm horizontally_merged = ppm::combine_images(image1, image2);
   * ppm vertically_merged = ppm::combine_images(image1, image2,
   *                                             ppm::direction::vertical,
   *                                             4, color(1, 0, 0));
   * @endcode
   */
  static ppm combine_images(const ppm &image1, const ppm &image2,
                            const direction orientation = direction::horizontal,
                            const int border_thickness = 1,
                            const color border_color = color(1, 1, 1));

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
