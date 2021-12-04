#include "Coordonnees.hpp"

#include <cmath>

Coordonnees::Coordonnees(double x, double y)
    : m_x(x), m_y(y)
{
}

Coordonnees::~Coordonnees()
{
}

void Coordonnees::translate(double x, double y)
{
    m_x += x;
    m_y += y;
}