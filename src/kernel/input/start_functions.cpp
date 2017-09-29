#include "start_functions.h"


double rho0 (const double x)
{
  return x;
}

double u0 (const double x)
{
  return 2 * x;
}

double g0 (const double x)
{
  return log (rho (x));
}
