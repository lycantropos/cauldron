#pragma once

#include <random>


namespace factories {
std::string characters_string(size_t min_size,
                              size_t max_size);


std::string non_zero_characters();
}
