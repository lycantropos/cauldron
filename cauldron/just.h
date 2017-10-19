#pragma once

#include "bases.h"


namespace strategies {
/**
 * Strategy which generates the same value.
 * @tparam Value: type of value generated by strategy.
 */
template<typename Value>
class Just : public CloneHelper<Value, Just<Value>> {
 public:
  /**
   * @param value: value to generate.
   */
  explicit Just(Value value) :
      value_(value) {};

  /**
   * Generates the same value.
   */
  Value operator()() const override {
    return value_;
  }

 private:
  Value value_;
};
}
