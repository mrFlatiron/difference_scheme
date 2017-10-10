#include "kernel/msr_matrix.h"
#include "kernel/difference_scheme_solver.h"
#include "kernel/solver_checker.h"

int main (const int argc, const char *argv[])
{
  (void)argc;
  (void)argv;

  difference_scheme_solver solver (100, 10, 1, 1, 0.5);

  solver.solve ();

  solver_checker checker;

  double g_C_norm = checker.net_dif_norm_C  (solver, net_func::G);
  double g_L_norm = checker.net_dif_norm_L2 (solver, net_func::G);
  double v_C_norm = checker.net_dif_norm_C  (solver, net_func::V);
  double v_L_norm = checker.net_dif_norm_L2 (solver, net_func::V);

  printf ("G C-norm dif : %le\n", g_C_norm);
  printf ("G L-norm dif : %le\n", g_L_norm);
  printf ("V C-norm dif : %le\n", v_C_norm);
  printf ("V L-norm dif : %le\n", v_L_norm);

  return 0;
}
