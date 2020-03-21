#pragma once

#include <libterminal/Terminal.h>

namespace x86
{

enum CGAColor
{
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,

    BRIGHT_BLACK,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    BRIGHT_BROWN,
    BRIGHT_GREY,
};

struct __packed CGACell
{
public:
    char codepoint;
    CGAColor foreground : 4;
    CGAColor background : 4;
    
    CGACell() {}

    CGACell(char codepoint, CGAColor foreground, CGAColor background)
        : codepoint(codepoint),
          foreground(foreground),
          background(background) {}

    void clear()
    {
        codepoint = ' ';
    }
};

class CGATerminal : public libterminal::Terminal
{
private:
    CGACell *_cells;

public:
    CGATerminal(void *addr) : libterminal::Terminal(80, 25), _cells(reinterpret_cast<CGACell *>(addr))
    {
        clear_memory();
        enable_cursor();
    }

    ~CGATerminal() {}

    void clear_memory();

    void enable_cursor();

    void disable_cursor();

    void on_cell_updated(int x, int y, libterminal::Cell cell);

    void on_cursor_moved(libterminal::Cursor cursor);
};

} // namespace x86
