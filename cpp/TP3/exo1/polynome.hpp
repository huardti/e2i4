#pragma once
#include "complexe.hpp"

#include <array>

class Poly2deg
{
public:
    Poly2deg(int a, int b, int c);
    ~Poly2deg();

    inline int delta() const { return (m_b * m_b) - (4 * m_a *m_c); };

typedef std::array<Complexe, 2> solutions_t;

    solutions_t solution() const;

private:
    int const m_a;
    int const m_b;
    int const m_c;

};