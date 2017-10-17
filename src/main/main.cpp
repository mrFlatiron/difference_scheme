#include "kernel/difference_scheme_solver.h"
#include "kernel/solver_checker.h"
#include "kernel/input/known_functions.h"

int main (const int argc, const char *argv[])
{
  (void)argc;
  (void)argv;

  solver_checker checker (10, 10, 3, 3, 3);

  difference_scheme_solver solver;

  checker.start_testing (solver, 1, 1, mu_const);

  return 0;
}
