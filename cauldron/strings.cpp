#include "strings.h"


namespace strategies {
Strings::Strings(std::shared_ptr<Generator<size_t>> lengths,
                 std::shared_ptr<Generator<char>> alphabet,
                 const Sieve<std::string> &sieve) :
    lengths_(std::move(lengths)),
    alphabet_(std::move(alphabet)),
    Filtered<std::string>(sieve) {}


Strings::Strings(std::shared_ptr<Generator<size_t>> lengths,
                 const char *alphabet,
                 const Sieve<std::string> &sieve) :
    lengths_(std::move(lengths)),
    alphabet_(std::make_shared<Characters>(alphabet)),
    Filtered<std::string>(sieve) {}


Strings::Strings(std::shared_ptr<Generator<size_t>> lengths,
                 const std::string &alphabet,
                 const Sieve<std::string> &sieve) :
    lengths_(std::move(lengths)),
    alphabet_(std::make_shared<Characters>(alphabet)),
    Filtered<std::string>(sieve) {}


std::string Strings::producer() const {
  auto length = (*lengths_)();
  std::string result(length, 0);
  // FIXME: workaround to get characters producer from generator
  auto characters_producer = [&]() -> char { return (*alphabet_)(); };
  generate_n(result.begin(),
             length,
             characters_producer);
  return result;
}


std::unique_ptr<Filtered<std::string>> Strings::update_sieve(
    const Sieve<std::string> &sieve
) const {
  return std::make_unique<Strings>(lengths_,
                                   alphabet_,
                                   sieve);
}
}
