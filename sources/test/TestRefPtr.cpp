#include <assert.h>
#include <stdio.h>
#include <libruntime/RefCounted.h>
#include <libruntime/RefPtr.h>

using namespace libruntime;

static int object_instance_count = 0;

class Object : public libruntime::RefCounted<Object>
{
private:
    int _x;

public:
    Object(int x) : _x(x) { object_instance_count++; }
    ~Object() { object_instance_count--; }
};

void test_coping_ref()
{
    assert(object_instance_count == 0);

    RefPtr<Object> a = make<Object>(10);

    assert(a.refcount() == 1);
    assert(object_instance_count == 1);

    RefPtr<Object> b = a;

    assert(b.refcount() == 2);
    assert(b.refcount() == a.refcount());
    assert(object_instance_count == 1);

    RefPtr<Object> c = b;
    assert(c.refcount() == 3);
    assert(c.refcount() == a.refcount());
    assert(c.refcount() == b.refcount());
    assert(object_instance_count == 1);

    assert(a.refcount() == 3);

    a = make<Object>(1234);
    assert(a.refcount() == 1);
    assert(b.refcount() == 2);
}

RefPtr<Object> gimme_that_ref()
{
    return make<Object>(42);
}

void test_ref_from_function()
{
    auto a = gimme_that_ref();

    assert(a.refcount() == 1);
    assert(object_instance_count == 1);
}

int main(int argc, char const *argv[])
{
    test_coping_ref();
    assert(object_instance_count == 0);

    test_ref_from_function();
    assert(object_instance_count == 0);

    return 0;
}
