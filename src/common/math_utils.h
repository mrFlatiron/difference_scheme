#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <vector>
#include "precompiled/precompiled_header.h"

namespace math_utils
{
  template<typename T>
  T min (const T &l, const T &r)
  {
    if (l < r)
      return l;
    else
      return r;
  }

  constexpr double EQ_PRECISION = 1e-15;

  bool eq (const double a, const double b);

  double c_norm (const double *a, const int size);
  double l2_norm (const double *a, const int size);
  double l2_norm_dif (const double *a, const double *b, const int size);

  void mult_vector_coef (std::vector<double> &vect, const double coef);
}

#endif // MATH_UTILS_H
