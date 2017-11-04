#pragma once

#include <functional>
#include <random>
#include <map>


template<typename Map>
typename Map::const_iterator nearest_less_item(
    Map const &map,
    typename Map::key_type const &key
) {
  typename Map::const_iterator iterator = map.lower_bound(key);
  if (iterator != map.begin()) {
    return --iterator;
  }
  return map.end();
}


template<typename T>
std::vector<T> observe(std::function<T()> generator,
                       const size_t sample_size) {
  std::vector<T> sample;
  sample.reserve(sample_size);
  for (size_t _ = 0; _ < sample_size; ++_) {
    sample.push_back(generator());
  }
  std::sort(sample.begin(), sample.end());
  return sample;
}


template<typename Number>
std::map<Number, size_t> frequency_polygon(
    const std::vector<Number> &observations
) {
  static_assert(std::is_arithmetic<Number>(),
                "``Number`` type should be arithmetic.");

  std::map<Number, size_t> result;
  size_t cumulative = 0;
  for (const Number observation : observations) {
    cumulative++;
    result[observation] = cumulative;
  }
  return result;
}


template<typename Number, class Float>
std::function<Float(Float)> empirical_distribution_function(
    const std::vector<Number> &sample
) {
  static_assert(std::is_arithmetic<Number>(),
                "``Number`` type should be arithmetic.");
  static_assert(std::is_floating_point<Float>(),
                "``Float`` type should be floating point.");

  size_t sample_size = sample.size();
  std::map<Number, size_t> polygon = frequency_polygon<Number>(sample);

  return [=](Float x_coordinate) -> Float {
    size_t nearest_less_frequency;
    auto nearest_less_point_ptr = nearest_less_item(polygon,
                                                    x_coordinate);
    if (nearest_less_point_ptr == polygon.end()) {
      nearest_less_frequency = 0;
    } else {
      nearest_less_frequency = (*nearest_less_point_ptr).second;
    }
    return Float(nearest_less_frequency) / sample_size;
  };
}


namespace kolmogorov_smirnov {
/**
 * based on
 * https://en.wikipedia.org/wiki/Kolmogorov–Smirnov_test
 */
template<class Float>
Float statistic(std::function<Float(Float)> edf,
                std::function<Float(Float)> other_edf,
                const Float start,
                const Float stop,
                const Float step) {
  static_assert(std::is_floating_point<Float>(),
                "``Float`` type should be floating point.");

  Float result = 0;
  for (Float point = start; point < stop; point += step) {
    result = std::max(std::abs(edf(point) - other_edf(point)),
                      result);
  }
  return result;
}


template<class Float>
Float c_alpha(Float threshold) {
  static_assert(std::is_floating_point<Float>(),
                "``Float`` type should be floating point.");

  return std::sqrt(-std::log(threshold / 2) / 2);
}


template<class Float>
bool test(std::function<Float(Float)> edf,
          std::function<Float(Float)> other_edf,
          size_t points_count,
          size_t other_points_count,
          const Float start,
          const Float stop,
          const Float threshold) {
  static_assert(std::is_floating_point<Float>(),
                "``Float`` type should be floating point.");

  size_t steps_count = std::max(points_count,
                                other_points_count);
  Float step = stop / steps_count - start / steps_count;
  Float statistic_value = statistic(edf, other_edf,
                                    start, stop, step);
  Float boundary = c_alpha(threshold)
      * std::sqrt(points_count + other_points_count)
      / std::sqrt(points_count * other_points_count);
  bool null_hypothesis_is_rejected = statistic_value > boundary;
  return !null_hypothesis_is_rejected;
}
}

static const double THRESHOLD = 0.001;


template<typename Number, class Float>
bool are_samples_similar(const std::vector<Number> &sample,
                         const std::vector<Number> &other_sample,
                         const Float start,
                         const Float stop,
                         const Float threshold) {
  static_assert(std::is_arithmetic<Number>(),
                "``Number`` type should be arithmetic.");
  static_assert(std::is_floating_point<Float>(),
                "``Float`` type should be floating point.");

  auto edf = empirical_distribution_function<Number, Float>(sample);
  auto other_edf =
      empirical_distribution_function<Number, Float>(other_sample);

  return kolmogorov_smirnov::test(edf,
                                  other_edf,
                                  sample.size(),
                                  other_sample.size(),
                                  start,
                                  stop,
                                  threshold);
}


template<typename Number, class Float>
bool are_generators_similar(std::function<Number()> generator,
                            std::function<Number()> other_generator,
                            const Float start,
                            const Float stop,
                            const size_t start_sample_size = 1'000,
                            const size_t stop_sample_size = 10'000,
                            const Float threshold = THRESHOLD) {
  static_assert(std::is_arithmetic<Number>(),
                "``Number`` type should be arithmetic.");
  static_assert(std::is_floating_point<Float>(),
                "``Float`` type should be floating point.");

  static std::random_device random_device;

  std::uniform_int_distribution<size_t> samples_sizes(start_sample_size,
                                                      stop_sample_size);

  const size_t sample_size = samples_sizes(random_device);
  const size_t other_sample_size = samples_sizes(random_device);

  std::vector<Number> sample = observe(generator,
                                       sample_size);
  std::vector<Number> other_sample = observe(other_generator,
                                             other_sample_size);
  return are_samples_similar(sample,
                             other_sample,
                             start,
                             stop,
                             threshold);
}


template<typename Number, class Float>
bool are_samplers_similar(std::function<std::vector<Number>()> sampler,
                          std::function<std::vector<Number>()> other_sampler,
                          const Float start,
                          const Float stop,
                          const Float threshold = THRESHOLD) {
  static_assert(std::is_arithmetic<Number>(),
                "``Number`` type should be arithmetic.");
  static_assert(std::is_floating_point<Float>(),
                "``Float`` type should be floating point.");

  std::vector<Number> sample = sampler();
  std::vector<Number> other_sample = other_sampler();
  return are_samples_similar(sample,
                             other_sample,
                             start,
                             stop,
                             threshold);
}
