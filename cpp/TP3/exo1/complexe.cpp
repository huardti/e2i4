#include "complexe.hpp"

#include <iostream>
#include <cmath>

Complexe::Complexe(double re, double im)
    : m_re(re), m_im(im)
{
}

Complexe::~Complexe()
{
}

void Complexe::print() const
{
    std::cout << m_re << " + " << m_im << "i";
}

Complexe& Complexe::operator+=(const Complexe& a)
{
    m_re += a.m_re;
    m_im += a.m_im;

    return *this;
}
Complexe& Complexe::operator-=(const Complexe& a)
{
    m_re -= a.m_re;
    m_im -= a.m_im;

    return *this;
}
Complexe& Complexe::operator*=(const Complexe& a)
{
    auto tmp_re = m_re;
    auto tmp_im = m_im;

    m_re = tmp_re * a.m_re - tmp_im * a.m_im;
    m_im = tmp_re * a.m_im + tmp_im * a.m_re;

    return *this;
}

Complexe Complexe::operator+(const Complexe& a) const
{
    Complexe r(*this);
    return r += a;
}
Complexe Complexe::operator-(const Complexe& a) const
{
    Complexe r(*this);
    return r -= a;
}
Complexe Complexe::operator*(const Complexe& a) const
{
    Complexe r(*this);
    return r *= a;
}