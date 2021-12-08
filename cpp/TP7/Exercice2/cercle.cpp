#include "cercle.hpp"

#include <iostream>

Cercle::Cercle(double x, double y, double r)
    : m_rayon(r), m_coord(x,y)
{
    std::cout << "- Cercle::Cercle -";

}

Cercle::~Cercle()
{
    std::cout << "- Cercle::~Cercle -";
}

std::ostream& operator<<(std::ostream& stream, const Cercle& s)
{
    return stream << "Cercle " << s.m_coord <<  ", r = " << s.m_rayon;
}
