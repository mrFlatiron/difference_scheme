#include "start_functions.h"


double rho0 (const double x)
{
  return x + 2;
}

double u0 (const double x)
{
  return 2 * x;
}

double g0 (const double x)
{
  return log (rho0 (x));
}
