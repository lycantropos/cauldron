#pragma once

#include <functional>
#include <memory>
#include "sieve.h"


namespace strategies {
template<typename T>
class Strategy {
 public:
  virtual T operator()() const = 0;
};


template<typename T>
class Filtered : public Strategy<T> {
 public:
  std::unique_ptr<Filtered<T>> filter(
      const Requirement<T> &requirement) const {
    auto sieve = sieve_.extend(requirement);
    return update_sieve(sieve);
  }

  T operator()() const override {
    auto producer = std::bind(&Filtered::producer, this);
    return sieve_.sift(producer);
  }

 protected:
  Sieve<T> sieve_;

  explicit Filtered(const Sieve<T> &sieve) :
      sieve_(sieve) {};

  virtual T producer() const = 0;

  virtual std::unique_ptr<Filtered<T>> update_sieve(
      const Sieve<T> &sieve
  ) const = 0;
};
}
