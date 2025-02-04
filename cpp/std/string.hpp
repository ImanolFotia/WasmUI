#pragma once

#include "../defs.hpp"
#include "../math/math.hpp"



namespace std {


extern "C" {
  [[maybe_unused]] void* memset(void* dest, int ch, size_t count);
  [[maybe_unused]] void* memcpy(void* dest, void *src, size_t count);
  [[maybe_unused]] size_t strlen(const char* str);
}

template <typename T> char *to_string(char *buffer, T number, int base = 10) {
  return nullptr;
}
template <> char *to_string<int>(char *buffer, int number, int base);
template <> char *to_string<double>(char *buffer, double number, int base);
} // namespace std