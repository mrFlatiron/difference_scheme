#include "known_functions.h"

double f0 (const int t, const int x)
{
  (void)t;
  (void)x;
  return x*x + x - 1;
}

double f1 (const int t, const int x)
{
  (void)t;
  return 2 * x * x * x - 3 * x * x + x + 1 - exp (-x);
}

double p_wave_deriv (const double z)
{
  (void)z;
  return 1;
}
