#include "polynome.hpp"

#include <cmath>
#include <stdexcept>

Poly2deg::Poly2deg(int a, int b, int c) :
    m_a(a), m_b(b), m_c(c)
{

}

Poly2deg::~Poly2deg()
{

}
Poly2deg::solutions_t Poly2deg::solution() const
{
    int d = delta();
    solutions_t racines;

    if(d == 0) // racines double
    {
        racines[0].Re( (-m_b) / (2 * m_a) );
        racines[1] = racines[0];
    }
    else if(d > 0)
    {
        racines[0].Re( ((-m_b) - std::sqrt(d)) / (2 * m_a) );
        racines[1].Re( ((-m_b) + std::sqrt(d)) / (2 * m_a) );

    }
    else if(d < 0)
    {
        racines[0].Re( (-m_b) / (2 * m_a) );
        racines[1].Re( (-m_b) / (2 * m_a) );

        racines[0].Im( (-std::sqrt(-d)) / (2 * m_a) );
        racines[1].Im( (std::sqrt(-d)) / (2 * m_a) );

    }
    else
    {
        throw std::logic_error("solution solver internal error");
    }

    return racines;
}
