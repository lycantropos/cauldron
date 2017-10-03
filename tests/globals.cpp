#include <limits>
#include "globals.h"


namespace constants {
const size_t min_capacity = 1;
const size_t max_capacity = 100;

const char min_character = ::std::numeric_limits<char>::min();
const char max_character = ::std::numeric_limits<char>::max();

const size_t non_zero_characters_count = size_t(max_character - min_character);
const size_t alphanumeric_characters_count = 26 * 2 + 10;
}
