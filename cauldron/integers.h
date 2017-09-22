#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include "base.h"


namespace strategies {

template<typename T>
class Integers : Generator<T> {
 public:
  explicit Integers(T min_value = std::numeric_limits<T>::min(),
                    T max_value = std::numeric_limits<T>::max())
      : min_value_(min_value),
        max_value_(max_value) {};

  explicit Integers(T max_value)
      : min_value_(std::numeric_limits<T>::min()),
        max_value_(max_value) {};

  T operator()() override {
    std::random_device random_device;
    auto distribution = std::uniform_int_distribution<T>(min_value_,
                                                         max_value_);
    return distribution(random_device);
  }

 private:
  T min_value_;
  T max_value_;
};
}
