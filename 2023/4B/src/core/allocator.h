#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>

#include "src/core/macros.h"

typedef void* (*RawAlloc)(size_t size);
typedef void (*RawDealloc)(void* ptr);

/**
 * @brief Memory allocator, it keeps the allocator and de-allocator functions.
 */
typedef struct {
  RawAlloc raw_alloc;     /**< Allocator functions*/
  RawDealloc raw_dealloc; /**< De-allocator functions*/
} Allocator;

extern Allocator _g_default_cpu_allocator;

static inline Allocator* aitisa_default_cpu_allocator() {
  return &_g_default_cpu_allocator;
}

static inline void aitisa_set_default_cpu_raw_alloc(RawAlloc raw_alloc) {
  aitisa_default_cpu_allocator()->raw_alloc = raw_alloc;
}

static inline void aitisa_set_default_cpu_raw_dealloc(RawDealloc raw_dealloc) {
  aitisa_default_cpu_allocator()->raw_dealloc = raw_dealloc;
}

#endif
