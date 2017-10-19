#pragma once

#include <functional>
#include <vector>


namespace strategies {
static const unsigned MAX_CYCLES = 1'000;

template<typename Product>
using Requirement = std::function<bool(Product)>;


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


template<typename Product>
class Sieve {
 public:
  explicit Sieve(unsigned max_cycles = MAX_CYCLES) :
      max_cycles_(max_cycles) {};

  Sieve(std::initializer_list<Requirement<Product>> requirements,
        unsigned max_cycles = MAX_CYCLES) :
      requirements_(requirements),
      max_cycles_(max_cycles) {}

  explicit Sieve(const std::vector<Requirement<Product>> &requirements,
                 unsigned max_cycles = MAX_CYCLES) :
      requirements_(requirements),
      max_cycles_(max_cycles) {}

  bool satisfactory(Product product) const {
    for (const auto &requirement: requirements_) {
      bool satisfies_requirement = requirement(product);
      if (not satisfies_requirement) {
        return false;
      }
    }
    return true;
  }

  Sieve<Product> expand(const Requirement<Product> &requirement) const {
    std::vector<Requirement<Product>> requirements(requirements_);
    requirements.push_back(requirement);
    return Sieve<Product>(requirements,
                          max_cycles_);
  }

  Product sift(std::function<Product()> producer) const {
    for (unsigned _ = 0; _ < max_cycles_; ++_) {
      Product product = producer();
      if (not satisfactory(product)) {
        continue;
      }
      return product;
    }
    throw OutOfCycles(max_cycles_);
  }

 protected:
  std::vector<Requirement<Product>> requirements_;
  unsigned max_cycles_;
};
}
