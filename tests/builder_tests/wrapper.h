#pragma once


template<typename Wrapped>
class Wrapper {
 public:
  explicit Wrapper(Wrapped value) :
      field_(value) {};

  Wrapped field() const {
    return field_;
  }

  bool operator==(const Wrapper &other) const {
    return field_ == other.field();
  }

 private:
  Wrapped field_;
};
