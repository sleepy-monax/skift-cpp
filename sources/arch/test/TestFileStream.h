#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

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

    libruntime::Error write_byte(byte byte) override
    {
        fputc(byte, _file);

        return libruntime::Error::SUCCEED;
    }
};
