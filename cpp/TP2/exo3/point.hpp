#pragma once

class Point
{
public:
    Point(double x = 0, double y = 0);
    ~Point();

    void translate(double x, double y);
    inline double abscisse() const { return m_x; };
    inline double ordonnee() const { return m_y; };

    double rho() const;
    double theta() const;

    void rotation(double angle);


private:
    double m_x, m_y;
};