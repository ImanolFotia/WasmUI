#pragma once

#include "defs.hpp"

extern "C" {
  [[maybe_unused]] void* memset(void* dest, int ch, size_t count);
  [[maybe_unused]] void* memcpy(void* dest, void *src, size_t count);
  [[maybe_unused]] size_t strlen(const char* str);
}
struct JsCString {
  size_t data;
  size_t len;
};

class JsString {
public:
  JsString() = default;
  JsString(const char *str) : data{(size_t)(str)}, length{strlen(str)} {}
  JsString(size_t str, size_t l) : data{str}, length{l} {}
  JsString(size_t x) : data{x}{
    length = strlen((char*)x);
  }

  size_t data{};
  size_t length{};

private:
};


extern "C" void puts(JsString);