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

class __packed CGACell
{
private:
    char _codepoint;
    CGAColor _foreground : 4;
    CGAColor _background : 4;

public:
    CGACell() {}

    CGACell(int codepoint, CGAColor foreground, CGAColor background)
        : _codepoint(static_cast<char>(codepoint)),
          _foreground(foreground),
          _background(background) {}
};

class CGATerminal : public libterminal::Terminal
{
private:
    CGACell *_cells;

public:
    CGATerminal(void *addr) : libterminal::Terminal(80, 25), _cells(reinterpret_cast<CGACell *>(addr)) {}

    ~CGATerminal() {}

    void on_cell_updated(int x, int y, libterminal::Cell cell);

    void on_cursor_moved(libterminal::Cursor cursor);
};

} // namespace x86
