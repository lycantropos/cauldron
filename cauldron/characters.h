#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include "common.h"
#include "integers.h"


namespace strategies {
class Characters {
 public:
  explicit Characters(const std::string &whitelist_characters);

  explicit Characters(const char whitelist_characters[]);

  char operator()() const;

 private:
  std::string characters_;
};


Characters characters(const char whitelist_characters[]);
}
