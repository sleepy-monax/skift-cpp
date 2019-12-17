#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/Iteration.h>
#include <libruntime/Move.h>
#include <libruntime/Types.h>
#include <libsystem/Assert.h>

namespace libruntime
{

template <typename T>
struct LinkedListItem
{
    T value;

    LinkedListItem *prev;
    LinkedListItem *next;

    LinkedListItem(T value) : value(value), prev(nullptr), next(nullptr) {}
};

template <typename T>
class LinkedList
{
private:
    int _count;
    LinkedListItem<T> *_head;
    LinkedListItem<T> *_tail;

public:
    LinkedList()
    {
        _count = 0;
        _head = nullptr;
        _tail = nullptr;
    }

    ~LinkedList()
    {
        clear();
    }

    void clear()
    {
        LinkedListItem<T> *current = _head;

        while (current)
        {
            LinkedListItem<T> *next = current->next;
            delete current;
            current = next;
        }

        _count = 0;
        _head = nullptr;
        _tail = nullptr;
    }

    template <typename Callback>
    void iterate(Callback callback)
    {
        LinkedListItem<T> *current = _head;

        while (current)
        {
            LinkedListItem<T> *next = current->next;

            if (callback(current->value) != Iteration::STOP)
            {
                current = next;
            }
            else
            {
                return;
            }
        }
    }

    template <typename Comparator>
    void sort(Comparator comparator)
    {
        for (LinkedListItem<T> *i = _head; i->next; i = i->next)
        {
            for (LinkedListItem<T> *j = i->next; j; j = j->next)
            {
                if (comparator(i->value, j->value) > 0)
                {
                    T tmp = i->value;
                    i->value = j->value;
                    j->value = tmp;
                }
            }
        }
    }

    int count()
    {
        return _count;
    }

    bool empty() { return _count == 0; }

    bool any() { return _count > 0; }

    T push(T value)
    {
        LinkedListItem<T> *item = new LinkedListItem<T>(libruntime::move(value));

        item->prev = nullptr;

        if (!_head)
        {
            _head = item;
            _tail = item;
        }
        else
        {
            item->next = _head;

            _head->prev = item;
            _head = item;
        }

        _count++;

        return item->value;
    }

    T push_back(T value)
    {
        LinkedListItem<T> *item = new LinkedListItem<T>(libruntime::move(value));

        item->next = nullptr;

        if (!_tail)
        {
            _head = item;
            _tail = item;
        }
        else
        {
            item->prev = _tail;

            _tail->next = item;
            _tail = item;
        }

        _count++;

        return item->value;
    }

    T pop()
    {
        assert(_head);

        LinkedListItem<T> *item = _head;

        if (_count == 1)
        {
            _head = nullptr;
            _tail = nullptr;
        }
        else if (_count > 1)
        {
            item->next->prev = nullptr;
            _head = item->next;
        }

        _count--;

        T value = item->value;
        delete item;
        return value;
    }

    T pop_back()
    {
        assert(_tail);

        LinkedListItem<T> *item = _tail;

        if (_count == 1)
        {
            _head = nullptr;
            _tail = nullptr;
        }
        else if (_count > 1)
        {
            item->prev->next = nullptr;
            _tail = item->prev;
        }

        _count--;

        T value = item->value;
        delete item;
        return value;
    }

    T peek()
    {
        assert(_head);
        return _head->value;
    }

    T peek_back()
    {
        assert(_tail);
        return _tail->value;
    }

    T peek_and_pushback()
    {
        if (_count == 1)
        {
            return peek();
        }
        else
        {
            T value = peek();

            auto *item = _head;

            // Make the next item the first one.
            item->next->prev = NULL;
            _head = item->next;

            // Make the first item the last one.
            item->next = NULL;
            _tail->next = item;
            item->prev = _tail;
            _tail = item;

            return value;
        }
    }

    bool containe(T value)
    {
        LinkedListItem<T> *current = _head;

        while (current)
        {
            LinkedListItem<T> *next = current->next;

            if (current->value == value)
                return true;

            current = next;
        }

        return false;
    }

    void remove_first(T value)
    {
        LinkedListItem<T> *current = _head;

        while (current)
        {
            LinkedListItem<T> *next = current->next;

            if (current->value == value)
            {
                if (current->prev != nullptr)
                {
                    current->prev->next = current->next;
                }
                else
                {
                    _head = current->next;
                }

                if (current->next != nullptr)
                {
                    current->next->prev = current->prev;
                }
                else
                {
                    _tail = current->prev;
                }

                _count--;
                delete current;

                return;
            }

            current = next;
        }
    }

    void remove_all(T value)
    {
        LinkedListItem<T> *current = _head;

        while (current)
        {
            LinkedListItem<T> *next = current->next;

            if (current->value == value)
            {
                if (current->prev != nullptr)
                {
                    current->prev->next = current->next;
                }
                else
                {
                    _head = current->next;
                }

                if (current->next != nullptr)
                {
                    current->next->prev = current->prev;
                }
                else
                {
                    _tail = current->prev;
                }

                _count--;
                delete current;
            }

            current = next;
        }
    }
};

} // namespace libruntime
