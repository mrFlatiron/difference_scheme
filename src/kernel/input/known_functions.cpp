#include "known_functions.h"
#include "precompiled/precompiled_header.h"
#include "wanted_functions.h"



double f0 (const double t, const double x)
{
  (void)t;
  (void)x;
//  return deriv_g_t (t, x) + 0.5 * (v (t, x) * deriv_g_x (t, x) +
//                                   deriv_gv_x (t, x) + (2 - g (t, x)) * deriv_v_x (t, x));
  return 0;
}

double f1 (const double t, const double x)
{
  (void)t;
  (void)x;
//  return deriv_v_t (t, x) + 1. / 3 * (v (t, x) * deriv_v_x (t, x) +
//                                      deriv_vv_x (t, x)) +
//      p_wave_deriv (exp (g (t, x))) * deriv_g_x (t, x) - mu_const * exp (-g (t, x)) * deriv_v_xx (t, x);
  return 0;
}

double p_wave_deriv (const double z)
{
  (void)z;
  return 1.4 * pow (z, 0.4);
}

double deriv_gv_x (double t, double x)
{
  return g (t, x) * deriv_v_x (t, x) + deriv_g_x (t, x) * v (t, x);
}

double deriv_vv_x (double t, double x)
{
  return 2 * deriv_v_x (t, x) * v (t, x);
}
