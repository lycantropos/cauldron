#pragma once

#include <functional>


namespace utils {
template<typename T>
T positive_modulo(T dividend,
                  T divisor) {
  return (dividend % divisor + divisor) % divisor;
}


template<typename T>
bool satisfies_predicates(
    T object,
    const std::vector<std::function<bool(T)>> &predicates) {
  for (const auto &predicate: predicates) {
    if (not predicate(object)) {
      return false;
    }
  }
  return true;
}
}
