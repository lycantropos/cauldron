#pragma once

#include <random>
#include <string>
#include <algorithm>
#include <limits>
#include "common.h"
#include "integers.h"


namespace strategies {
class Characters : public Filtered<char> {
 public:
  explicit Characters(const std::string &whitelist_characters,
                      const Sieve<char> &sieve = Sieve<char>());

  explicit Characters(const char whitelist_characters[],
                      const Sieve<char> &sieve = Sieve<char>());

 private:
  std::string characters_;

  char producer() const override;

  std::unique_ptr<Filtered<char>> update_sieve(
      const Sieve<char> &sieve
  ) const override;
};
}
