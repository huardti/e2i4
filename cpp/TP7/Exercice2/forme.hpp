#pragma once

#include "coordonnees.hpp"

class Forme
{
public:
    Forme(double x, double y);
    virtual ~Forme();

    virtual double perimetre() const = 0;
    virtual double surface() const = 0;

    virtual std::ostream& print(std::ostream& stream) const = 0;

    void deplace(double x, double y) { m_coord.depalce(x,y); }

    friend std::ostream& operator<<(std::ostream& stream, const Forme& s);

protected:
    Coordonnees m_coord;
};