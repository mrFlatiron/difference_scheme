#include "scheme_iter_data.h"

scheme_iter_data::scheme_iter_data ()
{

}

scheme_iter_data::~scheme_iter_data ()
{

}

scheme_iter_data::scheme_iter_data (const int N, const int M)
{
  m_rhs.resize (2 * M);
}

msr_matrix &scheme_iter_data::system ()
{
  return m_system;
}

const msr_matrix &scheme_iter_data::system () const
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

void scheme_iter_data::inc_iter()
{
  m_iter++;
}

