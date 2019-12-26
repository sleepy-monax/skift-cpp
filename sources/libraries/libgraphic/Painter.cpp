#include <libgraphic/Painter.h>

namespace libgraphic
{

void Painter::push_clip(Bound clip)
{
    _clip_stack.push(_current_clip);
    _current_clip = clip.clipped_in(_current_clip);
}

void Painter::pop_clip()
{
    _current_clip = _clip_stack.peek();
    _clip_stack.pop();
}

Bound Painter::clip()
{
    return _current_clip;
}

void Painter::clear(Color color)
{
    for (int x = 0; x < _current_clip.width(); x++)
    {
        for (int y = 0; y < _current_clip.width(); y++)
        {
            _surface->set_pixel(_current_clip.location() + Point(x, y), color);
        }
    }
}

void Painter::plot(Point p, Color color)
{
    auto absolute = _current_clip.location() + p;

    if (_current_clip.containe(absolute))
    {
        auto bg = _surface->get_pixel(absolute);
        _surface->set_pixel(absolute, Color::alpha_blend(color, bg));
    }
}

void Painter::draw_line(Point from, Point to, Color color)
{
    auto ax = from.X();
    auto ay = from.Y();
    auto bx = to.X();
    auto by = to.Y();

    int dx = libmath::abs(bx - ax);
    int dy = libmath::abs(by - by);

    int sx = ax < bx ? 1 : -1;
    int sy = ay < by ? 1 : -1;

    int err = (dx > dy ? dx : -dy) / 2, e2;

    for (;;)
    {
        plot(Point(ax, ay), color);

        if (ax == bx && ay == by)
            break;

        e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            ax += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            ay += sy;
        }
    }
}

void Painter::draw_rectangle(Bound bound, Color color)
{
    draw_line(bound.topleft(), bound.topright(), color);
    draw_line(bound.topleft(), bound.bottomleft(), color);
    draw_line(bound.bottomright(), bound.bottomleft(), color);
    draw_line(bound.bottomright(), bound.topright(), color);
}

void Painter::draw_triangle(Point p0, Point p1, Point p2, Color color)
{
    draw_line(p0, p1, color);
    draw_line(p1, p2, color);
    draw_line(p2, p0, color);
}

// void Painter::draw_polygon(Point *points, int n, Color color)
// {
//     for (int i = 0; i < n; i++)
//     {
//         draw_line(points[i], points[i + 1], color);
//     }
// }
//
// void Painter::draw_circle(Point center, double radius, Color color)
// {
// }
//
// void Painter::draw_ellipse(Bound bound, Color color)
// {
// }
//
// void Painter::draw_circle_sector(Point center, double radius, CircleSector sector, Color color)
// {
// }
//
// void Painter::draw_rectangle_with_radius(Bound bound, double radius, Color color)
// {
// }
//
void Painter::fill_rectangle(Bound bound, Color color)
{
    for (int x = 0; x < bound.width(); x++)
    {
        for (int y = 0; y < bound.height(); y++)
        {
            plot(bound.location() + Point(x, y), color);
        }
    }
}
//
// void Painter::fill_triangle(Point p0, Point p1, Point p2, Color color)
// {
// }
//
// void Painter::fill_polygon(Point *points, int n, Color color)
// {
// }
//
// void Painter::fill_circle(Point center, double radius, Color color)
// {
// }
//
// void Painter::fill_ellipse(Bound bound, Color color)
// {
// }
//
// void Painter::fill_circle_sector(Point center, double radius, CircleSector sector, Color color)
// {
// }
//
// void Painter::fill_rectangle_with_radius(Bound bound, double radius, Color color)
// {
// }
//

void Painter::blit(Surface &source, Bound destination_bound, Color color, SurfaceFiltering filtering = SurfaceFiltering::NEAREST)
{
    blit(source, source.bound(), destination_bound, color, filtering);
}

void Painter::blit(Surface &source, Bound source_bound, Bound destination_bound, Color color, SurfaceFiltering filtering = SurfaceFiltering::NEAREST)
{
    for (int x = 0; x < destination_bound.width(); x++)
    {
        for (int y = 0; y < destination_bound.height(); y++)
        {
            float xx = x / (float)destination_bound.width;
            float yy = y / (float)destination_bound.height;

            Color pix = source.sample(xx, yy, source_bound, filtering);
            plot(Point(x, y), pix);
        }
    }
}

} // namespace libgraphic
