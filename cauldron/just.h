#pragma once

namespace strategies {
template<typename T>
class Just {
 public:
  explicit Just(T value) :
      value_(value) {};

  T operator()() {
    return value_;
  }

 private:
  T value_;
};
}
