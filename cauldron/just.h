#pragma once

#include "bases.h"


namespace strategies {
template<typename T>
/**
 * Strategy which generates the same value.
 */
class Just : public CloneHelper<T, Just<T>> {
 public:
  /**
   * @param value: value to generate.
   */
  explicit Just(T value) :
      value_(value) {};

  /**
   * Generates the same value.
   */
  T operator()() const override {
    return value_;
  }

 private:
  T value_;
};
}
