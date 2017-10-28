#pragma once

#include <functional>
#include <vector>
#include <memory>


namespace cauldron {
template<typename Product>
using Converter = std::function<Product(Product)>;


template<typename Product>
class Facility {
 public:
  Facility() = default;

  Facility(std::initializer_list<Converter<Product>> converters) :
      converters_(converters) {}

  explicit Facility(const std::vector<Converter<Product>> &converters) :
      converters_(converters) {}

  Facility<Product> expand(const Converter<Product> &converter) const {
    std::vector<Converter<Product>> converters(converters_);
    converters.push_back(converter);
    return Facility<Product>(converters);
  }

  Product convert(Product product) const {
    for (const auto &converter: converters_) {
      product = converter(product);
    }
    return product;
  }

 protected:
  std::vector<Converter<Product>> converters_;
};
}
