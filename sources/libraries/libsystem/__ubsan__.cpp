/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libruntime/Types.h>
#include <libsystem/Logger.h>

#define UBSAN_LOG(__args...) \
    libsystem::log(libsystem::LogLevel::ERROR, data->location.filename, data->location.line, __args);

struct UbsanSourceLocation
{
    char *filename;
    uint32_t line;
    uint32_t column;
};

struct UbsanTypeDescriptor
{
    uint16_t type_kind;
    uint16_t type_info;
    uint8_t type_name[1];
};

struct UbsanTypeMismatchData_v1
{
    UbsanSourceLocation location;
    UbsanTypeDescriptor *type;
    uint8_t log_alignment;
    uint8_t type_check_kind;
};

extern "C" void __ubsan_handle_type_mismatch_v1(UbsanTypeMismatchData_v1 *data, unsigned long ptr)
{
    (void)ptr;

    if (ptr == 0)
    {
        UBSAN_LOG("Null pointer of type {} at {#x}", ptr, reinterpret_cast<char *>(&data->type->type_name[0]));
    }
    else if ((data->log_alignment - 1) & ptr)
    {
        UBSAN_LOG("Miss aligned address {#x} of type {} wich requireds {} byte alignment", ptr, reinterpret_cast<char *>(&data->type->type_name[0]), data->log_alignment);
    }
    else
    {
        UBSAN_LOG("Not enought space at {#x} for {}", ptr, reinterpret_cast<char *>(&data->type->type_name[0]));
    }

    assert_not_reached();
}

struct UbsanInvalidValueData
{
    UbsanSourceLocation location;
    UbsanTypeDescriptor *type;
};

extern "C" void __ubsan_handle_load_invalid_value(UbsanInvalidValueData *data, unsigned long value)
{
    UBSAN_LOG("Load of value {#x} is not a valid value for type {}", value, reinterpret_cast<char *>(&data->type->type_name[0]));
    assert_not_reached();
}

struct UbsanOutOfBoundsData
{
    UbsanSourceLocation location;
    UbsanTypeDescriptor *array_type;
    UbsanTypeDescriptor *index_type;
};

extern "C" void __ubsan_handle_out_of_bounds(UbsanOutOfBoundsData *data, unsigned long index)
{
    (void)index;

    libsystem::log(libsystem::LogLevel::ERROR, data->location.filename, data->location.line, "Out of bounds");
}

struct UbsanPointerOverflowData
{
    UbsanSourceLocation location;
};

extern "C" void __ubsan_handle_pointer_overflow(UbsanPointerOverflowData *data, unsigned long base, unsigned long result)
{
    (void)base;
    (void)result;

    libsystem::log(libsystem::LogLevel::ERROR, data->location.filename, data->location.line, "Pointer overflow");
}

struct UbsanOverflowData
{
    UbsanSourceLocation location;
    UbsanTypeDescriptor *type;
};

extern "C" void __ubsan_handle_add_overflow(UbsanOverflowData *data, unsigned long ulLHS, unsigned long ulRHS)
{
    (void)ulLHS;
    (void)ulRHS;

    libsystem::log(libsystem::LogLevel::ERROR, data->location.filename, data->location.line, "Add overflow");
}

extern "C" void __ubsan_handle_sub_overflow(UbsanOverflowData *data, unsigned long ulLHS, unsigned long ulRHS)
{
    (void)ulLHS;
    (void)ulRHS;

    libsystem::log(libsystem::LogLevel::ERROR, data->location.filename, data->location.line, "Sub overflow");
}

extern "C" void __ubsan_handle_mul_overflow(UbsanOverflowData *data, unsigned long ulLHS, unsigned long ulRHS)
{
    (void)ulLHS;
    (void)ulRHS;

    libsystem::log(libsystem::LogLevel::ERROR, data->location.filename, data->location.line, "Mul overflow");
}

extern "C" void __ubsan_handle_divrem_overflow(UbsanOverflowData *data, unsigned long ulLHS, unsigned long ulRHS)
{
    (void)ulLHS;
    (void)ulRHS;

    libsystem::log(libsystem::LogLevel::ERROR, data->location.filename, data->location.line, "Divrem overflow");
}
