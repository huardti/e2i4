#include "point.hpp"

#include <cmath>

Point::Point(double x, double y)
    : m_x(x), m_y(y)
{
}

Point::~Point()
{
}

void Point::translate(double x, double y)
{
    m_x += x;
    m_y += y;
}

double Point::rho() const
{
    return std::sqrt(m_x*m_x + m_y*m_y);
}

/* theta is in radian */
double Point::theta() const
{
    return std::atan2(m_y, m_x);
}

void Point::rotation(double angle){
    double r = rho();
    double t = theta();

    t += angle;

    m_x = r*std::cos(t);
    m_y = r*std::sin(t);
}