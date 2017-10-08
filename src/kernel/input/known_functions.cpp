#include "known_functions.h"

double f0 (const int t, const int x)
{
  (void)t;
  (void)x;
  return sin (x) + cos (x);
}

double f1 (const int t, const int x)
{
  (void)t;
  return sin (x) * cos (x) + 1 + 0.5 * exp (-x) * sin (x);
}

double p_wave_deriv (const double z)
{
  (void)z;
  return 1;
}
