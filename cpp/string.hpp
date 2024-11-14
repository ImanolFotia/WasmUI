#pragma once

#include "defs.hpp"

struct JsCString {
  size_t p;
  size_t len;
};

class JsString {
public:
  JsString(const char *str) : ptr{(size_t)(str)}, length{pLength(str)} {}
  JsString(size_t str, size_t l) : ptr{str}, length{l} {}
  JsString(size_t x) : ptr{x}{
    length = pLength((char*)x);
  }

  size_t ptr{};
  size_t length{};

private:
  size_t pLength(const char *str) {
    char *s = (char *)str;
    int l = 0;
    while (*(s++)) l++;
    return l;
  }
};
