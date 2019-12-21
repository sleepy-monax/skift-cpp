#pragma once

#include <libruntime/RefPtr.h>
#include <libruntime/Vector.h>

#include <libgraphic/Bound.h>
#include <libgraphic/Color.h>
#include <libgraphic/Surface.h>

namespace libgraphic
{

enum class CircleSector
{
    TOP_LEFT,
    TOP_RIGHT,
    BOTTON_LEFT,
    BOTTON_RIGHT
};

class Painter
{
private:
    Bound _current_clip;
    libruntime::Vector<Bound> _clip_stack;
    libruntime::RefPtr<Surface> _surface;

public:
    Painter(libruntime::RefPtr<Surface> surface) : _surface(surface)
    {
    }

    ~Painter() {}

    void push_clip(Bound clip);
    void pop_clip();
    Bound clip();

    void clear(Color color);
    void plot(Point p, Color color);

    void draw_line(Point from, Point to, Color);
    void draw_rectangle(Bound bound, Color color);
    void draw_triangle(Point p0, Point p1, Point p2, Color color);
    void draw_polygon(Point *points, int n, Color color);
    void draw_circle(Point center, double radius, Color color);
    void draw_ellipse(Bound bound, Color color);
    void draw_circle_sector(Point center, double radius, CircleSector sector, Color color);
    void draw_rectangle_with_radius(Bound bound, double radius, Color color);

    void fill_rectangle(Bound bound, Color color);
    void fill_triangle(Point p0, Point p1, Point p2, Color color);
    void fill_polygon(Point *points, int n, Color color);
    void fill_circle(Point center, double radius, Color color);
    void fill_ellipse(Bound bound, Color color);
    void fill_circle_sector(Point center, double radius, CircleSector sector, Color color);
    void fill_rectangle_with_radius(Bound bound, double radius, Color color);

    void blit(Surface &source, Bound source_bound, Bound destination_bound, Color color);
};

} // namespace libgraphic
