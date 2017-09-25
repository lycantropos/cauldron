#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include <functional>
#include "common.h"
#include "utils.h"


namespace strategies {
class Booleans {
  using Predicate = std::function<bool(bool)>;
 public:
  explicit Booleans(double probability = 0.5,
                    unsigned max_attempts = MAX_ATTEMPTS)
      : probability_(probability),
        max_attempts_(max_attempts) {};

  explicit Booleans(double probability,
                    std::vector<Predicate> predicates,
                    unsigned max_attempts = MAX_ATTEMPTS)
      : probability_(probability),
        predicates_(std::move(predicates)),
        max_attempts_(max_attempts) {};

  bool satisfactory(bool object) const {
    return utils::satisfies_predicates<bool>(object, predicates_);
  }

  bool operator()() const {
    std::random_device random_device;
    auto distribution = std::bernoulli_distribution(probability_);
    for (unsigned _ = 0; _ < max_attempts_; ++_) {
      auto result = distribution(random_device);
      if (not satisfactory(result)) {
        continue;
      }
      return result;
    }
    throw OutOfTries(max_attempts_);
  }

  Booleans filter(const Predicate &predicate) {
    auto predicates = std::vector<Predicate>(predicates_);
    predicates.push_back(predicate);
    return Booleans(probability_,
                    predicates,
                    max_attempts_);
  }

 private:
  double probability_;
  std::vector<Predicate> predicates_;
  unsigned max_attempts_;
};
}
