#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include <memory>
#include "common.h"
#include "integers.h"
#include "characters.h"
#include "utils.h"


namespace strategies {
class Strings : public Generator<std::string> {
 public:
  Strings(std::shared_ptr<Generator<size_t>> lengths,
          std::shared_ptr<Generator<char>> alphabet,
          unsigned max_attempts = MAX_ATTEMPTS);

  Strings(std::shared_ptr<Generator<size_t>> lengths,
          std::shared_ptr<Generator<char>> alphabet,
          const std::vector<utils::Predicate<std::string>> &predicates,
          unsigned max_attempts = MAX_ATTEMPTS);

  Strings(std::shared_ptr<Generator<size_t>> lengths,
          const char *alphabet);

  Strings(std::shared_ptr<Generator<size_t>> lengths,
          const std::string &alphabet);

  Strings filter(const utils::Predicate<std::string> &predicate) const;

  bool satisfactory(const std::string &string) const;

  std::string operator()() const override;

 private:
  std::shared_ptr<Generator<size_t>> lengths_;
  std::shared_ptr<Generator<char>> alphabet_;
  std::vector<utils::Predicate<std::string>> predicates_;
  unsigned max_attempts_;
};
}
