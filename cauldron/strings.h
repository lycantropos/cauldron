#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include "common.h"
#include "integers.h"
#include "characters.h"


namespace strategies {
class Strings {
 public:
  Strings(const size_t &length,
          const Characters &alphabet);

  Strings(const size_t &length,
          const char alphabet[]);

  Strings(const size_t &length,
          const std::string &alphabet);

  std::string operator()() const;

 private:
  size_t length_;
  Characters alphabet_;
};
}
