#include "kernel/msr_matrix.h"
#include "kernel/difference_scheme_solver.h"

int main (const int argc, const char *argv[])
{
  (void)argc;
  (void)argv;

  difference_scheme_solver solver (100, 100, 1, 1, 0.5);

  solver.solve ();

  return 0;
}
