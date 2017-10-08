#include "start_functions.h"


double rho0 (const double x)
{
  return exp (x);
}

double u0 (const double x)
{
  return sin (x);
}

double g0 (const double x)
{
  return log (rho0 (x));
}
