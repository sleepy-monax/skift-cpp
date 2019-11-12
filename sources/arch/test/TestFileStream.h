#pragma once

#include <cstdio>
#include <libsystem/Stream.h>

class TestFileStream : public libsystem::Stream
{
private:
    FILE *_file;

public:
    TestFileStream(const char *path, const char *attr)
    {
        _file = fopen(path, attr);
    }

    TestFileStream(FILE *file) : _file(file) {}

    ~TestFileStream() {}

    bool writable() override
    {
        return true;
    }

    virtual libruntime::Error write_byte(byte byte)
    {
        fputc(byte, _file);

        return libruntime::Error::SUCCEED;
    }
};
