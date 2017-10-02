#pragma once

#include <functional>
#include <memory>
#include "utils.h"


namespace strategies {
static const unsigned MAX_ATTEMPTS = 1000;


template<typename T>
class Generator {
 public:
  virtual T operator()() const = 0;
};


class OutOfTries : public std::exception {
 public:
  explicit OutOfTries(unsigned max_tries) :
      msg_("Could not generate any valid examples in "
               + std::to_string(max_tries)
               + " attempts") {};

  const char *what() const throw() override {
    return msg_.c_str();
  }

 private:
  std::string msg_;
};


template<typename T>
class Sieve {
 public:
  explicit Sieve(unsigned max_attempts = MAX_ATTEMPTS) :
      max_tries_(max_attempts) {};

  explicit Sieve(const std::vector<utils::Predicate<T>> &predicates,
                 unsigned max_attempts = MAX_ATTEMPTS) :
      predicates_(predicates),
      max_tries_(max_attempts) {}

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
                    max_tries_);
  }

  T sift(std::function<T()> producer) const {
    for (unsigned _ = 0; _ < max_tries_; ++_) {
      T product = producer();
      if (not satisfactory(product)) {
        continue;
      }
      return product;
    }
    throw OutOfTries(max_tries_);
  }

 protected:
  std::vector<utils::Predicate<T>> predicates_;
  unsigned max_tries_;
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
