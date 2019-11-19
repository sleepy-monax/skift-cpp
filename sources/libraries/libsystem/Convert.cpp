#include <libsystem/Assert.h>
#include <libsystem/Convert.h>
#include <libc/string.h>

static const char *basechar = "0123456789abcdefghijklmnopqrstuvwxyz";
static const char *basechar_maj = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

uint libsystem::convert_string_to_uint(const char *str, int base)
{
    assert(str);
    assert(base > 0 && base < 36);

    uint value = 0;

    for (int i = 0; str[i]; i++)
    {
        value = value * base;

        for (int j = 0; j < base; j++)
        {
            if ((basechar[j] == str[i]) || (basechar_maj[j] == str[i]))
            {
                value += j;
            }
        }
    }

    return value;
}

int libsystem::convert_uint_to_string(uint value, char *str, int base)
{
    assert(str);
    assert(base > 0 && base < 36);

    str[0] = '\0';
    if (value == 0)
    {
        libc::strapd(str, '0');
        return 1;
    }

    int written = 0;

    while (value != 0)
    {
        libc::strapd(str, basechar_maj[value % base]);
        value /= base;

        written++;
    }

    libc::strrvs(str);

    return written;
}