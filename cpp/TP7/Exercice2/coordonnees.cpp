#include "coordonnees.hpp"

#include <cmath>
#include <iostream>

Coordonnees::Coordonnees(double x, double y)
    : m_x(x), m_y(y)
{
}

Coordonnees::~Coordonnees()
{
}
/*
void Coordonnees::translate(double x, double y)
{
    m_x += x;
    m_y += y;
}
*/

std::ostream& operator<<(std::ostream& stream, const Coordonnees& s)
{
    return stream << "(" << s.m_x << ", " << s.m_y << ")";
}
