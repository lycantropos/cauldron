#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include "bases.h"
#include "integers.h"


namespace strategies {
class Characters : public CloneHelper<char, Characters> {
 public:
  explicit Characters(const std::string &whitelist_characters);

  explicit Characters(const char whitelist_characters[]);

  char operator()() const override;

 private:
  std::string characters_;
};
}
