#pragma once

class Complexe
{
public:
    Complexe(double re = 0, double im = 0);
    Complexe(const Complexe& obj) : m_re(obj.m_re), m_im(obj.m_im) {}
    ~Complexe();

    inline double Re() const { return m_re; }
    inline double Im() const { return m_im; }

    inline void Re(double re) { m_re = re; }
    inline void Im(double im) { m_im = im; }

    void print() const;

    Complexe& operator+=(const Complexe& a);
    Complexe& operator-=(const Complexe& a);
    Complexe& operator*=(const Complexe& a);

    Complexe operator+(const Complexe& a) const;
    Complexe operator-(const Complexe& a) const;
    Complexe operator*(const Complexe& a) const;

    Complexe& operator=(const Complexe& a) = default;

private:
    double m_re, m_im;
};