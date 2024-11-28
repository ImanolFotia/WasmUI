#pragma once

#include "defs.hpp"

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
  arena->data = (void *)global_heap_position;
  global_heap_position += capacity;
}

void *alloc_arena(Arena *arena, size_t size) {
  if (size + arena->size >= arena->capacity)
    return nullptr;
  void *ptr = (char *)arena->data + arena->size;
  arena->size += size;
  return ptr;
}

void reset_arena(Arena *arena) { arena->size = 0; }
}

extern "C" {
#ifndef ARENAS
#define ARENAS
Arena transfer_arena;
Arena io_arena;
#endif

typedef struct File_t *FILE;

__attribute__((constructor)) static void arena_layout() {
  create_arena(&transfer_arena, 64 * 1024 * 1024);
  create_arena(&io_arena, sizeof(FILE) * 1024 + 8 * 1024 * 1024);
}
}