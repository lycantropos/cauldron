#pragma once

#include <functional>
#include <memory>
#include "utils.h"


namespace strategies {
static const unsigned MAX_CYCLES = 1000;


template<typename T>
class Generator {
 public:
  virtual T operator()() const = 0;
};


class OutOfCycles : public std::exception {
 public:
  explicit OutOfCycles(unsigned max_cycles) :
      msg_("Could not find any satisfactory product in "
               + std::to_string(max_cycles)
               + " cycles") {};

  const char *what() const throw() override {
    return msg_.c_str();
  }

 private:
  std::string msg_;
};


template<typename T>
class Sieve {
 public:
  explicit Sieve(unsigned max_cycles = MAX_CYCLES) :
      max_cycles_(max_cycles) {};

  explicit Sieve(const std::vector<utils::Predicate<T>> &predicates,
                 unsigned max_cycles = MAX_CYCLES) :
      predicates_(predicates),
      max_cycles_(max_cycles) {}

  bool satisfactory(T product) const {
    for (const auto &predicate: predicates_) {
      if (not predicate(product)) {
        return false;
      }
    }
    return true;
  }

  Sieve<T> extend(const utils::Predicate<T> &predicate) const {
    auto predicates = std::vector<utils::Predicate<T>>(predicates_);
    predicates.push_back(predicate);
    return Sieve<T>(predicates,
                    max_cycles_);
  }

  T sift(std::function<T()> producer) const {
    for (unsigned _ = 0; _ < max_cycles_; ++_) {
      T product = producer();
      if (not satisfactory(product)) {
        continue;
      }
      return product;
    }
    throw OutOfCycles(max_cycles_);
  }

 protected:
  std::vector<utils::Predicate<T>> predicates_;
  unsigned max_cycles_;
};


template<typename T>
class Filtered : public Generator<T> {
 public:
  Filtered() = default;

  explicit Filtered(const Sieve<T> &sieve) :
      sieve_(sieve) {};

  std::unique_ptr<Filtered<T>> filter(
      const utils::Predicate<T> &predicate
  ) const {
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
