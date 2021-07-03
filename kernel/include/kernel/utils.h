#ifndef _UTILS_H
#define _UTILS_H

void hex_to_ascii(int n, int digits, char *str);

void reverse(char *str, int length);
char *itoa(int value, char* buffer, int base);

void panic(char* file, char *panic_msg, int line);

#define assert(b) ((b) ? (void)0 : panic(__FILE__, "Assertion failed!", __LINE__))

#endif