#pragma once

#include <vector>
#include <string>
#include "globals.h"


namespace factories {
template<typename T>
std::vector<T> integers_range(T min_integer,
                              T max_integer) {
  auto length = static_cast<size_t>(max_integer - min_integer + 1);
  std::vector<T> integers;
  integers.reserve(length);
  for (T integer = min_integer; integer <= max_integer; ++integer) {
    integers.push_back(integer);
  }
  return integers;
}


std::string characters_string(size_t min_length = constants::min_capacity,
                              size_t max_length = constants::max_capacity);

std::string non_zero_characters(char min_character = constants::min_character,
                                char max_character = constants::max_character);
}
