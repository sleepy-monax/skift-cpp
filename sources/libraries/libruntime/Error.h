#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

namespace libruntime
{

#define ERROR_LIST(__ITEM)  \
    __ITEM(SUCCEED)         \
    __ITEM(NOT_IMPLEMENTED) \
    __ITEM(READ_ONLY)       \
    __ITEM(WRITE_ONLY)      \
    __ITEM(END_OF_STREAM)   \
    __ITEM(NO_SUCH_THREAD)  \
    __ITEM(NO_CHILD_THREAD) \
    __ITEM(NO_SUCH_PROCESS) \
    __ITEM(NO_CHILD_PROCESS)

enum class Error
{
#define ERROR_ENUM_ENTRY(__entry) __entry,
    ERROR_LIST(ERROR_ENUM_ENTRY)
};

} // namespace libruntime
