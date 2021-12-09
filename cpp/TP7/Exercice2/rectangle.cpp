#include "rectangle.hpp"

#include <iostream>

Rectangle::Rectangle(double x, double y, double L, double l)
    :  Forme(x,y), m_L(L), m_l(l)
{
    std::cout << "- Rectangle::Rectangle -";
}

Rectangle::~Rectangle()
{
    std::cout << "- Rectangle::~Rectangle -";

}

std::ostream& Rectangle::print(std::ostream& stream) const
{
    return stream << "Rectangle " << m_coord <<  ", L = " << m_L << ", l = " << m_l;
}