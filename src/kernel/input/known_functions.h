#ifndef KNOWN_FUNCTIONS_H
#define KNOWN_FUNCTIONS_H

constexpr double mu_const = 0.001;

double f0 (const double t, const double x);
double f1 (const double t, const double x);

double p_wave_deriv (const double z);

double deriv_gv_x (double t, double x);
double deriv_vv_x (double t, double x);

#endif // KNOWN_FUNCTIONS_H
