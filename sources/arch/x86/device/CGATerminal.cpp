#include "arch/x86/device/CGATerminal.h"
#include "arch/x86/x86.h"

namespace x86
{

CGAColor terminal_color_to_cga(libterminal::Color color)
{
    switch (color)
    {
    case libterminal::Color::BLACK:
        return CGAColor::BLACK;

    case libterminal::Color::RED:
        return CGAColor::RED;

    case libterminal::Color::GREEN:
        return CGAColor::GREEN;

    case libterminal::Color::YELLOW:
        return CGAColor::BROWN;

    case libterminal::Color::BLUE:
        return CGAColor::BLUE;

    case libterminal::Color::MAGENTA:
        return CGAColor::MAGENTA;

    case libterminal::Color::CYAN:
        return CGAColor::CYAN;

    case libterminal::Color::GREY:
        return CGAColor::GREY;

    case libterminal::Color::BRIGHT_BLACK:
        return CGAColor::BRIGHT_BLACK;

    case libterminal::Color::BRIGHT_RED:
        return CGAColor::BRIGHT_RED;

    case libterminal::Color::BRIGHT_GREEN:
        return CGAColor::BRIGHT_GREEN;

    case libterminal::Color::BRIGHT_YELLOW:
        return CGAColor::BRIGHT_BROWN;

    case libterminal::Color::BRIGHT_BLUE:
        return CGAColor::BRIGHT_BLUE;

    case libterminal::Color::BRIGHT_MAGENTA:
        return CGAColor::BRIGHT_MAGENTA;

    case libterminal::Color::BRIGHT_CYAN:
        return CGAColor::BRIGHT_CYAN;

    case libterminal::Color::BRIGHT_GREY:
        return CGAColor::BRIGHT_GREY;

    default:
        return CGAColor::MAGENTA;
    }
}

void CGATerminal::clear_memory()
{
    for (int i = 0; i < width() * height(); i++)
    {
        _cells[i].clear();
    }
}

void CGATerminal::enable_cursor()
{
    out8(0x3D4, 0x0A);
    out8(0x3D5, (in8(0x3D5) & 0xC0) | 0);
    out8(0x3D4, 0x0B);
    out8(0x3D5, (in8(0x3D5) & 0xE0) | 15);
}

void CGATerminal::disable_cursor()
{
    out8(0x3D4, 0x0A);
    out8(0x3D5, 0x20);
}

void CGATerminal::on_cell_updated(int x, int y, libterminal::Cell cell)
{
    _cells[y * width() + x] = CGACell(
        cell.codepoint().to_cp437(),
        terminal_color_to_cga(cell.attributes().foreground()),
        terminal_color_to_cga(cell.attributes().background()));
}

void CGATerminal::on_cursor_moved(libterminal::Cursor cursor)
{
    uint16_t pos = cursor.Y() * width() + cursor.X();

    out8(0x3D4, 0x0F);
    out8(0x3D5, (uint8_t)(pos & 0xFF));
    out8(0x3D4, 0x0E);
    out8(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

} // namespace x86