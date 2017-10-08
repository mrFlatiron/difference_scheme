#include "scheme_iter_data.h"

scheme_iter_data::scheme_iter_data ()
{

}

scheme_iter_data::~scheme_iter_data ()
{

}

scheme_iter_data::scheme_iter_data (const int N, const int M)
{
  init (N, M);
}

laspack_matrix &scheme_iter_data::system ()
{
  return m_system;
}

const laspack_matrix &scheme_iter_data::system() const
{
  return m_system;
}

std::vector<matrix_el> &scheme_iter_data::equation_coefs ()
{
  return m_equation_coefs;
}

const std::vector<matrix_el> &scheme_iter_data::equation_coefs() const
{
  return m_equation_coefs;
}

std::vector<double> &scheme_iter_data::rhs ()
{
  return m_rhs;
}

const std::vector<double> &scheme_iter_data::rhs () const
{
  return m_rhs;
}

int scheme_iter_data::iter () const
{
  return m_iter;
}

void scheme_iter_data::inc_iter ()
{
  m_iter++;
  m_equation_coefs.clear ();
}

void scheme_iter_data::init (const int N, const int M)
{
  (void)N;
  m_rhs.resize (2 * M);
}

