#include "start_functions.h"
#include "wanted_functions.h"
#include "precompiled/precompiled_header.h"

double rho0 (const double x)
{
  (void)x;
//  if (x < 4.5 || x > 5.5)
//    return 1;
//  return 2;

//  return exp (g0 (x));

  return 1;
}

double u0 (const double x)
{
  (void)x;
//  return 0;
//  return v (0, x);

  if (x < 4.5 || x > 5.5)
    return 0;
  return 1;
}

double g0 (const double x)
{
  return log (rho0 (x));
//  return g (0, x);
}
