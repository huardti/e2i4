#pragma once

#include "forme.hpp"
#include "coordonnees.hpp"

#include <sstream>

class Rectangle : public Forme
{
public:
    Rectangle(double x, double y, double L, double l);
    ~Rectangle();

    double perimetre() const { return 2 * (m_l + m_L); }
    double surface() const { return m_l * m_L; }

    std::ostream& print(std::ostream& stream) const;

protected:
    double m_L;
    double m_l;
};