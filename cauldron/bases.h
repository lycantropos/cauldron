#pragma once

#include <memory>
#include <random>
#include "sieve.h"
#include "facility.h"


namespace cauldron {
template<typename Value>
class Union;


template<typename Value>
class Filtered;


template<typename Value>
class Mapped;


/**
 * Strategies base class.
 *
 * Hereafter **strategy** is an object
 * which encapsulates an algorithm
 * for generating specific type of values.
 * @tparam Value: type of values generated by strategy.
 */
template<typename Value>
class Strategy {
 public:
  /**
   * Generates data.
   */
  virtual Value operator()() const = 0;

  /**
   * Returns a new strategy
   * that generates values from either original strategy or provided one.
   */
  virtual Union<Value> operator||(const Strategy<Value> &strategy) const {
    return Union<Value>{std::move(clone()),
                        std::move(strategy.clone())};
  }

  /**
   * Same result as for any other strategy.
   * Basically added for possible overriding
   * with support of strategies' union associativity.
   */
  virtual Union<Value> operator||(const Union<Value> &strategy) const {
    return strategy || static_cast<const Strategy<Value> &>(*this);
  }

  /**
   * Returns a new strategy
   * that generates values from the strategy
   * modified with provided ``strategies::Converter`` instance.
   */
  virtual std::unique_ptr<Mapped<Value>> map(
      const Converter<Value> &converter
  ) const {
    Facility<Value> facility{converter};
    return std::make_unique<Mapped<Value>>(facility,
                                           std::move(clone()));
  }

  /**
   * Returns a new strategy
   * that generates values from the strategy
   * which satisfy provided ``strategies::Requirement`` instance.
   *
   * Note that if the ``requirement`` is too hard to satisfy
   * this might result in failing with ``OutOfCycles``.
   */
  virtual std::unique_ptr<Filtered<Value>> filter(
      const Requirement<Value> &requirement
  ) const {
    Sieve<Value> sieve{requirement};
    return std::make_unique<Filtered<Value>>(sieve,
                                             std::move(clone()));
  }

  /**
   * Creates a copy of the strategy instance
   * and returns pointer to it.
   */
  virtual std::unique_ptr<Strategy<Value>> clone() const = 0;
};


/**
 * Helper class for implementing ``Strategy::clone`` method
 * using
 * <a href="https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern">
 * curiously recurring template pattern
 * </a>.
 * @tparam Value: type of values generated by strategy.
 * @tparam Derived: derivative strategy class.
 */
template<typename Value, class Derived>
class CloneHelper : public Strategy<Value> {
 public:
  std::unique_ptr<Strategy<Value>> clone() const override {
    return std::make_unique<Derived>(static_cast<const Derived &>(*this));
  }
};


/**
 * Union of strategies.
 * Generates values from any one of given strategies.
 *
 * @tparam Value: type of values generated by strategy.
 */
template<class Value>
class Union : public CloneHelper<Value, Union<Value>> {
 public:
  Union(std::initializer_list<std::shared_ptr<Strategy<Value>>> strategies) {
    if (strategies.size() == 0) {
      throw std::invalid_argument("``strategies`` should be non-empty.");
    }
    strategies_ = strategies;
  }

  explicit Union(std::vector<std::shared_ptr<Strategy<Value>>> strategies) {
    if (strategies.size() == 0) {
      throw std::invalid_argument("``strategies`` should be non-empty.");
    }
    strategies_ = std::move(strategies);
  }

  /**
   * Generates value from one of ``strategies_`` elements.
   */
  Value operator()() const override {
    static std::random_device random_device;
    size_t max_index = strategies_.size() - 1;
    std::uniform_int_distribution<size_t> distribution(0, max_index);
    size_t index = distribution(random_device);
    return (*strategies_[index])();
  }

  Union<Value> operator||(const Strategy<Value> &strategy) const override {
    std::vector<std::shared_ptr<Strategy<Value>>> strategies(strategies_);
    strategies.push_back(std::move(strategy.clone()));
    return Union<Value>(strategies);
  }

  Union<Value> operator||(const Union<Value> &strategy) const override {
    std::vector<std::shared_ptr<Strategy<Value>>> strategies(strategies_);
    strategies.insert(strategies.begin(),
                      strategy.strategies_.begin(),
                      strategy.strategies_.end());
    return Union<Value>(strategies);
  }

 private:
  std::vector<std::shared_ptr<Strategy<Value>>> strategies_;
};


/**
 * Strategy which filters out values
 * that satisfies defined ``strategies::Requirement`` instances.
 *
 * Hereafter a **requirement** is an
 * <a href="https://en.wikipedia.org/wiki/Predicate_(mathematical_logic)">
 * unary predicate
 * </a>.
 * @tparam Value: type of values generated by strategy.
 */
template<typename Value>
class Filtered : public CloneHelper<Value, Filtered<Value>> {
 public:
  explicit Filtered(const Sieve<Value> &sieve,
                    std::shared_ptr<Strategy<Value>> strategy) :
      sieve_(sieve),
      strategy_(std::move(strategy)) {};

  std::unique_ptr<Filtered<Value>> filter(
      const Requirement<Value> &requirement
  ) const override {
    auto sieve = sieve_.expand(requirement);
    return std::make_unique<Filtered<Value>>(sieve,
                                             strategy_);
  }

  /**
   * Generates value
   * that satisfies defined ``strategies::Requirement`` instances.
   * @throws strategies::OutOfCycles
   */
  Value operator()() const override {
    std::function<Value()> producer([&]() -> Value {
      return (*strategy_)();
    });
    return sieve_.sift(producer);
  }

 protected:
  Sieve<Value> sieve_;
  std::shared_ptr<Strategy<Value>> strategy_;
};


/**
 * Strategy which modifies values
 * with defined ``strategies::Converter`` instances.
 *
 * Hereafter a **converter** is an
 * <a href="https://en.wikipedia.org/wiki/Operator_(mathematics)">
 * operator
 * </a>.
 * @tparam Value: type of values generated by strategy.
 */
template<typename Value>
class Mapped : public CloneHelper<Value, Mapped<Value>> {
 public:
  explicit Mapped(const Facility<Value> &facility,
                  std::shared_ptr<Strategy<Value>> strategy) :
      facility_(facility),
      strategy_(std::move(strategy)) {};

  std::unique_ptr<Mapped<Value>> map(
      const Converter<Value> &converter
  ) const override {
    auto facility = facility_.expand(converter);
    return std::make_unique<Mapped>(facility,
                                    strategy_);
  }

  /**
   * Generates value
   * and modifies it with defined ``strategies::Converter`` instances.
   */
  Value operator()() const override {
    Value product = (*strategy_)();
    return facility_.convert(product);
  }

 protected:
  Facility<Value> facility_;
  std::shared_ptr<Strategy<Value>> strategy_;
};
}
