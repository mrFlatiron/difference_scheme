#include "start_functions.h"
#include "wanted_functions.h"
#include "precompiled/precompiled_header.h"

double rho0 (const double x)
{
  return exp (g0 (x));
}

double u0 (const double x)
{
  return v (0, x);
}

double g0 (const double x)
{
  return g (0, x);
}
