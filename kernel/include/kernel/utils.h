#ifndef _UTILS_H
#define _UTILS_H

void reverse(char *str, int length);
char* itoa(int num, char *str, int base);

void panic(char* file, char *panic_msg, char *line);

#endif