#include <limits>
#include "globals.h"


namespace constants {
const size_t min_capacity = 1;
const size_t max_capacity = 100;

const char min_character = ::std::numeric_limits<char>::min();
const char max_character = ::std::numeric_limits<char>::max();
}
