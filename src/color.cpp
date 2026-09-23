#include "color.h"

// Copied directly from the ray tracing in a weekend. I intend to explore
// whats happening here in more detail at a later time.
//
// Seems to be related to rgb of 255 vs 256.
void write_color(std::ostream &output, double red, double green, double blue) {
  int red_byte = int(255.999 * red);
  int green_byte = int(255.999 * green);
  int blue_byte = int(255.999 * blue);

  output << red_byte << ' ' << green_byte << ' ' << blue_byte << '\n';
}
