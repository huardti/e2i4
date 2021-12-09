#pragma once
#include "forme.hpp"
#include "coordonnees.hpp"

#include <sstream>
#include <cmath>

class Triangle : public Forme
{
public:
    Triangle(double x, double y, double cote); // on suppose que le triangle a les trois cotes de taille cote.
    ~Triangle();

    double perimetre() const { return 3*m_cote; }
    double surface() const { return (sqrt(3)/4) * m_cote * m_cote; }

    std::ostream& print(std::ostream& stream) const;

protected:
    double m_cote;
};