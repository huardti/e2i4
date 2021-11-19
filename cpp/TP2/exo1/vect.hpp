#pragma once

#include <iostream>

template <typename T> class Vect_3d
{
 public:
#if 0
  // 1.a
  Vect_3d()
  {
    m_x = 0;
    m_y = 0;
    m_z = 0;
  }

  Vect_3d(T x, T y, T z)
  {
    m_x = x;
    m_y = y;
    m_z = z;
  }
#endif
#if 0
  // 1.b et 1.c
  Vect_3d(T x = 0, T y = 0, T z = 0)
  {
    m_x = x;
    m_y = y;
    m_z = z;
  }
#endif
#if 1
  // 1.c
  Vect_3d(T x = 0, T y = 0, T z = 0) :
    m_x(x), m_y(y), m_z(z)
  {
  }
#endif
  // Copy Constructor
  Vect_3d(const Vect_3d<T>& obj)
    : m_x(obj.m_x), m_y(obj.m_y), m_z(obj.m_z)
  {
  }

  //Destructor
  ~Vect_3d() {}


  void print() const
  {
      std::cout << "vect = " << m_x << " " << m_y << " " << m_z << std::endl;
  }

 private:
  T m_x, m_y, m_z;
};