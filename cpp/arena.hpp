#pragma once

#include "defs.hpp"
#include "string.hpp"

extern "C" {
extern unsigned char __heap_base;

unsigned char *global_heap_position = &__heap_base;

struct Arena {
  size_t capacity{};
  size_t size{};
  void *data{};
};

void create_arena(Arena *arena, size_t capacity) {
  arena->capacity = capacity;
  arena->data = (void*)global_heap_position;
  global_heap_position += capacity;
}

void *alloc_arena(Arena *arena, size_t size) {
 if (size + arena->size >= arena->capacity)
    return nullptr;
  void* ptr = (char*)arena->data + arena->size;
  arena->size += size;
  return ptr;
}

void reset_arena(Arena *arena) { arena->size = 0; }
}