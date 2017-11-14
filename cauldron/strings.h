#pragma once

#include "bases.h"


namespace cauldron {
/**
 * Strategy which generates ``std::string`` instances
 * with lengths and characters generated from corresponding strategies.
 */
class Strings : public CloneHelper<std::string, Strings> {
 public:
  /**
   * @param lengths: strategy to generate strings lengths from.
   * @param alphabet: strategy to generate strings characters from.
   */
  Strings(const Strategy<size_t> &lengths,
          const Strategy<char> &alphabet);

  /**
   * Default copy constructor doesn't fit
   * since we're using ``std::unique_ptr`` as class members
   * which is not copyable.
   */
  Strings(const Strings &strings);

  /**
   * Generates pseudo-random ``std::string`` instance.
   */
  std::string operator()() const override;

 private:
  std::unique_ptr<Strategy<size_t>> lengths_;
  std::unique_ptr<Strategy<char>> alphabet_;
};
}
