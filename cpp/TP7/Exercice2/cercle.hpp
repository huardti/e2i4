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

    double perimetre() { return 2*M_PI*m_rayon; }
    double surface() {  return M_PI*m_rayon*m_rayon; }

    friend std::ostream& operator<<(std::ostream& stream, const Cercle& s);

protected:
    double m_rayon;
    Coordonnees m_coord;
};