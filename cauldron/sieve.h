#pragma once

#include <functional>
#include <vector>
#include <memory>


namespace strategies {
static const unsigned MAX_CYCLES = 1'000;

template<typename T>
using Requirement = std::function<bool(T)>;


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

  explicit Sieve(const std::vector<Requirement<T>> &requirements,
                 unsigned max_cycles = MAX_CYCLES) :
      requirements_(requirements),
      max_cycles_(max_cycles) {}

  bool satisfactory(T product) const {
    for (const auto &predicate: requirements_) {
      if (not predicate(product)) {
        return false;
      }
    }
    return true;
  }

  Sieve<T> extend(const Requirement<T> &requirement) const {
    auto requirements = std::vector<Requirement<T>>
        (requirements_);
    requirements.push_back(requirement);
    return Sieve<T>(requirements,
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
  std::vector<Requirement<T>> requirements_;
  unsigned max_cycles_;
};
}
