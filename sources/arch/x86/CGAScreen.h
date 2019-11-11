#pragma once

#include <stddef.h>

#include <libruntime/Macros.h>

static const size_t CGA_WIDTH = 80;

static const size_t CGA_HEIGHT = 25;

/* Hardware text mode color constants. */
enum CGAColor
{
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHT_GREY,
    DARK_GREY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    LIGHT_BROWN,
    WHITE,
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

class __packed CGALine
{
private:
    CGACell _cells[CGA_WIDTH];

public:
    CGALine(CGAColor foreground, CGAColor background)
    {
        for (int i = 0; i < CGA_WIDTH; i++)
        {
            _cells[i] = CGACell(' ', foreground, background);
        }
    }

    void set(int x, int codepoint, CGAColor foreground, CGAColor background)
    {
        _cells[x] = CGACell(codepoint, foreground, background);
    }
};

class __packed CGAScreen
{
private:
    CGALine *_lines;

public:
    CGAScreen()
    {
    }

    CGAScreen(void *buffer)
    {
        _lines = (CGALine *)buffer;
    }

    int width()
    {
        return 80;
    }

    int height()
    {
        return 25;
    }

    void clear(CGAColor foreground, CGAColor background)
    {
        for (int i = 0; i < height(); i++)
        {
            _lines[i] = CGALine(foreground, background);
        }
    }

    void scroll(CGAColor foreground, CGAColor background)
    {
        for (int i = 0; i < height() - 1; i++)
        {
            _lines[i] = _lines[i + 1];
        }

        _lines[height() - 1] = CGALine(foreground, background);
    }

    void set(int x, int y, int codepoint, CGAColor foreground, CGAColor background)
    {
        _lines[y].set(x, codepoint, foreground, background);
    }
};