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
  Strings(Generator<size_t> *lengths,
          Generator<char> *alphabet,
          unsigned max_attempts = MAX_ATTEMPTS);

  Strings(Generator<size_t> *lengths,
          Generator<char> *alphabet,
          const std::vector<utils::Predicate<std::string>> &predicates,
          unsigned max_attempts = MAX_ATTEMPTS);

  Strings(Generator<size_t> *lengths,
          const char *alphabet);

  Strings(Generator<size_t> *lengths,
          const std::string &alphabet);

  Strings filter(const utils::Predicate<std::string> &predicate) const;

  bool satisfactory(const std::string &string) const;

  std::string operator()() const override;

 private:
  Generator<size_t> *lengths_;
  Generator<char> *alphabet_;
  std::vector<utils::Predicate<std::string>> predicates_;
  unsigned max_attempts_;
};
}
