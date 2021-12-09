#include "triangle.hpp"

#include <iostream>

Triangle::Triangle(double x, double y, double cote)
    : Forme(x,y), m_cote(cote)
{
    std::cout << "- Triangle::Triangle -";
}

Triangle::~Triangle()
{
    std::cout << "- Triangle::~Triangle -";

}

std::ostream& Triangle::print(std::ostream& stream) const
{
    return stream << "Triangle " << m_coord <<  ", c = " << m_cote;
}