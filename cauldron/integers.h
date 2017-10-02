#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include <functional>
#include "bases.h"


namespace strategies {
template<typename T>
class Integers : public Filtered<T> {
  using FilteredT = Filtered<T>;
 public:
  explicit Integers(T min_value = std::numeric_limits<T>::min(),
                    T max_value = std::numeric_limits<T>::max(),
                    const Sieve<T> &sieve = Sieve<T>())
      : min_value_(min_value),
        max_value_(max_value),
        Filtered<T>(sieve) {};

 private:
  T min_value_;
  T max_value_;

  T producer() const override {
    static std::random_device random_device;
    auto distribution = std::uniform_int_distribution<T>(min_value_,
                                                         max_value_);
    auto result = distribution(random_device);
    return result;
  }

  std::unique_ptr<FilteredT> update_sieve(
      const Sieve<T> &sieve
  ) const override {
    return std::make_unique<Integers<T>>(min_value_,
                                         max_value_,
                                         sieve);
  }
};
}
