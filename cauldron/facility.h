#pragma once

#include <functional>
#include <vector>
#include <memory>


namespace strategies {
template<typename T>
using Converter = std::function<T(T)>;


template<typename T>
class Facility {
 public:
  Facility() = default;

  explicit Facility(const std::vector<Converter<T>> &converters) :
      converters_(converters) {}

  Facility<T> expand(const Converter<T> &converter) const {
    auto converters = std::vector<Converter<T>>(converters_);
    converters.push_back(converter);
    return Facility<T>(converters);
  }

  T convert(T product) const {
    for (const auto &converter: converters_) {
      product = converter(product);
    }
    return product;
  }

 protected:
  std::vector<Converter<T>> converters_;
};
}
