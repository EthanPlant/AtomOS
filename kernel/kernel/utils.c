#include <limits.h>
#include <kernel/utils.h>

void hex_to_ascii(int n, int digits, char *str)
{
    int tmp;
    int index = 2;
    str[0] = '0';
    str[1] = 'x';

    // Construct our string
    for (int i = (4 * digits) - 4; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp > 0xA) str[index++] = tmp - 0xA + 'a';
        else str[index++] = tmp + '0';
    }
    tmp = n & 0xF;
    if (tmp > 0xA) str[index++] = tmp - 0xA + 'a';
    else str[index++] = tmp + '0';
    str[index] = '\0'; // Null-terminate our string
}