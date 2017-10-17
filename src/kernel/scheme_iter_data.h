#ifndef SCHEME_ITER_DATA_H
#define SCHEME_ITER_DATA_H
#include "precompiled/precompiled_header.h"
#include "laspack_matrix.h"
#include "matrix_el.h"

class scheme_iter_data
{
private:
  std::vector<matrix_el> m_equation_coefs;
  laspack_matrix m_system;
  std::vector<double> m_rhs;
  int m_iter = 1;
public:
  scheme_iter_data ();
  ~scheme_iter_data ();

  scheme_iter_data (const int N, const int M);

  laspack_matrix &system ();
  const laspack_matrix &system () const;

  std::vector<matrix_el> &equation_coefs ();
  const std::vector<matrix_el> &equation_coefs () const;

  std::vector<double> &rhs ();
  const std::vector<double> &rhs () const;

  int iter () const;

  void inc_iter ();

  void init (const int N, const int M);

};

#endif // SCHEME_ITER_DATA_H
