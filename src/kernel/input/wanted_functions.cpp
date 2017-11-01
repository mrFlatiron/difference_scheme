#include "wanted_functions.h"
#include "precompiled/precompiled_header.h"



double g (double t, double x)
{
  (void)t;
  (void)x;
  return sin (x) + cos (t);
}

double v (double t, double x)
{
  (void)t;
  (void)x;
  return sin (x);
}

double deriv_g_t (double t, double x)
{
  (void)x;
  (void)t;
  return - sin (t);
}

double deriv_g_x (double t, double x)
{
  (void)t;
  (void)x;
  return cos (x);
}

double deriv_v_t (double t, double x)
{
  (void)t;
  (void)x;
  return 0;
}

double deriv_v_x (double t, double x)
{
  (void)t;
  (void)x;
  return cos (x);
}

double deriv_v_xx (double t, double x)
{
  (void)t;
  (void)x;
  return -sin (x);
}
