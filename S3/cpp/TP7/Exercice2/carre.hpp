#pragma once

#include "rectangle.hpp"

#include <iostream>

class Carre : public Rectangle
{
public:
    Carre(double x, double y, double c);
    ~Carre();

    std::ostream& print(std::ostream& stream) const;


protected:
};