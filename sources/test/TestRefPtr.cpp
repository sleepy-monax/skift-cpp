#include <assert.h>
#include <stdio.h>

#include <libruntime/Move.h>
#include <libruntime/RefCounted.h>
#include <libruntime/RefPtr.h>
#include <libruntime/Macros.h>

using namespace libruntime;

static int object_instance_count = 0;

class Object : public libruntime::RefCounted<Object>
{
private:
    int _x;

public:
    int x() { return _x; }
    Object(int x) : _x(x) { object_instance_count++; }
    ~Object() { object_instance_count--; }
};

void test_coping_ref()
{
    // Regular constructor
    RefPtr<Object> a = make<Object>(10);
    assert(a.refcount() == 1);
    assert(object_instance_count == 1);

    // Copy constructor
    RefPtr<Object> b = a;
    assert(b.refcount() == 2);
    assert(object_instance_count == 1);

    // Move constructor
    RefPtr<Object> c = libruntime::move(b);
    assert(c.refcount() == 2);
    assert(object_instance_count == 1);

    // Copy assignment
    b = a;
    assert(b.refcount() == 3);
    assert(object_instance_count == 1);

    // Move assignment
    b = libruntime::move(a);
    assert(b.refcount() == 2);
    assert(object_instance_count == 1);
}

int main(int argc, char const *argv[])
{
    __unused(argc);
    __unused(argv);

    assert(object_instance_count == 0);
    test_coping_ref();
    assert(object_instance_count == 0);

    return 0;
}
