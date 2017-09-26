#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include "common.h"
#include "integers.h"


namespace strategies {
class Characters {
  using Predicate = std::function<bool(char)>;
 public:
  explicit Characters(const std::string &whitelist_characters,
                      unsigned max_attempts = MAX_ATTEMPTS);

  explicit Characters(const std::string &whitelist_characters,
                      const std::vector<Predicate> &predicates,
                      unsigned max_attempts = MAX_ATTEMPTS);

  explicit Characters(const char whitelist_characters[]);

  bool satisfactory(char object) const;

  Characters filter(const Predicate &predicate) const;

  char operator()() const;

 private:
  std::string characters_;
  std::vector<Predicate> predicates_;
  unsigned max_attempts_;
};
}
