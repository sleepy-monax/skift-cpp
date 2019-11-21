#include <assert.h>
#include <libruntime/Macros.h>
#include <libruntime/LinkedList.h>

int main(int argc, char const *argv[])
{
    __unused(argc);
    __unused(argv);

    // Empty list

    libruntime::LinkedList<int> list{};

    assert(!list.any());
    assert(list.empty());
    assert(list.count() == 0);

    // List with elements
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);

    assert(list.containe(1));
    assert(list.containe(2));
    assert(list.containe(3));
    assert(list.containe(4));
    assert(list.containe(5));
    assert(!list.containe(6));

    assert(list.any());
    assert(!list.empty());
    assert(list.count() == 5);

    list.remove_all(1);
    list.remove_all(2);
    list.remove_all(3);
    list.remove_all(4);
    list.remove_all(5);
    list.remove_all(6);

    assert(!list.any());
    assert(list.empty());
    assert(list.count() == 0);

    return 0;
}
