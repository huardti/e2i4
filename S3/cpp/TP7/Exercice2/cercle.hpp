#pragma once
#include "forme.hpp"
#include "coordonnees.hpp"

#include <sstream>
#include <cmath>

class Cercle : public Forme
{
public:
    Cercle(double x, double y, double r);
    ~Cercle();

    double perimetre() const { return 2*M_PI*m_rayon; }
    double surface() const {  return M_PI*m_rayon*m_rayon; }

    std::ostream& print(std::ostream& stream) const;

protected:
    double m_rayon;
};