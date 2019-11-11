#include <stddef.h>
#include <stdint.h>

#include <arch/x86/CGAScreen.h>
#include <arch/x86/SerialStream.h>

size_t strlen(const char *str)
{
    size_t len = 0;

    while (str[len])
    {
        len++;
    }

    return len;
}

static size_t terminal_row = 0;

static size_t terminal_column = 0;

static CGAColor terminal_foreground = CGAColor::LIGHT_GREY;

static CGAColor terminal_background = CGAColor::BLACK;

static CGAScreen terminal_screen;

void terminal_clear()
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_screen.clear(terminal_foreground, terminal_background);
}

void terminal_set_color(CGAColor foreground, CGAColor background)
{
    terminal_foreground = foreground;
    terminal_background = background;
}

void terminal_write(int codepoint)
{
    if (codepoint == '\n')
    {
        terminal_column = 0;
        if (++terminal_row == CGA_HEIGHT)
        {
            terminal_row = CGA_HEIGHT - 1;
            terminal_screen.scroll(terminal_foreground, terminal_background);
        }
    }
    else
    {
        terminal_screen.set(terminal_column, terminal_row, codepoint, terminal_foreground, terminal_background);

        if (++terminal_column == CGA_WIDTH)
        {
            terminal_column = 0;
            if (++terminal_row == CGA_HEIGHT)
                terminal_row = 0;
        }
    }
}

void terminal_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        terminal_write(data[i]);
}

void terminal_write(const char *data)
{
    terminal_write(data, strlen(data));
}

extern "C" void arch_main(void)
{
    terminal_screen = CGAScreen((void *)0xB8000);
    auto serial = SerialStream(SerialPort::COM1);

    /* Initialize terminal interface */
    terminal_set_color(CGAColor::DARK_GREY, CGAColor::WHITE);
    terminal_clear();

    while (true)
    {
        terminal_write("Hello, kernel World!\n");
        serial.write((void *)"Hello, world!\n", sizeof("Hello, world!\n") - 1);

        terminal_write("Hello, other kernel World!\n");
        serial.write((void *)"Hello, other world!\n", sizeof("Hello, other world!\n") - 1);
    }
}