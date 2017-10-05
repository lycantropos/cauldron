#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include <memory>
#include "bases.h"
#include "integers.h"
#include "characters.h"


namespace strategies {
class Strings : public CloneHelper<std::string, Strings> {
 public:
  Strings(std::shared_ptr<Strategy<size_t>> lengths,
          std::shared_ptr<Strategy<char>> alphabet);

  std::string operator()() const override;

 private:
  std::shared_ptr<Strategy<size_t>> lengths_;
  std::shared_ptr<Strategy<char>> alphabet_;
};
}
