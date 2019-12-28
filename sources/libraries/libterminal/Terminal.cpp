/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libterminal/Terminal.h>

namespace libterminal
{

void Terminal::clear(int fromx, int fromy, int tox, int toy)
{
    for (int i = fromx + fromy * _width; i < tox + toy * _width; i++)
    {
        set_cell(i % _width, i / _width, Cell(_current_attributes, U' '));
    }
}

void Terminal::clear_line(int line)
{
    if (line >= 0 && line < _height)
    {
        for (int i = 0; i < _width; i++)
        {
            set_cell(i, line, Cell(_current_attributes, U' '));
        }
    }
}

void Terminal::set_cursor(int x, int y)
{
    _cursor = Cursor(libmath::clamp(x, _width), libmath::clamp(y, _height));

    on_cursor_moved(_cursor);
}

void Terminal::move_cursor(int vx, int vy)
{
    // FIXME: this code look silly...
    
    if (_cursor.X() + vx < 0 || _cursor.X() + vx >= _width)
    {
        _cursor = Cursor((_cursor.X() + vx) % _width, _cursor.Y());
        move_cursor(0, vy + (_cursor.X() + vx) / _width);
    }
    else if (_cursor.Y() + vy < 0)
    {
        scroll(_cursor.Y() + vy);
        _cursor = Cursor(_cursor.X(), 0);
        move_cursor(vx, 0);
    }
    else if (_cursor.Y() + vy >= _height)
    {
        scroll((_cursor.Y() + vy) - (_height - 1));
        _cursor = Cursor(_cursor.X(), _height - 1);
        move_cursor(vx, 0);
    }
    else
    {
        _cursor = Cursor(_cursor.X() + vx,
                         _cursor.Y() + vy);

        on_cursor_moved(_cursor);
    }
}

void Terminal::scroll(int how_many_line)
{
    if (how_many_line < 0)
    {
        for (int line = 0; line < how_many_line; line++)
        {
            for (int i = (_width * _height) - 1; i >= _height; i++)
            {
                int x = i % _width;
                int y = i / _width;

                set_cell(x, y, cell_at(x, y - 1));
            }

            clear_line(0);
        }
    }
    else if (how_many_line > 0)
    {
        for (int line = 0; line < how_many_line; line++)
        {
            for (int i = 0; i < ((_width * _height) - _width); i++)
            {
                int x = i % _width;
                int y = i / _width;

                set_cell(x, y, cell_at(x, y + 1));
            }

            clear_line(_height - 1);
        }
    }
}

void Terminal::new_line()
{
    move_cursor(-_cursor.X(), 1);
}

void Terminal::backspace()
{
    move_cursor(-1, 0);
}

void Terminal::append(libsystem::Codepoint codepoint)
{
    if (codepoint == U'\n')
    {
        new_line();
    }
    else if (codepoint == U'\r')
    {
        move_cursor(-_cursor.X(), 0);
    }
    else if (codepoint == U'\t')
    {
        move_cursor(8 - (_cursor.X() % 8), 0);
    }
    else if (codepoint == U'\b')
    {
        backspace();
    }
    else
    {
        set_cell(_cursor.X(), _cursor.Y(), Cell(_current_attributes, codepoint));
        move_cursor(1, 0);
    }
}

void Terminal::do_ansi(libsystem::Codepoint op, const Parameter *parameters, int parameter_count)
{
    switch (op)
    {
    case U'A':
        if (parameters[0].empty)
        {
            move_cursor(0, -1);
        }
        else
        {
            move_cursor(0, -parameters[0].value);
        }
        break;

    case U'B':
        if (parameters[0].empty)
        {
            move_cursor(0, 1);
        }
        else
        {
            move_cursor(0, parameters[0].value);
        }
        break;

    case U'C':
        if (parameters[0].empty)
        {
            move_cursor(1, 0);
        }
        else
        {
            move_cursor(parameters[0].value, 0);
        }
        break;

    case U'D':
        if (parameters[0].empty)
        {
            move_cursor(-1, 0);
        }
        else
        {
            move_cursor(-parameters[0].value, 0);
        }
        break;

    case U'E':
        if (parameters[0].empty)
        {
            move_cursor(-_cursor.X(), 1);
        }
        else
        {
            move_cursor(-_cursor.X(), parameters[0].value);
        }
        break;

    case U'F':
        if (parameters[0].empty)
        {
            move_cursor(-_cursor.X(), -1);
        }
        else
        {
            move_cursor(-_cursor.X(), -parameters[0].value);
        }
        break;

    case U'G':
        if (parameters[0].empty)
        {
            move_cursor(0, _cursor.Y());
        }
        else
        {
            move_cursor(parameters[0].value - 1, _cursor.Y());
        }
        break;

    case U'f':
    case U'H':
    {
        int row;
        int column;

        if (parameters[0].empty)
        {
            row = 0;
        }
        else
        {
            row = parameters[0].value - 1;
        }

        if (parameters[1].empty)
        {
            column = 0;
        }
        else
        {
            column = parameters[1].value - 1;
        }

        set_cursor(column, row);
    }
    break;

    case U'J':
        if (parameters[0].value == 0)
        {
            clear(_cursor.X(), _cursor.Y(), _width - 1, _height - 1);
        }
        else if (parameters[0].value == 1)
        {
            clear(0, 0, _cursor.X(), _cursor.Y());
        }
        else if (parameters[0].value == 2)
        {
            clear(0, 0, _width - 1, _height - 1);
        }
        break;

    case U'K':
        if (parameters[0].value == 0)
        {
            clear(_cursor.X(), _cursor.Y(), _width - 1, _cursor.Y());
        }
        else if (parameters[0].value == 1)
        {
            clear(0, _cursor.Y(), _cursor.X(), _cursor.Y());
        }
        else if (parameters[0].value == 2)
        {
            clear(0, _cursor.Y(), _width - 1, _cursor.Y());
        }
        break;

    case U'S':
        if (parameters[0].empty)
        {
            scroll(-1);
        }
        else
        {
            scroll(-parameters[0].value);
        }
        break;

    case U'T':
        if (parameters[0].empty)
        {
            scroll(1);
        }
        else
        {
            scroll(parameters[0].value);
        }
        break;

    case U'm':
        for (int i = 0; i < parameter_count; i++)
        {
            if (parameters[i].empty || parameters[i].value == 0)
            {
                _current_attributes = _default_attributes;
            }
            else
            {
                int attr = parameters[i].value;

                if (attr == 1)
                {
                    _current_attributes = _current_attributes.bolded();
                }
                else if (attr >= 30 && attr <= 37)
                {
                    _current_attributes = _current_attributes.with_foreground(static_cast<Color>(attr - 30));
                }
                else if (attr >= 90 && attr <= 97)
                {
                    _current_attributes = _current_attributes.with_foreground(static_cast<Color>(attr - 90 + 8));
                }
                else if (attr >= 40 && attr <= 47)
                {
                    _current_attributes = _current_attributes.with_background(static_cast<Color>(attr - 40));
                }
                else if (attr >= 100 && attr <= 107)
                {
                    _current_attributes = _current_attributes.with_background(static_cast<Color>(attr - 100 + 8));
                }
            }
        }
        break;

    case U's':
        _saved_cursor = _cursor;
        break;

    case U'u':
        _cursor = _saved_cursor;
        break;

    default:
        break;
    }
}

libruntime::Error Terminal::write_codepoint(libsystem::Codepoint codepoint)
{
    switch (_state)
    {
    case ParserState::WAIT_FOR_ESC:
        if (codepoint == U'\e')
        {
            for (int i = 0; i < TERMINAL_PARAMETERS_COUNT; i++)
            {
                _parameters[i].empty = true;
                _parameters[i].value = 0;
                _parameters_top = 0;
            }

            _state = ParserState::ESCPECT_BRACKET;
        }
        else
        {
            _state = ParserState::WAIT_FOR_ESC;
            append(codepoint);
        }
        break;

    case ParserState::ESCPECT_BRACKET:
        if (codepoint == U'[')
        {
            _state = ParserState::READ_ATTRIBUTE;
        }
        else if (codepoint == U'c')
        {
            _current_attributes = _default_attributes;

            _state = ParserState::WAIT_FOR_ESC;
        }
        else
        {
            append(codepoint);
            _state = ParserState::WAIT_FOR_ESC;
        }
        break;

    case ParserState::READ_ATTRIBUTE:
        if (codepoint.is_digit())
        {
            _parameters[_parameters_top].empty = false;
            _parameters[_parameters_top].value *= 10;
            _parameters[_parameters_top].value += codepoint.numeric_value();
        }
        else
        {
            if (codepoint == U';')
            {
                _parameters_top++;
            }
            else
            {
                do_ansi(codepoint, _parameters, _parameters_top + 1);

                _state = ParserState::WAIT_FOR_ESC;
            }
        }
        break;

    default:
        assert_not_reached();
        break;
    }

    return libruntime::Error::SUCCEED;
}

} // namespace libterminal
