#pragma once

#include "../js_string.hpp"
#include "../math/math.hpp"
#include "std/type_traits.hpp"

extern "C" int puts(JsString);

extern "C" void env_setelementtext(JsString id, JsString text);

namespace std {

template <typename T> int replace_val(const char *fmt, char *buffer, T val) {
  int printed_chars = 0;
  fmt++;
  char *str;
  if constexpr (std::is_same<T, const char *>::value ||
                std::is_same<T, char *>::value) {
    str = (char *)val;
  } else if constexpr (std::is_integral<T>::value) {
    char buf[50];
    memset(&buf, 0, 50);
    to_string<int>(buf, (long)val);
    str = (char *)buf;
  } else if constexpr (std::is_floating_point<T>::value) {
    char buf[50];
    to_string<double>(buf, val);
    str = (char *)buf;
  }

  while (*str != '\0') {
    *buffer++ = *str++;
    printed_chars++;
  }

  while (*fmt != '%' && *fmt != '\0') {
    *buffer++ = *fmt++;
    printed_chars++;
  }

  return printed_chars;
}

static int printf(const char *fmt, char *buffer) {
  if (*fmt == '%')
    return 0;
  *buffer = *fmt;
  return 1;
}

template <typename T, typename... Args>
int printf(const char *fmt, char *buffer, T val, Args &&...args) {
  int printed_chars = 0;
  if (*fmt == '%') {
    fmt++;
    char *str;
    if constexpr (std::is_same<T, const char *>::value ||
                  std::is_same<T, char *>::value) {
      str = (char *)val;
    } else if constexpr (std::is_integral<T>::value) {
      char buf[50];
      memset(&buf, 0, 50);
      to_string<int>(buf, (long)val);
      str = (char *)buf;
    } else if constexpr (std::is_floating_point<T>::value) {
      char buf[50];
      to_string<double>(buf, val);
      str = (char *)buf;
    }

    while (*str != '\0') {
      *buffer++ = *str++;
      printed_chars++;
    }

    while (*fmt != '%' && *fmt != '\0') {
      *buffer++ = *fmt++;
      printed_chars++;
    }

  } else {

    while (*fmt != '%' && *fmt != '\0') {
      *buffer++ = *fmt++;
      printed_chars++;
    }

    if (*fmt == '%') {
      fmt++;
      char *str;
      if constexpr (std::is_same<T, const char *>::value ||
                    std::is_same<T, char *>::value) {
        str = (char *)val;
      } else if constexpr (std::is_integral<T>::value) {
        char buf[50];
        memset(&buf, 0, 50);
        to_string<int>(buf, (long)val);
        str = (char *)buf;
      } else if constexpr (std::is_floating_point<T>::value) {
        char buf[50];
        to_string<double>(buf, val);
        str = (char *)buf;
      }

      while (*str != '\0') {
        *buffer++ = *str++;
        printed_chars++;
      }

      while (*fmt != '%' && *fmt != '\0') {
        *buffer++ = *fmt++;
        printed_chars++;
      }
    }
  }

  printed_chars += printf(fmt, buffer, args...);
  return printed_chars;
}

template <typename... Args> static int printf(const char *str, Args &&...args) {
  char buffer[4096]; // 4Kb printing buffer
  memset(&buffer, 0, 4096);
  char *input = (char *)str;
  int printed_chars = 0;

  printed_chars = printf(input, &buffer[0], args...);
  puts(buffer);
  return printed_chars;
}

/// sprintf

static int sprintf(char *buffer, const char *fmt) {
  if (*fmt == '%')
    return 0;
  *buffer = *fmt;
  return 1;
}

template <typename T, typename... Args>
int sprintf(char *buffer, const char *fmt, T val, Args &&...args) {
  int printed_chars = 0;
  if (*fmt == '%') {
    fmt++;
    char *str;
    if constexpr (std::is_same<T, const char *>::value ||
                  std::is_same<T, char *>::value) {
      str = (char *)val;
    } else if constexpr (std::is_integral<T>::value) {
      char buf[50];
      memset(&buf, 0, 50);
      to_string<int>(buf, (long)val);
      str = (char *)buf;
    } else if constexpr (std::is_floating_point<T>::value) {
      char buf[50];
      to_string<double>(buf, val);
      str = (char *)buf;
    }

    while (*str != '\0') {
      *buffer++ = *str++;
      printed_chars++;
    }

    while (*fmt != '%' && *fmt != '\0') {
      *buffer++ = *fmt++;
      printed_chars++;
    }

  } else {

    while (*fmt != '%' && *fmt != '\0') {
      *buffer++ = *fmt++;
      printed_chars++;
    }
    if (*fmt == '%') {
      fmt++;
      char *str;
      if constexpr (std::is_same<T, const char *>::value ||
                    std::is_same<T, char *>::value) {
        str = (char *)val;
      } else if constexpr (std::is_integral<T>::value) {
        char buf[50];
        memset(&buf, 0, 50);
        to_string<int>(buf, (long)val);
        str = (char *)buf;
      } else if constexpr (std::is_floating_point<T>::value) {
        char buf[50];
        to_string<double>(buf, val);
        str = (char *)buf;
      }

      while (*str != '\0') {
        *buffer++ = *str++;
        printed_chars++;
      }

      while (*fmt != '%' && *fmt != '\0') {
        *buffer++ = *fmt++;
        printed_chars++;
      }
    }
  }

  printed_chars += sprintf(buffer, fmt, args...);
  return printed_chars;
}

template <typename... Args>
static void SetElementText(const char *id, const char *fmt, Args &&...args) {
  char buffer[4096];
  sprintf(buffer, fmt, args...);
  env_setelementtext(id, buffer);
}
} // namespace std