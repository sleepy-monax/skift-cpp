/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/Macros.h>
#include <libruntime/Types.h>
#include <libsystem/Assert.h>
#include <libsystem/Logger.h>
#include <libsystem/__alloc__.h>

#define MAX_AT_EXIT_FUNCTION 128

struct AtExitFunctionEntry
{
    void (*destructor_func)(void *);
    void *obj_ptr;
    void *dso_handle;
};

static size_t __atexit_function_count = 0;
static AtExitFunctionEntry __atexit_function[MAX_AT_EXIT_FUNCTION];

void *__dso_handle;

extern "C" int __cxa_atexit(void (*f)(void *), void *objptr, void *dso)
{
    assert(__atexit_function_count < MAX_AT_EXIT_FUNCTION);

    __atexit_function[__atexit_function_count].destructor_func = f;
    __atexit_function[__atexit_function_count].obj_ptr = objptr;
    __atexit_function[__atexit_function_count].dso_handle = dso;
    __atexit_function_count++;

    return 0;
};

void *operator new(size_t size)
{
    //FIXME HACK: round size to 2 to fix ubsan checks.

    return __alloc__::malloc(size - size % 4 + 4);
}

void *operator new[](size_t size)
{
    //FIXME HACK: round size to 2 to fix ubsan checks.

    return __alloc__::malloc(size - size % 4 + 4);
}

void operator delete(void *ptr)
{
    __alloc__::free(ptr);
}

void operator delete[](void *ptr)
{
    __alloc__::free(ptr);
}

void operator delete(void *ptr, size_t size)
{
    __unused(size);
    __alloc__::free(ptr);
}

void operator delete[](void *ptr, size_t size)
{
    __unused(size);
    __alloc__::free(ptr);
}

extern "C" void __cxa_pure_virtual()
{
    logger_fatal("Attempt at calling a pure virtual function!");
    assert_not_reached();
}