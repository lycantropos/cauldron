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
class Strings : public Generator<std::string> {
 public:
  Strings(const Integers<size_t> &lengths,
          const Characters &alphabet,
          unsigned max_attempts = MAX_ATTEMPTS);

  Strings(const Integers<size_t> &lengths,
          const Characters &alphabet,
          const std::vector<utils::Predicate<std::string>> &predicates,
          unsigned max_attempts = MAX_ATTEMPTS);

  Strings(const Integers<size_t> &lengths,
          const char *alphabet);

  Strings(const Integers<size_t> &lengths,
          const std::string &alphabet);

  Strings filter(const utils::Predicate<std::string> &predicate) const;

  bool satisfactory(const std::string &string) const;

  std::string operator()() const override;

 private:
  Integers<size_t> lengths_;
  Characters alphabet_;
  std::vector<utils::Predicate<std::string>> predicates_;
  unsigned max_attempts_;
};
}
