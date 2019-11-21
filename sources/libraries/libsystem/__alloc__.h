#pragma once

#include <libruntime/Types.h>

namespace __alloc__
{

extern "C" void *malloc(size_t size);

extern "C" void free(void *ptr);

extern "C" void *calloc(size_t nobj, size_t size);

extern "C" void *realloc(void *p, size_t size);

} // namespace __alloc__
