#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include "common.h"
#include "integers.h"


namespace strategies {
class Characters : public Generator<char> {
 public:
  explicit Characters(const std::string &whitelist_characters,
                      unsigned max_attempts = MAX_ATTEMPTS);

  explicit Characters(const std::string &whitelist_characters,
                      const std::vector<utils::Predicate<char>> &predicates,
                      unsigned max_attempts = MAX_ATTEMPTS);

  explicit Characters(const char whitelist_characters[]);

  Characters filter(const utils::Predicate<char> &predicate) const;

  bool satisfactory(char character) const;

  char operator()() const override;

 private:
  std::string characters_;
  std::vector<utils::Predicate<char>> predicates_;
  unsigned max_attempts_;
};
}
