#include "string.hpp"

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

