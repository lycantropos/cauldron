#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include <functional>
#include "base.h"


namespace strategies {
template<typename T>
class Integers {
  using Predicate = std::function<bool(T)>;
 public:
  explicit Integers(T min_value = std::numeric_limits<T>::min(),
                    T max_value = std::numeric_limits<T>::max(),
                    unsigned max_attempts = MAX_ATTEMPTS)
      : min_value_(min_value),
        max_value_(max_value),
        max_attempts_(max_attempts) {};

  explicit Integers(T min_value,
                    T max_value,
                    const std::vector<Predicate> &predicates,
                    unsigned max_attempts = MAX_ATTEMPTS)
      : min_value_(min_value),
        max_value_(max_value),
        predicates_(predicates),
        max_attempts_(max_attempts) {};

  bool satisfactory(T object) const {
    for (const auto &predicate: predicates_) {
      if (not predicate(object)) {
        return false;
      }
    }
    return true;
  }

  T operator()() const {
    std::random_device random_device;
    auto distribution = std::uniform_int_distribution<T>(min_value_,
                                                         max_value_);
    for (unsigned _ = 0; _ < max_attempts_; ++_) {
      auto result = distribution(random_device);
      if (not satisfactory(result)) {
        continue;
      }
      return result;
    }
    throw OutOfTries(max_attempts_);
  }

  Integers<T> filter(const Predicate &predicate) {
    auto predicates = std::vector<Predicate>(predicates_);
    predicates.push_back(predicate);
    return Integers<T>(min_value_,
                       max_value_,
                       predicates,
                       max_attempts_);
  }

 private:
  T min_value_;
  T max_value_;
  std::vector<Predicate> predicates_;
  unsigned max_attempts_;
};
}
