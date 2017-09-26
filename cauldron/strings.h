#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include "common.h"
#include "integers.h"
#include "characters.h"
#include "utils.h"


namespace strategies {
class Strings {
 public:
  Strings(size_t length,
          Characters alphabet,
          unsigned max_attempts = MAX_ATTEMPTS);

  Strings(size_t length,
          Characters alphabet,
          std::vector<utils::Predicate<std::string>> predicates,
          unsigned max_attempts = MAX_ATTEMPTS);

  Strings(size_t length,
          const char alphabet[]);

  Strings(size_t length,
          const std::string &alphabet);

  Strings filter(const utils::Predicate<std::string> &predicate) const;

  bool satisfactory(const std::string &string) const;

  std::string operator()() const;

 private:
  size_t length_;
  Characters alphabet_;
  std::vector<utils::Predicate<std::string>> predicates_;
  unsigned max_attempts_;
};
}
