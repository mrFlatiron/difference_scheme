#ifndef LASPACK_MATRIX_H
#define LASPACK_MATRIX_H

#include "3rd_party/laspack/qmatrix.h"

class matrix_el;

class laspack_matrix
{
private:
  QMatrix *m_raw = nullptr;
public:
  laspack_matrix ();

  laspack_matrix (const std::vector<matrix_el> &els, const int matrix_size);
  ~laspack_matrix ();

  QMatrix *raw ();
  const QMatrix *raw () const;

  int n () const;
  void construct_from (const std::vector<matrix_el> &els, const int matrix_size);

  void dump (FILE *out = stdout) const;
};

#endif // LASPACK_MATRIX_H
