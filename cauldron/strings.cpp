#include "strings.h"


namespace strategies {
Strings::Strings(const size_t &length,
                 const Characters &alphabet) :
    length_(length),
    alphabet_(alphabet) {}


Strings::Strings(const size_t &length,
                 const char alphabet[]) :
    length_(length),
    alphabet_(Characters(alphabet)) {}


Strings::Strings(const size_t &length,
                 const std::string &alphabet) :
    length_(length),
    alphabet_(Characters(alphabet)) {}


std::string Strings::operator()() const {
  std::string result(length_, 0);
  std::generate_n(result.begin(),
                  length_,
                  alphabet_);
  return result;
}
}
