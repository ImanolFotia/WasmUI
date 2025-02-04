#include "string.hpp"


namespace std {
void* memset(void* dest, int ch, size_t count) {
  unsigned char* arr = (unsigned char*)dest;
  for(int i = 0; i < count; i++)
    arr[i] = ch;

  return dest;
}

void* memcpy(void* dest, void *src, size_t count) {
  char* dest_c = (char*)dest;
  const char* src_c = (char*)src;
  for(int i = 0; i < count; i++) {
    dest_c[i] = src_c[i];
  }
  return dest;
}

size_t strlen(const char* str) {
    const char *end = str;
    while (*end != '\0') ++end;
    return end-str;
}

template <> char *to_string<int>(char *buffer, int number, int base) {
  if (number == 0) {
    *buffer = '0';
    buffer++;
    *buffer = '\0';
    return buffer;
  }
  int num = Math::trunc(Math::log10(number) + 1);
  for (int i = num; i > 0; i--) {
    *buffer = '0' + number / ((int)Math::pow(base, i - 1)) % base;
    buffer++;
  }
  *buffer = '\0';
  return buffer;
}

template <> char *to_string<double>(char *buffer, double number, int base) {
  if (number == 0) {
    *buffer++ = '0';
    return buffer;
  }
  // integer part
  int num = 1;
  if (number >= 1.0) {
    num = Math::trunc(Math::log10(number) + 1);
  }
  if (num == 0) {
    *buffer++ = '0';
  } else {
    for (int i = num; i > 0; i--) {
      *buffer++ = '0' + (int)number / ((int)Math::pow(base, i - 1)) % base;
    }
  }
  *buffer++ = '.';
  // decimal part
  double f = number - (int)(number);
  f = f * 100000;
  num = 5;
  for (int i = num; i > 0; i--) {
    *buffer = '0' + (int)f / ((int)Math::pow(base, i - 1)) % base;
    buffer++;
  }
  *buffer = '\0';
  return buffer;
}


}