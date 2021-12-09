#include "carre.hpp"

Carre::Carre(double x, double y, double c)
    : Rectangle(x,y,c,c)
{
    std::cout << "- Carre::Carre -";
}

Carre::~Carre()
{
    std::cout << "- Carre::~Carre -";
}

std::ostream& Carre::print(std::ostream& stream) const
{
    return stream << "Carre " << m_coord << ", c = " << m_L;
}