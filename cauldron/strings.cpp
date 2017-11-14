#include <algorithm>

#include "strings.h"


namespace cauldron {
Strings::Strings(const Strategy<size_t> &lengths,
                 const Strategy<char> &alphabet) :
    lengths_(lengths.clone()),
    alphabet_(alphabet.clone()) {}


Strings::Strings(const Strings &strings) :
    lengths_((*strings.lengths_).clone()),
    alphabet_((*strings.alphabet_).clone()) {}


std::string Strings::operator()() const {
  size_t length = (*lengths_)();
  std::string result(length, 0);
  // FIXME: workaround using lambda to get producer from strategy
  auto characters_producer = [&]() -> char {
    return (*alphabet_)();
  };
  std::generate_n(result.begin(),
                  length,
                  characters_producer);
  return result;
}
}
