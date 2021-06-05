#include <stdbool.h>
#include <string.h>

#include <kernel/arch.h>
#include <kernel/timer.h>
#include <kernel/tty.h>
#include <kernel/utils.h>

// Creates a formatted hex string with a certain amount of digits
void hex_to_ascii(int n, int digits, char *str)
{
    // str = "";
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
char* itoa(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32) {
        return buffer;
    }
 
    // consider the absolute value of the number
    int n;
    if (value < 0) n = -value;
    else n = value;
 
    int i = 0;
    while (n)
    {
        int r = n % base;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / base;
    }
 
    // if the number is 0
    if (i == 0) {
        buffer[i++] = '0';
    }
 
    // If the base is 10 and the value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }
 
    buffer[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    reverse(buffer, i);

    return buffer;
}

const char *witty_comments[] = 
{
    "Somebody set up us the bomb",
    "Everything's going to plan. No, really, that was supposed to happen.",
    "Uh... Did I do that?",
    "Oops.",
    "Why did you do that?",
    "I feel sad now :(",
    "My bad.",
    "I'm sorry, Dave",
    "I let you down. Sorry :(",
    "On the bright side, I bought you a teddy bear!",
    "Oh - I know what I did wrong!",
    "Hey, that tickles!",
    "Don't be sad. I'll do better next time, I promise!",
    "Don't be sad, have a hug! <3",
    "I just don't know what went wrong :(",
    "Shall we play a game?",
    "Quite honestly, I wouldn't worry myself about that",
    "Sorry :(",
    "This doesn't make any sense!",
    "Why is it breaking :(",
    "Don't do that.",
    "Ouch. That hurt :(",
    "You're mean .",
    "But it works on my machine",
    "404: Comment not found",
    "Probably nothing to worry about",
    "I'd like to interject for a moment",
    "The code monkeys at our headquarters are working VEWY HAWD to fix this!"
};

// Kernel panic screen
void panic(char *file, char *panic_msg, int line)
{
    terminal_clear();
    terminal_setcolor(VGA_COLOR_BROWN, VGA_COLOR_BLACK);
    terminal_writestring("---");
    terminal_setcolor(VGA_COLOR_RED, VGA_COLOR_BLACK);
    terminal_writestring(" !!! ");
    terminal_setcolor(VGA_COLOR_BROWN, VGA_COLOR_BLACK);
    terminal_writestring("------------------------------------------------------------------------\n");
    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_writestring("    KERNEL PANIC\n");
    terminal_writestring("    // ");
    terminal_writestring(witty_comments[get_ticks() % 28]);
    terminal_newline();
    terminal_newline();
    terminal_setcolor(VGA_COLOR_RED, VGA_COLOR_BLACK);
    terminal_writestring("    ");
    terminal_writestring(panic_msg);
    terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_newline();
    terminal_writestring("    at ");
    terminal_writestring(file);
    terminal_writestring(":");
    char *line_str = "";
    itoa(line, line_str, 10);
    terminal_writestring(line_str);
    terminal_newline();
    terminal_newline();
    terminal_writestring("    Diagnostic:\n");
    terminal_writestring("    System was running for ");
    char* tick_str = "";
    itoa(get_ticks(), tick_str, 10);
    terminal_writestring(tick_str);
    terminal_writestring(" ticks.\n");
    terminal_newline();
    terminal_writestring("    Context:\n");
    terminal_writestring("    ");
    print_registers();
    terminal_newline();
    terminal_newline();
    terminal_writestring("    System halted!\n");
    terminal_newline();
    terminal_setcolor(VGA_COLOR_BROWN, VGA_COLOR_BLACK);
    terminal_writestring("--------------------------------------------------------------------------------\n");
    while (1);
}