#include "difference_scheme_solver.h"

#include "input/rhs_functions.h"
#include "input/start_functions.h"

difference_scheme_solver::difference_scheme_solver () :
  m_state (solver_state::invalid)
{

}

difference_scheme_solver::~difference_scheme_solver ()
{

}

difference_scheme_solver::difference_scheme_solver (const int M, const int N, const int X, const int T, const double mu) :
  m_M (M),
  m_N (N),
  m_X (X),
  m_T (T),
  m_mu (mu),
  m_state (solver_state::invalid)
{
  init ();
}

void difference_scheme_solver::solve ()
{
  m_state = solver_state::in_progress;
  for (int i = 0; i < m_N; i++)
    {
      make_first_system ();
      if (state () == solver_state::failed)
        return;
      make_second_system ();
      if (state () == solver_state::failed)
        return;
      merge_systems ();
      if (state () == solver_state::failed)
        return;
      solve_system ();
      if (state () == solver_state::failed)
        return;
    }
  m_state = solver_state::solved;
}

double difference_scheme_solver::g_val (const int n, const int m) const
{
  return get_G_layer (n)[m];
}

double difference_scheme_solver::u_val (const int n, const int m) const
{
  return get_V_layer (n)[m];
}

double difference_scheme_solver::v_val(const int n, const int m) const
{
  return u_val (n, m);
}

double difference_scheme_solver::val (const net_func func,
                                      const int n,
                                      const int m) const
{
  switch (func)
    {
    case net_func::G:
      return g_val (n, m);
    case net_func::V:
      return u_val (n, m);
    }
  DEBUG_PAUSE ("Shouldn't happen");
  return 0;
}

double difference_scheme_solver::val (const net_func func, const scheme_point p) const
{
  return val (func, p.n (), p.m ());
}

double difference_scheme_solver::var_incr (const variable var) const
{
  switch (var)
    {
    case variable::t:
      return m_t;
    case variable::x:
      return m_h;
    }
  DEBUG_PAUSE ("Shouldn't happen");
  return 0;
}

solver_state difference_scheme_solver::state () const
{
  return m_state;
}

void difference_scheme_solver::make_first_system ()
{
  int row = 0; // row in system
  int m = 0;
  int n = m_last_computed_layer;
  //something about m == 0;
  //rhs
  row++;
  m++;
  for (; m < m_M; m++, row++)
    {
      set_coef (net_func::G, row, m - 1, - g_val (n, m) / (2 * m_h));
      set_coef (net_func::G, row, m    , (1 / m_t + 0.5 * deriv ({net_func::V}, {deriv_type::wide}, variable::x, scheme_point (n, m))));
      set_coef (net_func::G, row, m + 1, v_val (n, m) / (2 * m_h));
      set_coef (net_func::V, row, m + 1, 1 / (2 * m_h));
      set_coef (net_func::V, row, m - 1, - 1 / (2 * m_h));
      set_rhs_val (row,
                   f0 (n * m_t, m * m_h) +
                   g_val (n, m) *
                   (1 / m_t + 0.5 * deriv ({net_func::V}, {deriv_type::wide}, variable::x, scheme_point (n, m))));
    }
  //something about m == m_M
  //rhs
}

void difference_scheme_solver::make_second_system ()
{
  int row = m_M + 1;
}

void difference_scheme_solver::merge_systems ()
{

}

void difference_scheme_solver::solve_system ()
{

}

void difference_scheme_solver::init ()
{
  if (m_M <= 2 ||
      m_N <= 0 ||
      m_X <= 0 ||
      m_T <= 0 ||
      m_mu <= 0)
    {
      m_state = solver_state::invalid;
      return;
    }

  if (utils::eq (m_M, 0) ||
      utils::eq (m_N, 0))
    {
      m_state = solver_state::invalid;
      return;
    }

  m_h = m_X / m_M;
  m_t = m_T / m_N;

  m_G.resize (nodes_count ());
  m_V.resize (nodes_count ());

  fill_zero_layer ();
  fill_V_borders ();

  m_last_computed_layer = 0;
  m_state = solver_state::ready;
}

double *difference_scheme_solver::get_G_layer (const int layer)
{
  return m_G.data () + (m_M + 1) * layer;
}

const double *difference_scheme_solver::get_G_layer (const int layer) const
{
  return m_G.data () + (m_M + 1) * layer;
}

double *difference_scheme_solver::get_V_layer (const int layer)
{
  return m_V.data () + (m_M + 1) * layer;
}

const double *difference_scheme_solver::get_V_layer (const int layer) const
{
  return m_V.data () + (m_M + 1) * layer;
}

void difference_scheme_solver::fill_zero_layer ()
{
  double *g = get_G_layer (0);
  double *v = get_V_layer (0);

  for (int i = 0; i <= m_M; i++)
    {
      double x = i * m_h;
      g[i] = g0 (x);
      v[i] = u0 (x);
    }
}

void difference_scheme_solver::fill_V_borders ()
{
  for (int n = 0; n <= m_N; n++)
    {
      get_V_layer (n)[0] = 0;
      get_V_layer (n)[m_M] = 0;
    }
}

double difference_scheme_solver::deriv (const std::vector<net_func> &product,
                                        const std::vector<deriv_type> &types,
                                        const variable var,
                                        scheme_point p) const
{
  if (isize (types) >= 3 ||
      isize (product) >= 3)
    {
      DEBUG_PAUSE ("Not implemented");
      return 0;
    }
  if (isize (product) == 2)
    {
      switch (isize (types))
        {
        case 2:
          return val (product[0], p) * deriv ({product[1]}, types, var, p) +
                 deriv ({product[0]}, {types[0]}, var, p) * deriv ({product[1]}, {types[1]}, var, p) +
                 deriv ({product[1]}, {types[0]}, var, p) * deriv ({product[0]}, {types[1]}, var, p) +
                 val (product[1], p) * deriv ({product[0]}, types, var, p);
          break;
        case 1:
          return val (product[0], p) * deriv ({product[1]}, types, var, p) +
                 val (product[1], p) * deriv ({product[0]}, types, var, p);
          break;
        default:
          DEBUG_PAUSE ("Not implemented");
          return 0;
        }
    }
  if (isize (product) == 1)
    {
      net_func func = product[0];
      switch (isize (types))
        {
        case 2:
          if (types[0] != deriv_type::fw ||
              types[1] != deriv_type::bw)
            {
              DEBUG_PAUSE ("Not implemented");
              return 0;
            }
          return (val (func, p.inc (var)) - 2 * val (func, p) + val (func, p.dec (var))) / (var_incr (var) * var_incr (var));
        case 1:
          switch (types[0])
            {
            case deriv_type::fw:
              return (val (func, p.inc (var)) - val (func, p)) / var_incr (var);
            case deriv_type::bw:
              return (val (func, p) - val (func, p.dec (var))) / var_incr (var);
            case deriv_type::wide:
              return (val (func, p.inc (var)) - val (func, p.dec (var))) / (2 * var_incr (var));
            }
        }
    }
  DEBUG_PAUSE ("Shouldn't happen");
  return 0;
}

void difference_scheme_solver::set_coef (const net_func f, const int row, const int m, const double val)
{
  int col;
  switch (f)
    {
    case net_func::G:
      col = m;
      break;
    case net_func::V:
      col = m_M + m; // (m_M + 1) + (m - 1)
    }

  m_iter_data.equation_coefs ().emplace_back (row, col, val);
}

void difference_scheme_solver::set_rhs_val (const int row, const double val)
{
  m_iter_data.rhs ()[row] = val;
}

int difference_scheme_solver::nodes_count () const
{
  return (m_M + 1) * (m_N + 1);
}
