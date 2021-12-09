#include "cercle.hpp"

#include <iostream>

Cercle::Cercle(double x, double y, double r)
    : Forme(x,y), m_rayon(r)
{
    std::cout << "- Cercle::Cercle -";

}

Cercle::~Cercle()
{
    std::cout << "- Cercle::~Cercle -";
}

std::ostream& Cercle::print(std::ostream& stream) const
{
    return stream << "Cercle " << m_coord <<  ", r = " << m_rayon;
}
