#pragma once

#include "defs.hpp"
#include "std/string.hpp"

struct JsCString {
  size_t data;
  size_t len;
};

class JsString {
public:
  JsString() = default;
  JsString(const char *str) : data{(size_t)(str)}, length{std::strlen(str)} {}
  JsString(size_t str, size_t l) : data{str}, length{l} {}
  JsString(size_t x) : data{x}{
    length = std::strlen((char*)x);
  }

  size_t data{};
  size_t length{};

private:
};

