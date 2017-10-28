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
  Strings(std::shared_ptr<Strategy<size_t>> lengths,
          std::shared_ptr<Strategy<char>> alphabet);

  /**
   * Generates pseudo-random ``std::string`` instance.
   */
  std::string operator()() const override;

 private:
  std::shared_ptr<Strategy<size_t>> lengths_;
  std::shared_ptr<Strategy<char>> alphabet_;
};
}
