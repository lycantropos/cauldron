#pragma once

#include "bases.h"


namespace cauldron {
/**
 * Strategy which generates ``Object`` instances
 * with constructor arguments generated from corresponding strategies.
 *
 * \note ``Object`` should have appropriate constructor
 *  so types and order of passed strategies agree
 *  with types and order of constructor parameters
 *  or compile-time error will arise.
 * @tparam Object: type of objects generated by strategy.
 */
template<class Object, class ...Value>
class Builder : public CloneHelper<Object, Builder<Object, Value...>> {
 public:
  explicit Builder(std::shared_ptr<cauldron::Strategy<Value>>... strategy) :
      strategies_(std::make_tuple(strategy...)) {}

  /**
   * Generates pseudo-random ``Object`` instance.
   */
  Object operator()() const override {
    auto strategies_count = std::tuple_size<decltype(strategies_)>{};
    return produce(strategies_,
                   std::make_index_sequence<strategies_count>{});
  }

 private:
  std::tuple<std::shared_ptr<cauldron::Strategy<Value>>...> strategies_;

  /**
   * Helper function for unpacking ``Builder::strategies_`` tuple
   * into variadic ``Strategy`` instances.
   */
  template<std::size_t... Indices>
  Object produce(
      std::tuple<std::shared_ptr<cauldron::Strategy<Value>>...> tuple,
      std::index_sequence<Indices...>
  ) const {
    return produce(std::get<Indices>(tuple)...);
  }

  /**
   * Helper function for producing values
   * from variadic ``Strategy`` instances.
   */
  Object produce(
      std::shared_ptr<cauldron::Strategy<Value>>... strategy
  ) const {
    return Object(strategy->operator()()...);
  }
};
}
