#include "color.h"

void write_color(std::ostream &output, float red, float green, float blue) {
  const float scale = MAX_RGB + 0.999f;

  int red_byte = int(scale * red);
  int green_byte = int(scale * green);
  int blue_byte = int(scale * blue);

  output << red_byte << ' ' << green_byte << ' ' << blue_byte << '\n';
}
