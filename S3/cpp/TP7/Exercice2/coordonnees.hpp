#pragma once

#include <sstream>
class Coordonnees
{
public:
    Coordonnees(double x = 0, double y = 0);
    ~Coordonnees();

    inline double x() const { return m_x; }
    inline double y() const { return m_y; }

    void depalce(double x, double y);

    friend std::ostream& operator<<(std::ostream& stream, const Coordonnees& s);

private:
    double m_x, m_y;
};