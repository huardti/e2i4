#include "forme.hpp"

#include <iostream>

Forme::Forme(double x, double y)
    : m_coord(x,y)
{
    std::cout << "- Forme::Forme -";
}

Forme::~Forme() {
    std::cout << "- Forme::~Forme -" << std::endl;
}

std::ostream& operator<<(std::ostream& stream, const Forme& s)
{
    s.print(stream);
    return stream;
}