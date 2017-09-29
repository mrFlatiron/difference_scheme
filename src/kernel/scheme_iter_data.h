#ifndef SCHEME_ITER_DATA_H
#define SCHEME_ITER_DATA_H

#include "msr_matrix.h"

class scheme_iter_data
{
private:
  msr_matrix m_system;
  simple_vector m_rhs;
  int m_iter;
public:
  scheme_iter_data ();
  ~scheme_iter_data ();

  scheme_iter_data (const int N, const int M);
  msr_matrix &system ();
  const msr_matrix &system () const;
};

#endif // SCHEME_ITER_DATA_H
