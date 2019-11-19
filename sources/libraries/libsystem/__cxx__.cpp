#include <libruntime/Types.h>
#include <libruntime/Macros.h>
#include <libsystem/Assert.h>

void *operator new(size_t size)
{
    __unused(size);
    assert_not_reached();
}

void *operator new[](size_t size)
{
    __unused(size);
    assert_not_reached();
}

void operator delete(void *ptr)
{
    __unused(ptr);

    assert_not_reached();
}

void operator delete[](void *ptr)
{
    __unused(ptr);

    assert_not_reached();
}

void operator delete(void *ptr, size_t size)
{
    __unused(ptr);
    __unused(size);
    assert_not_reached();
}

void operator delete[](void *ptr, size_t size)
{
    __unused(ptr);
    __unused(size);
    assert_not_reached();
}

extern "C" void __cxa_pure_virtual()
{
    assert_not_reached();
}