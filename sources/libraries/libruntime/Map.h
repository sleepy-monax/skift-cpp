#pragma once

#include <libruntime/Vector.h>

namespace libruntime
{

template <typename KeyType, typename ValueType>
class KeyValuePair
{
private:
    KeyType _key;
    ValueType _value;

public:
    KeyType &key() { return _key; }

    ValueType &value() { return _value; }

    Entry(KeyType key, ValueType value) : _key(key), _value(value) {}

    ~Entry() {}

    Entry(KeyValuePair &other) : _key(other.key()), _value(other.value()) {}

    Entry(KeyValuePair &&other) : _key(move(other.key())),
                                  _value(move(other.value())) {}

    KeyValuePair &operator=(KeyValuePair &other)
    {
        _key = other.key();
        _value = other.value();

        return *this;
    }

    KeyValuePair &operator=(KeyValuePair &&other)
    {
        if (this != &other)
        {
            _key = move(other.key());
            _value = move(other.value());
        }

        return *this;
    }
};

template <typename KeyType, typename ValueType>
class Map
{
private:
    Vector<KeyValuePair<KeyType, ValueType>> _entries;

public:
    Map() : _entries() {}

    ~Map() {}

    void remove(KeyType &key)
    {
        for (size_t i = 0; i < _entries.count(); i++)
        {
            if (_entries[i].key() == key)
            {
                _entries.remove(i);
            }
        }
    }

    void put(KeyType &key, ValueType &value)
    {
        for (size_t i = 0; i < _entries.count(); i++)
        {
            if (_entries[i].key() == key)
            {
                _entries[i] = KeyValuePair(key, value);

                return;
            }
        }

        _entries.pushback(KeyValuePair(key, value));

        return;
    }

    bool get(KeyType &key, ValueType &value)
    {
        for (size_t i = 0; i < _entries.count(); i++)
        {
            if (_entries[i].key() == key)
            {
                value = _entries[i].value();

                return true;
            }
        }

        return false;
    }

    bool containe(KeyType &key)
    {
        for (size_t i = 0; i < _entries.count(); i++)
        {
            if (_entries[i].key() == key)
            {
                return true;
            }
        }

        return false;
    }

    template <typename Callback>
    void foreach (Callback callback)
    {
        for (size_t i = 0; i < _entries.count(); i++)
        {
            if (callback(_entries[i]) == Iteration::STOP)
            {
                return;
            }
        }
    }
};

} // namespace libruntime
