#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include <functional>
#include "bases.h"


namespace strategies {
template<typename T>
class Integers : public CloneHelper<T, Integers<T>> {
 public:
  explicit Integers(T min_value = std::numeric_limits<T>::min(),
                    T max_value = std::numeric_limits<T>::max())
      : min_value_(min_value),
        max_value_(max_value) {};

  T operator()() const override {
    static std::random_device random_device;
    auto distribution = std::uniform_int_distribution<T>(min_value_,
                                                         max_value_);
    auto result = distribution(random_device);
    return result;
  }

 private:
  T min_value_;
  T max_value_;
};
}
