#pragma once
#include <random>

namespace Engine::Random
{
inline std::random_device SEEDER;
inline std::mt19937 ENGINE{SEEDER()};

inline size_t Int(size_t Min, size_t Max) {
  std::uniform_int_distribution Get{Min, Max};
  return Get(ENGINE);
}
}