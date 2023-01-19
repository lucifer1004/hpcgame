#include "src/core/allocator.h"

#include <stdlib.h>

Allocator _g_default_cpu_allocator = {malloc, free};
