#ifndef LASPACK_VECTOR_H
#define LASPACK_VECTOR_H

#include "3rd_party/laspack/vector.h"

class laspack_vector
{
  Vector *m_raw = nullptr;
public:
  laspack_vector ();
  ~laspack_vector ();

  laspack_vector (const int size);

  Vector *raw ();
  const Vector *raw () const;

  laspack_vector (const std::vector<double> v);

  double operator[] (const int i) const;
};

#endif // LASPACK_VECTOR_H
