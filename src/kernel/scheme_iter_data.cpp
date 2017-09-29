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

