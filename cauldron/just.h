#pragma once

#include "bases.h"


namespace strategies {
template<typename T>
class Just : public Strategy<T> {
 public:
  explicit Just(T value) :
      value_(value) {};

  T operator()() const override {
    return value_;
  }

 private:
  T value_;
};
}
