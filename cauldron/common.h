#pragma once

#include <functional>
#include <memory>
#include "sieve.h"


namespace strategies {

template<typename T>
class Generator {
 public:
  virtual T operator()() const = 0;
};


template<typename T>
class Filtered : public Generator<T> {
 public:
  Filtered() = default;

  explicit Filtered(const Sieve<T> &sieve) :
      sieve_(sieve) {};

  std::unique_ptr<Filtered<T>> filter(const Requirement<T> &predicate) const {
    auto sieve = sieve_.extend(predicate);
    return update_sieve(sieve);
  }

  T operator()() const override {
    auto producer = std::bind(&Filtered::producer, this);
    return sieve_.sift(producer);
  }

 protected:
  Sieve<T> sieve_;

  virtual T producer() const = 0;

  virtual std::unique_ptr<Filtered<T>> update_sieve(
      const Sieve<T> &sieve
  ) const = 0;
};
}
