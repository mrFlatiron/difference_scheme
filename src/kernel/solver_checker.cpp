#include "precompiled/precompiled_header.h"
#include "solver_checker.h"
#include "difference_scheme_solver.h"
#include "input/wanted_functions.h"

solver_checker::solver_checker ()
{

}

solver_checker::~solver_checker ()
{

}

solver_checker::solver_checker (int x_multiplier, int t_multiplier, int M_start, int N_start, int table_size, bool latex_format)
{
  m_x_multiplier = x_multiplier;
  m_t_multiplier = t_multiplier;
  m_M_start = M_start;
  m_N_start = N_start;
  m_table_size = table_size;
  m_latex_format = latex_format;
}

void solver_checker::print_testing_config (FILE *fout) const
{
  fprintf (fout, "Partitioning multipliers:\n"
                 "\tx : %d\n"
                 "\ty : %d\n"
                 "Volumes:\n"
                 "\tX : %lf\n"
                 "\tY : %lf\n\n", m_x_multiplier, m_t_multiplier,
           m_X, m_T);
}

void solver_checker::print_table (const std::string &table_name, const std::vector<double> &norms, FILE *fout) const
{
  std::vector<std::string> table_entries;
  int n = m_table_size + 1;
  if (!m_latex_format)
    table_entries.emplace_back ("N\\M");
  else
    table_entries.emplace_back ("N\\textbackslash M");
  for (int j = 1; j < n; j++)
    {
      table_entries.emplace_back (int_to_string (m_M_start * pow_i (m_x_multiplier, j - 1)));
    }
  for (int i = 1; i < n; i++)
    {
      table_entries.emplace_back (int_to_string (m_N_start * pow_i (m_t_multiplier, i - 1)));
      for (int j = 1; j < n; j++)
        table_entries.emplace_back (double_to_string (norms[(i - 1) * m_table_size + j - 1]));
    }
  fprintf (fout, "%s:\n", table_name.c_str ());
  print_table (table_entries, fout);
}

void solver_checker::print_table (const std::vector<std::string> &table_entries, FILE *fout) const
{
  int n = m_table_size + 1;

  std::vector<int> cols_width (n, -1);

  std::string latex_config = "|";
  for (int i = 0; i < n; i++)
    latex_config += "c|";

  if (m_latex_format)
    {
      fprintf (fout, "\\begin{center}\n"
                     "\\begin{tabular}{%s}\n"
                     "\\hline \n", latex_config.c_str ());
    }

  for (int j = 0; j < n; j++)
    {
      for (int i = 0; i < n; i++)
        {
          int size = isize (table_entries[i * n + j]);
          if (size > cols_width[j])
            cols_width[j] = size;
        }
    }

  for (auto &v : cols_width)
    v += 2;

  for (int i = 0; i < n; i++)
    {
      if (!m_latex_format)
        fprintf (fout, "%s", format_divider ().c_str ());
      for (int j = 0; j < n; j++)
        {
          int spaces_sum = cols_width[j] - isize (table_entries[i * n + j]);
          int spaces_left = spaces_sum / 2;
          int spaces_right = spaces_sum - spaces_left;



          for (int s = 0; s < spaces_left; s++)
            fprintf (fout, " ");

          fprintf (fout, "%s", table_entries[i * n + j].c_str ());

          for (int s = 0; s < spaces_right; s++)
            fprintf (fout, " ");

          if (j != n - 1 || !m_latex_format)
            fprintf (fout, "%s", format_divider ().c_str ());
        }
      fprintf (fout, "%s", format_newline ().c_str ());
    }

  if (m_latex_format)
    {
      fprintf (fout, "\\end{tabular}\n"
                     "\\end{center}\n");
    }
}

double solver_checker::grid_dif_norm_C (const difference_scheme_solver &solver, grid_func func) const
{
  int M = solver.M ();
  int N = solver.N ();
  double h = solver.var_incr (variable::x);
  double T = solver.T ();
  double max = -1;
  int n = N;
  switch (func)
    {
    case grid_func::G:
      {
        for (int m = 0; m <= M; m++)
          {
            double val = fabs (solver.g_val (n, m) - g (T, h * m));
            max = (max < val) ? val : max;
          }
        return max;
      }
      break;
    case grid_func::V:
      {
        for (int m = 1; m < M; m++)
          {
            double val = fabs (solver.v_val (n, m) - v (T, h * m));
            max = (max < val) ? val : max;
          }
        return max;
      }
      break;
    }
  DEBUG_PAUSE ("Shouldn't happen");
  return 0;
}

double solver_checker::grid_dif_norm_L2 (const difference_scheme_solver &solver, grid_func func) const
{
  int M = solver.M ();
  int N = solver.N ();
  double h = solver.var_incr (variable::x);
  double sum = 0;
  double T = solver.T ();
  int n = N;
  switch (func)
    {
    case grid_func::G:
      {
        for (int m = 0; m <= M; m++)
          {
            double val = fabs (solver.g_val (n, m) - g (T, h * m));
            sum += val * val;
          }
        return sqrt (h * sum);
      }
      break;
    case grid_func::V:
      {
        for (int m = 1; m < M; m++)
          {
            double val = fabs (solver.v_val (n, m) - v (T, h * m));
            sum += val * val;
          }
        return sqrt (h * sum);
      }
      break;
    }
  DEBUG_PAUSE ("Shouldn't happen");
  return 0;
}

double solver_checker::grid_dif_norm_L2h (const difference_scheme_solver &solver, grid_func func) const
{
  int M = solver.M ();
  int N = solver.N ();
  double h = solver.var_incr (variable::x);
  double sum = 0;
  double T = solver.T ();
  double X = solver.X ();
  int n = N;
  switch (func)
    {
    case grid_func::G:
      {
        double val = fabs (solver.g_val (n, 0) - g (T, 0));
        sum += 0.5 * val * val * h;
        val = fabs (solver.g_val (n, M) - g (T, X));
        sum += 0.5 * val * val * h;

        for (int m = 0; m <= M; m++)
          {
            double val = fabs (solver.g_val (n, m) - g (T, h * m));
            sum += h * val * val;
          }
        return sqrt (sum);
      }
      break;
    case grid_func::V:
      {
        double val = fabs (solver.v_val (n, 0) - v (T, 0));
        sum += 0.5 * val * val * h;
        val = fabs (solver.v_val (n, M) - v (T, X));
        sum += 0.5 * val * val * h;

        for (int m = 1; m < M; m++)
          {
            double val = fabs (solver.v_val (n, m) - v (T, h * m));
            sum += val * val * h;
          }
        return sqrt (sum);
      }
      break;
    }
  DEBUG_PAUSE ("Shouldn't happen");
  return 0;
}

double solver_checker::grid_dif_norm_W21 (const difference_scheme_solver &solver, grid_func func) const
{
  return sqrt (grid_dif_norm_L2h (solver, func) * grid_dif_norm_L2h (solver, func) +
               grid_dif_seminorm (solver, func) * grid_dif_seminorm (solver, func));
}

double solver_checker::grid_dif_seminorm (const difference_scheme_solver &solver, grid_func func) const
{
  int M = solver.M ();
  int N = solver.N ();
  double T = solver.T ();
//  double X = solver.X ();
  double h = solver.var_incr (variable::x);
  double sum = 0;

  auto wanted_deriv = [func, h] (double t, double x)
  {
      switch (func)
        {
        case grid_func::G:
          return (g (t, x + h) - g (t, x)) / h;
        case grid_func::V:
          return (v (t, x + h) - v (t, x)) / h;
        }
      return 0.;
    };

  for (int m = 0; m < M; m++)
    {
      double val = fabs (solver.deriv_x ({func}, {deriv_type::fw}, N, m) - wanted_deriv (T, h * m));
      sum += val * val * h;
    }

  return sqrt (sum);
}

void solver_checker::start_testing (difference_scheme_solver &solver, double X, double T, double mu, FILE *fout)
{
  m_X = X;
  m_T = T;

  std::vector<double> G_C_norms;
  std::vector<double> G_L2_norms;
  std::vector<double> G_W21_norms;
  std::vector<double> V_C_norms;
  std::vector<double> V_L2_norms;
  std::vector<double> V_W21_norms;


  solver.disable_printing ();

  for (int nexp = 0; nexp < m_table_size; nexp++)
    {
      for (int mexp = 0; mexp < m_table_size; mexp++)
        {
          solver.reset (m_M_start * pow_i (m_x_multiplier, mexp),
                        m_N_start * pow_i (m_t_multiplier, nexp),
                        m_X, m_T, mu, false);
          if (solver.state () != solver_state::ready)
            {
              DEBUG_PAUSE ("Some shit happened");
              return;
            }

          solver.solve ();

          G_C_norms.push_back (grid_dif_norm_C  (solver, grid_func::G));
          G_L2_norms.push_back (grid_dif_norm_L2 (solver, grid_func::G));
          G_W21_norms.push_back (grid_dif_norm_W21 (solver, grid_func::G));
          V_C_norms.push_back (grid_dif_norm_C  (solver, grid_func::V));
          V_L2_norms.push_back (grid_dif_norm_L2 (solver, grid_func::V));
          V_W21_norms.push_back (grid_dif_norm_W21 (solver, grid_func::V));

        }
    }
  print_testing_config ();
  if (!m_latex_format)
    {
  print_table ("g C norms", G_C_norms, fout);
  print_table ("g L2 norms", G_L2_norms, fout);
  print_table ("g W21 norms", G_W21_norms, fout);
  print_table ("v C norms", V_C_norms, fout);
  print_table ("v L2 norms", V_L2_norms, fout);
  print_table ("v W21 norms", V_W21_norms, fout);
    }
  else
    {
      print_table ("$||G_m^n - g (\\tau n, hm)||_{C_h}$", G_C_norms, fout);
      print_table ("$||G_m^n - g (\\tau n, hm)||_{L2_h}$", G_L2_norms, fout);
      print_table ("$||G_m^n - g (\\tau n, hm)||_{W_2^1}$", G_W21_norms, fout);
      print_table ("$||V_m^n - u (\\tau n, hm)||_{C_h}$", V_C_norms, fout);
      print_table ("$||V_m^n - u (\\tau n, hm)||_{L2_h}$", V_L2_norms, fout);
      print_table ("$||V_m^n - u (\\tau n, hm)||_{W_2^1}$", V_W21_norms, fout);
    }
}

std::string solver_checker::double_to_string (double d)
{
  constexpr int maxlen = 32;

  char s[maxlen];

  auto actual = snprintf (s, maxlen, "%le", d);
  if (actual > maxlen)
    {
      DEBUG_PAUSE ("Value is not correctly stored");
      return std::string ();
    }

  return std::string (s);
}

std::string solver_checker::int_to_string(int i)
{
  constexpr int maxlen = 32;

  char s[maxlen];

  auto actual = snprintf (s, maxlen, "%d", i);
  if (actual > maxlen)
    {
      DEBUG_PAUSE ("Value is not correctly stored");
      return std::string ();
    }

  return std::string (s);
}

int solver_checker::pow_i (int x, int y)
{
  if (y < 0)
    {
      DEBUG_PAUSE ("Not implemented");
      return 0;
    }

  int retval = 1;

  for (int i = 0; i < y; i++)
    retval *= x;

  return retval;
}

std::string solver_checker::format_divider () const
{
  if (m_latex_format)
    return "&";
  else
    return "|";
}

std::string solver_checker::format_newline () const
{
  if (m_latex_format)
    return "\\\\ \n \\hline \n";
  else
    return "\n";
}
