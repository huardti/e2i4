#pragma once

class Coordonnees
{
public:
    Coordonnees(double x = 0, double y = 0);
    ~Coordonnees();

    void translate(double x, double y);
    void dep

    inline double abscisse() const { return m_x; };
    inline double ordonnee() const { return m_y; };



private:
    double m_x, m_y;
};