#include "solver_checker.h"
#include "difference_scheme_solver.h"
#include "input/wanted_functions.h"

solver_checker::solver_checker ()
{

}

solver_checker::~solver_checker ()
{

}

void solver_checker::print_table (const difference_scheme_solver &solver, FILE *fout) const
{
  (void)solver;
  (void)fout;
}

double solver_checker::net_dif_norm_C (const difference_scheme_solver &solver, net_func func) const
{
  int M = solver.M ();
  int N = solver.N ();
  double h = solver.var_incr (variable::x);
  double t = solver.var_incr (variable::t);
  double max = -1;
  switch (func)
    {
    case net_func::G:
      {
        for (int n = 1; n <= N; n++)
          {
            for (int m = 0; m <= M; m++)
              {
                double val = fabs (solver.g_val (n, m) - g (t * n, h * m));
                max = (max < val) ? val : max;
              }
          }
        return max;
      }
      break;
    case net_func::V:
      {
        for (int n = 1; n <= N; n++)
          {
            for (int m = 1; m < M; m++)
              {
                double val = fabs (solver.v_val (n, m) - v (t * n, h * m));
                max = (max < val) ? val : max;
              }
          }
        return max;
      }
      break;
    }
  DEBUG_PAUSE ("Shouldn't happen");
  return 0;
}

double solver_checker::net_dif_norm_L2 (const difference_scheme_solver &solver, net_func func) const
{
  int M = solver.M ();
  int N = solver.N ();
  double h = solver.var_incr (variable::x);
  double t = solver.var_incr (variable::t);
  double sum = 0;
//  int count = 0;
  switch (func)
    {
    case net_func::G:
      {
        for (int n = 1; n <= N; n++)
          {
            for (int m = 0; m <= M; m++)
              {
                double val = fabs (solver.g_val (n, m) - g (t * n, h * m));
                sum += val * val;
//                count++;
              }
          }
        return sqrt (sum);
      }
      break;
    case net_func::V:
      {
        for (int n = 1; n <= N; n++)
          {
            for (int m = 1; m < M; m++)
              {
                double val = fabs (solver.v_val (n, m) - v (t * n, h * m));
                sum += val * val;
              }
          }
        return sqrt (sum);
      }
      break;
    }
  DEBUG_PAUSE ("Shouldn't happen");
  return 0;
}
