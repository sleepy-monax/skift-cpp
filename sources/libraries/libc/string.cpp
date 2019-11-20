#include <libc/string.h>

extern "C"
{
    size_t strlen(const char *str)
    {
        size_t len = 0;

        while (str[len])
        {
            len++;
        }

        return len;
    }

    void strapd(char *str, char c)
    {
        unsigned int len = strlen(str);
        str[len] = c;
        str[len + 1] = '\0';
    }

    void strrvs(char *str)
    {
        int c, i, j;
        for (i = 0, j = strlen(str) - 1; i < j; i++, j--)
        {
            c = str[i];
            str[i] = str[j];
            str[j] = c;
        }
    }

    void *memset(void *dest, int ch, size_t size)
    {
        for (size_t i = 0; i < size; i++)
            ((char *)dest)[i] = ch;
        return dest;
    }

    void *memcpy(void *dest, const void *src, size_t size)
    {
        for (size_t i = 0; i < size; i++)
            ((char *)dest)[i] = ((char *)src)[i];
        return dest;
    }

    void *memmove(void *dest, const void *src, size_t size)
    {
        if (dest <= src)
        {
            for (size_t i = 0; i < size; i++)
                ((char *)dest)[i] = ((char *)src)[i];
        }
        else
        {
            for (int i = size - 1; i >= 0; i--)
                ((char *)dest)[i] = ((char *)src)[i];
        }
        return dest;
    }

    int memcmp(const void *ptr1, const void *ptr2, size_t n)
    {
        const char *iter1 = (const char *)ptr1;
        const char *iter2 = (const char *)ptr2;

        while (n > 0 && *iter1 == *iter2)
        {
            iter1++;
            iter2++;
            n--;
        }

        return n == 0 ? 0 : (*iter1 - *iter2);
    }
}
