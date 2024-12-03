#pragma once

#include <std/type_traits.hpp>
#include <string.hpp>

static int tprintf(const char* fmt, char* buffer) {
    *buffer = *fmt;
    return 1;
}

template <typename T, typename... Args>
static int tprintf(const char* fmt, char* buffer, T val, Args&&... args) {
    int printed_chars = 0;
    if (*fmt == '%') {
        fmt++;
        if (std::is_same<T, const char*>::value || std::is_same<T, char*>::value) {
            char* str = (char*)val;
            // puts(val);
            while (*str != '\0') {
                *buffer = *str;
                buffer++;
                printed_chars++;
                str++;
            }
        } else if (std::is_integral<T>::value) {
        } else if (std::is_floating_point<T>::value) {
        }

        while (*fmt != '%' && *fmt != '\0') {
            *buffer = *fmt;
            buffer++;
            printed_chars++;
            fmt++;
        }
    }
    printed_chars += tprintf(fmt, buffer, args...);
    return printed_chars;
}

template <typename... Args>
static int tprintf(const char* str, Args&&... args) {
    char buffer[4096];  // 4Kb printing buffer
    memset(&buffer, 0, 4096);
    char* input = (char*)str;
    int printed_chars = 0;

    printed_chars = tprintf(input, &buffer[0], args...);
    puts(buffer);
    return printed_chars;
}
