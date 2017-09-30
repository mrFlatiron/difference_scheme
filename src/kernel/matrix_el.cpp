#include "matrix_el.h"

matrix_el::matrix_el ()
{

}

matrix_el::~matrix_el ()
{

}

matrix_el::matrix_el (const int i, const int j, const double val)
{
  m_i = i;
  m_j = j;
  m_val = val;
}

int matrix_el::i () const
{
  return m_i;
}

int matrix_el::j () const
{
  return m_j;
}

double matrix_el::val () const
{
  return m_val;
}

bool matrix_el::operator < (const matrix_el &rhs) const
{
  if (i () < rhs.i ())
    return true;

  if (i () > rhs.i ())
    return false;

  if (j () < rhs.j ())
    return true;

  if (j () > rhs.j ())
    return false;

  return false;
}
