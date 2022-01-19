#ifndef __QS_MATRIX_H
#define __QS_MATRIX_H

#include <iostream>
#include <vector>
#include <assert.h>
template <typename T> class Matrix
{
 public:
  Matrix(unsigned row, unsigned col, const T& init);
  Matrix(const Matrix<T>& rhs);
  virtual ~Matrix();

  // Access the individual elements
  T& operator()(const unsigned& row, const unsigned& col);
  const T& operator()(const unsigned& row, const unsigned& col) const;

  // Access the row and column sizes
  unsigned get_rows() const;
  unsigned get_cols() const;

  void transpose();
  std::vector<T> tab_1D();

  void print() const;

 private:
  std::vector<std::vector<T> > m_mat;
  unsigned m_rows;
  unsigned m_cols;
};


// Parameter Constructor
template<typename T>
Matrix<T>::Matrix(unsigned row, unsigned col, const T& init) :
  m_rows(row), m_cols(col)
{
  m_mat.resize(col);
  for (unsigned i=0; i<m_mat.size(); i++) {
    m_mat[i].resize(row, init);
  }
}

// Copy Constructor
template<typename T>
Matrix<T>::Matrix(const Matrix<T>& rhs)
{
  m_mat = rhs.m_mat;
  m_rows = rhs.get_rows();
  m_cols = rhs.get_cols();
}

// (Virtual) Destructor
template<typename T>
Matrix<T>::~Matrix() {}


// Access the individual elements
template<typename T>
T& Matrix<T>::operator()(const unsigned& x, const unsigned& y)
{
  assert((x < m_cols));
  assert((y < m_rows));
  return m_mat[x][y];
}

// Access the individual elements (const)
template<typename T>
const T& Matrix<T>::operator()(const unsigned& x, const unsigned& y) const
{
  assert((x < m_cols));
  assert((y < m_rows));
  return m_mat[x][y];
}

// Get the number of rows of the matrix
template<typename T>
unsigned Matrix<T>::get_rows() const
{
  return m_rows;
}

// Get the number of columns of the matrix
template<typename T>
unsigned Matrix<T>::get_cols() const
{
  return m_cols;
}

template <typename T>
void Matrix<T>::transpose()
{
  std::vector<std::vector<T> > tmp;
  tmp.resize(m_cols);
  for (unsigned i=0; i<tmp.size(); i++) {
    tmp[i].resize(m_rows, 0);
  }

  for (size_t i = 0; i < m_cols; i++) {
    for (size_t j = 0; j < m_rows; j++) {
      tmp[i][j] = m_mat[j][i];
    }
  }

  m_mat = tmp;

  int t = m_rows;
  m_rows = m_cols;
  m_cols = t;
}

template <typename T>
std::vector<T> Matrix<T>::tab_1D()
{
  std::vector<T> ret;
  ret.resize(m_rows * m_cols);
  for (size_t y = 0; y < m_rows; y++) {
    for (size_t x = 0; x < m_cols; x++) {
      ret[x + y * m_cols] = m_mat[x][y];
    }
  }
  return ret;
}


template <typename T>
void Matrix<T>::print() const
{
  std::cout << m_rows << " "<< m_cols << std::endl;

  for (size_t y = 0; y < m_rows; y++) {
    for (size_t x = 0; x < m_cols; x++) {
      std::cout << m_mat[x][y] << "  ";
    }
    std::cout << std::endl;
  }
}

template <typename T>
Matrix<T> multi(Matrix<T> m1, Matrix<T> m2)
{
  Matrix<T> result(m1.get_rows(), m2.get_cols(), 0);

  for(unsigned int i = 0; i < m1.get_rows(); i++)
  {
    for(unsigned int j = 0; j < m2.get_cols(); j++)
    {
      for(unsigned int k = 0; k < m2.get_cols(); k++)
      {
        result(i,j) += m2(i,k) * m1(k,j);
      }
    }
  }
  return result;
}

#endif