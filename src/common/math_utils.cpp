#include "math_utils.h"

bool math_utils::eq (const double a, const double b)
{
  if (fabs (a - b) < EQ_PRECISION)
    return true;
  else
    return false;
}

double math_utils::c_norm (const double *a, const int size)
{
  double max = -1;

  for (int i = 0; i < size; i++)
    if (max < fabs (a[i]))
      max = fabs (a[i]);

  return max;
}

double math_utils::l2_norm (const double *a, const int size)
{
  double sq = 0;

  for (int i = 0; i < size; i++)
    sq += a[i] * a[i];

  return sqrt (sq);
}

void math_utils::mult_vector_coef (std::vector<double> &vect, const double coef)
{
  for (int i = 0; i < isize (vect); i++)
    vect[i] *= coef;
}

double math_utils::l2_norm_dif (const double *a, const double *b, const int size)
{
  double sq = 0;

  for (int i = 0; i < size; i++)
    {
      sq += (a[i] - b[i]) * (a[i] - b[i]);
    }

  return sqrt (sq);
}
