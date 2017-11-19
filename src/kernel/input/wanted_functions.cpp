#include "wanted_functions.h"
#include "precompiled/precompiled_header.h"



double g (double t, double x)
{
  (void)t;
  (void)x;
  return t + log (cos (M_PI * x / 10) + 1.5);
}

double v (double t, double x)
{
  (void)t;
  (void)x;
  return cos (2 * M_PI * t) * sin (M_PI * (x / 10) * (x / 10));
}

double deriv_g_t (double t, double x)
{
  (void)x;
  (void)t;
  return 1;
}

double deriv_g_x (double t, double x)
{
  (void)t;
  (void)x;
  return (-M_PI / 10 * sin (M_PI * x / 10)) / (cos (M_PI * x / 10) + 1.5);
}

double deriv_v_t (double t, double x)
{
  (void)t;
  (void)x;
  return -sin (M_PI * (x / 10) * (x / 10)) * 2 * M_PI * sin (2 * M_PI * t);
}

double deriv_v_x (double t, double x)
{
  (void)t;
  (void)x;
  return M_PI / 50 * cos (2 * M_PI * t) * x * cos (M_PI * x * x / 100);
}

double deriv_v_xx (double t, double x)
{
  (void)t;
  (void)x;
  return M_PI / 50 * cos (2 * M_PI * t) * (cos (M_PI * x * x / 100) + x * 2 * x * M_PI / 100 * (-sin (M_PI * x * x / 100)));
}
