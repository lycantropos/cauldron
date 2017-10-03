#pragma once

#include <algorithm>
#include "bases.h"


namespace strategies {
template<typename T>
class Vectors : public Filtered<std::vector<T>> {
  using Product = std::vector<T>;
  using ElementsStrategy = Strategy<T>;
  using SizesStrategy = Strategy<size_t>;
  using FilteredProduct = Filtered<Product>;

 public:
  Vectors(std::shared_ptr<SizesStrategy> sizes,
          std::shared_ptr<ElementsStrategy> elements,
          const Sieve<Product> &sieve = Sieve<Product>()) :
      sizes_(std::move(sizes)),
      elements_(std::move(elements)),
      Filtered<std::vector<T>>(sieve) {};

 private:
  std::shared_ptr<SizesStrategy> sizes_;
  std::shared_ptr<ElementsStrategy> elements_;

  std::vector<T> producer() const override {
    auto size = (*sizes_)();
    std::vector<T> result(size);
    // FIXME: workaround using lambda to get producer from strategy
    auto elements_producer = [&]() -> T { return (*elements_)(); };
    std::generate_n(result.begin(),
                    size,
                    elements_producer);
    return result;
  }

  std::unique_ptr<FilteredProduct> update_sieve(
      const Sieve<std::vector<T>> &sieve
  ) const override {
    return std::make_unique<Vectors>(sizes_,
                                     elements_,
                                     sieve);
  }
};
}
