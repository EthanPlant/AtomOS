#include <limits.h>
#include <stdbool.h>

#include <kernel/utils.h>

void reverse(char *str, int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end)
    {
        char c = str[start];
        str[start] = str[end];
        str[end] = c;
        ++start;
        --end;
    }
}

// Not technically part of the C standard so here for now instead of in libc
// Converts an integer to a string
char* itoa(int num, char *str, int base)
{
    int i = 0;
    bool isNegative = false;

    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num /= base;
    }

    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';
    reverse(str, i);

    return str;
}