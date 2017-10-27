#include <algorithm>
#include "strings.h"


namespace strategies {
Strings::Strings(std::shared_ptr<Strategy<size_t>> lengths,
                 std::shared_ptr<Strategy<char>> alphabet) :
    lengths_(std::move(lengths)),
    alphabet_(std::move(alphabet)) {}


std::string Strings::operator()() const {
  size_t length = (*lengths_)();
  std::string result(length, 0);
  // FIXME: workaround using bind to get producer from strategy
  auto characters_producer = std::bind(&Strategy<char>::operator(),
                                       alphabet_);
  std::generate_n(result.begin(),
                  length,
                  characters_producer);
  return result;
}
}
