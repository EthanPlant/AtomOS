#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool print(const char *data, size_t length)
{
    const unsigned char *bytes = (const unsigned char*) data;
    for (size_t i = 0; i < length; ++i)
        if (putchar(bytes[i]) == EOF) 
            return false;

    return true;
}

int printf(const char* restrict format, ...)
{
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0')
    {
        size_t maxrem = INT_MAX - written;

        if (format[0] != '%' || format[1] == '%')
        {
            if (format[0] == '%')
                format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;
            if (maxrem < amount)
            {
                va_end(parameters);
                return -1;
            }
            if (!print(format, amount))
            {
                va_end(parameters);
                return -1;
            }
            format += amount;
            written += amount;
            continue;
        }
        
        const char *format_begun_at = format++;

        if (*format == 'c')
        {
            format++;
            char c = (char) va_arg(parameters, int);
            if (!maxrem)
            {
                va_end(parameters);
                return -1;
            }
            if (!print(&c, sizeof(c)))
            {
                va_end(parameters);
                return -1;
            }
            written++;
        } 
        else if (*format == 's')
        {
            format++;
            const char *str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            if (maxrem < len)
            {
                va_end(parameters);
                return -1;
            }
            if (!print(format, len))
            {
                va_end(parameters);
                return -1;
            }
            written += len;
            format += len;
        }
    }

    va_end(parameters);
    return written;
}