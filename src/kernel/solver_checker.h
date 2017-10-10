#ifndef SOLVER_CHECKER_H
#define SOLVER_CHECKER_H

class difference_scheme_solver;
enum class net_func;

class solver_checker
{
public:
  solver_checker ();
  ~solver_checker ();

  void print_table (const difference_scheme_solver &solver, FILE *fout = stdout) const;

  double net_dif_norm_C (const difference_scheme_solver &solver, net_func func) const;
  double net_dif_norm_L2 (const difference_scheme_solver &solver, net_func func) const;
};

#endif // SOLVER_CHECKER_H
