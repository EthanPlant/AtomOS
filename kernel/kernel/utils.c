#include <limits.h>
#include <stdbool.h>

#include <kernel/utils.h>
#include <kernel/tty.h>

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
void panic(char *file, char *panic_msg, char *line)
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
    // TODO: Dump trace
    terminal_newline();
    terminal_newline();
    terminal_writestring("    System halted!\n");
    terminal_newline();
    terminal_setcolor(VGA_COLOR_BROWN, VGA_COLOR_BLACK);
    terminal_writestring("--------------------------------------------------------------------------------\n");
    while (1);
}