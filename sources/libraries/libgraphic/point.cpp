#include <libmath/Math.h>

#include <libgraphic/Point.h>

namespace libgraphic
{

double Point::Distance(Point from, Point to)
{
    return libmath::sqrt((from.X() - to.X()) * (from.X() - to.X()) +
                         (from.Y() - to.Y()) * (from.Y() - to.Y()));
}

} // namespace libgraphic
