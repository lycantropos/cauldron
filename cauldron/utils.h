#pragma once

#include <functional>
#include <vector>


namespace utils {
template<typename T>
using Predicate = std::function<bool(T)>;
}
