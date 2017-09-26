#pragma once

#include <functional>
#include <vector>


namespace utils {
template<typename T>
using Predicate = std::function<bool(T)>;


template<typename T>
T positive_modulo(T dividend,
                  T divisor) {
  return (dividend % divisor + divisor) % divisor;
}


template<typename T>
bool primitive_satisfies_predicates(
    T primitive,
    const std::vector<Predicate<T>> &predicates) {
  for (const auto &predicate: predicates) {
    if (not predicate(primitive)) {
      return false;
    }
  }
  return true;
}


template<typename T>
bool object_satisfies_predicates(
    const T &object,
    const std::vector<Predicate<T>> &predicates) {
  for (const auto &predicate: predicates) {
    if (not predicate(object)) {
      return false;
    }
  }
  return true;
}
}
