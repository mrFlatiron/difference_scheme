#ifndef SOLVER_CHECKER_H
#define SOLVER_CHECKER_H

#include "precompiled/precompiled_header.h"

class difference_scheme_solver;
enum class grid_func;

class solver_checker
{
private:
  int m_x_multiplier = 10;
  int m_t_multiplier = 10;
  int m_M_start = 3;
  int m_N_start = 3;
  double m_X = 0;
  double m_T = 0;
  int m_table_size = 2;
  bool m_latex_format = false;
public:
  solver_checker ();
  ~solver_checker ();

  solver_checker (int x_multiplier, int t_multiplier,
                  int M_start, int N_start, int table_size = 2, bool latex_format = false);

  void print_testing_config (FILE *fout = stdout) const;
  void print_table (const std::string &table_name, const std::vector<double> &norms, FILE *fout = stdout) const;

  void print_table (const std::vector<std::string> &table_entries, FILE *fout = stdout) const;

  double grid_dif_norm_C (const difference_scheme_solver &solver, grid_func func) const;
  double grid_dif_norm_L2 (const difference_scheme_solver &solver, grid_func func) const;
  double grid_dif_norm_L2h (const difference_scheme_solver &solver, grid_func func) const;
  double grid_dif_norm_W21 (const difference_scheme_solver &solver, grid_func func) const;
  double grid_dif_seminorm (const difference_scheme_solver &solver, grid_func func) const;

  void start_testing (difference_scheme_solver &solver, double X, double T, double mu, FILE *fout = stdout);

  static std::string double_to_string (double d);
  static std::string int_to_string (int i);
  static int pow_i (int x, int y);

  std::string format_divider () const;
  std::string format_newline () const;
};

#endif // SOLVER_CHECKER_H
