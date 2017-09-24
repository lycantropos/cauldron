#pragma once


template<typename T>
class Generator {
  virtual T operator()() const = 0;
};
