#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <string>

#define MSG_HEIGHT "Enter the total height in pixels you'd like: "
#define MSG_WIDTH "Enter the total width in pixels you'd like: "
#define MSG_FILENAME "Enter a filename for the output: "

template <typename T> T get_input(const std::string &message) {
  std::cout << message;
  T input;
  std::cin >> input;
  return input;
}

#endif
