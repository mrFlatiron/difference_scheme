#include "precompiled/precompiled_header.h"
#include "difference_scheme_solver.h"

#include "input/known_functions.h"
#include "input/start_functions.h"


#include "containers/simple_vector.h"

#include "3rd_party/laspack/itersolv.h"
#include "kernel/laspack_matrix.h"
#include "kernel/laspack_vector.h"

difference_scheme_solver::difference_scheme_solver () :
  m_state (solver_state::invalid)
{

}

difference_scheme_solver::~difference_scheme_solver ()
{

}

void difference_scheme_solver::reset (int M, int N, double X, double T, double mu)
{
  m_M = M;
  m_N = N;
  m_X = X;
  m_T = T;
  m_mu = mu;
  m_state = solver_state::invalid;
  init ();
}

difference_scheme_solver::difference_scheme_solver (int M, int N, double X, double T, double mu)
{
  reset (M, N, X, T, mu);
}

void difference_scheme_solver::solve ()
{
  m_state = solver_state::in_progress;
  for (int i = 0; i < m_N; i++)
    {
      make_first_system ();
      make_second_system ();
      merge_systems ();
      solve_system ();
      if (state () == solver_state::failed)
        return;
      m_last_computed_layer = m_iter_data.iter ();
      m_iter_data.inc_iter ();
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

double difference_scheme_solver::gas_mass (int n) const
{
  double sum = 0;
  for (int m = 0; m < M (); m++)
    sum += gas_mass_local (n, m);
  return sum;
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

int difference_scheme_solver::max_node (variable var) const
{
  switch (var)
    {
    case variable::t:
      return N ();
    case variable::x:
      return M ();
    }
  return -1;
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
  set_coef (net_func::G, row, 0, 1. / m_t + deriv_x ({net_func::V}, {deriv_type::fw}, n, 0) / 2);
  set_coef (net_func::V, row, 1, 1. / m_h);
  set_rhs_val (row,
               f0 (n * m_t, 0) + g_val (n , 0) / m_t +
               (m_h / 2) *
               (deriv_x ({net_func::G, net_func::V}, {deriv_type::fw, deriv_type::bw}, n, 1) -
                   0.5 *
                deriv_x ({net_func::G, net_func::V}, {deriv_type::fw, deriv_type::bw}, n, 2) +
                (2 - g_val (n, 0)) * (deriv_x ({net_func::V}, {deriv_type::fw, deriv_type::bw}, n, 1) -
                                         0.5 *
                                         deriv_x ({net_func::V}, {deriv_type::fw, deriv_type::bw}, n, 2))) +
               (g_val (n, 0) * deriv_x ({net_func::V}, {deriv_type::fw}, n, 0)) / 2);
  row++;
  m++;
  for (; m < m_M; m++, row++)
    {
      set_coef (net_func::G, row, m - 1, - v_val (n, m) / (2 * m_h));
      set_coef (net_func::G, row, m    , (1 / m_t + 0.5 * deriv_x ({net_func::V}, {deriv_type::wide}, n, m)));
      set_coef (net_func::G, row, m + 1, v_val (n, m) / (2 * m_h));

      if (m != m_M - 1)
        set_coef (net_func::V, row, m + 1, 1 / (2 * m_h));

      if (m != 1)
        set_coef (net_func::V, row, m - 1, - 1 / (2 * m_h));

      set_rhs_val (row,
                   f0 (n * m_t, m * m_h) +
                   g_val (n, m) *
                   (1 / m_t + 0.5 * deriv_x ({net_func::V}, {deriv_type::wide}, n, m)));
    }
  set_coef (net_func::G, row, m_M, 1. / m_t + deriv_x ({net_func::V}, {deriv_type::bw}, n, m_M) / 2);
  set_coef (net_func::V, row, m_M - 1, - 1. / m_h);
  set_rhs_val (row,
               f0 (n * m_t, m_X) +
               g_val (n, m_M) / m_t +
               (g_val (n, m_M) * deriv_x ({net_func::V}, {deriv_type::bw}, n, m_M)) / 2 -
               (m_h / 2) * (deriv_x ({net_func::G, net_func::V}, {deriv_type::fw, deriv_type::bw}, n, m_M - 1) -
                            0.5 * deriv_x ({net_func::G, net_func::V}, {deriv_type::fw, deriv_type::bw}, n, m_M - 2) +
                            (2 - g_val (n, m_M)) * (
                              deriv_x ({net_func::V}, {deriv_type::fw, deriv_type::bw}, n, m_M - 1) -
                              0.5 * deriv_x ({net_func::V}, {deriv_type::fw, deriv_type::bw}, n, m_M - 2)))
               );
}

void difference_scheme_solver::make_second_system ()
{
  int row = m_M + 1;
  int m = 1;
  int n = m_last_computed_layer;
  for (; m < m_M; m++, row++)
    {
      if (m != 1)
        set_coef (net_func::V, row, m - 1, - v_val (n, m) / (3 * m_h) - (m_mu * layer_norm (n)) / (m_h * m_h));

      set_coef (net_func::V, row, m, 1. / m_t + deriv_x ({net_func::V}, {deriv_type::wide}, n, m) / 3 +
                (2 * m_mu * layer_norm (n)) / (m_h * m_h));

      if (m != m_M - 1)
        set_coef ({net_func::V}, row, m + 1, v_val (n, m) / (3 * m_h) - (m_mu * layer_norm (n)) / (m_h * m_h));

      set_coef (net_func::G, row, m - 1, -(p_wave_deriv (g_val (n, m))) / (2 * m_h));
      set_coef (net_func::G, row, m + 1, (p_wave_deriv (g_val (n, m))) / (2 * m_h));
      set_rhs_val (row, f1 (n * m_t, m * m_h) -
                   deriv_x ({net_func::V}, {deriv_type::fw, deriv_type::bw}, n, m) * (
                     m_mu * layer_norm (n) - m_mu * exp (-g_val (n, m))) + v_val (n, m) / m_t);
    }
}

void difference_scheme_solver::merge_systems ()
{
  int dim = 2 * m_M;

  for (auto &p : m_iter_data.equation_coefs ())
    {
      p.val () = p.val () * m_t;
    }

  for (auto &rhs_v : m_iter_data.rhs ())
    rhs_v *= m_t;

  m_iter_data.system ().construct_from (m_iter_data.equation_coefs (), dim);


  if (m_enable_printing)
    printf ("Iter : %d\n", m_iter_data.iter ());

//  m_iter_data.system ().dump ();
}

void difference_scheme_solver::solve_system ()
{
  laspack_vector rhs (m_iter_data.rhs ());
  laspack_vector out (2 * m_M);

  BiCGSTABIter (m_iter_data.system ().raw (), out.raw (), rhs.raw (), 300, NULL, 1.2);

  set_computed (out);
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

  if (math_utils::eq (m_M, 0) ||
      math_utils::eq (m_N, 0))
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

  m_iter_data.init (m_N, m_M);

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

double difference_scheme_solver::deriv_x (const std::vector<net_func> &product, const std::vector<deriv_type> &types, const int n, const int m) const
{
  return deriv (product, types, variable::x, scheme_point (n, m));
}

double difference_scheme_solver::deriv_t (const std::vector<net_func> &product, const std::vector<deriv_type> &types, const int n, const int m) const
{
  return deriv (product, types, variable::t, scheme_point (n, m));
}

double difference_scheme_solver::deriv_any (variable var, net_func f, int n, int m) const
{
  int var_node;

  switch (var)
    {
    case variable::t:
      var_node = n;
      break;
    case variable::x:
      var_node = m;
      break;
    }

  auto sp = scheme_point (n, m);

  if (var_node == 0)
    return deriv ({f}, {deriv_type::fw}, var, sp);
  if (var_node == max_node (var))
    return deriv ({f}, {deriv_type::bw}, var, sp);

  return deriv ({f}, {deriv_type::wide}, var, sp);
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
  if (!math_utils::eq (val, 0))
    m_iter_data.equation_coefs ().emplace_back (row, col, val);
//  else
//    DEBUG_PAUSE ("Zero coef");
}

void difference_scheme_solver::set_rhs_val (const int row, const double val)
{
  m_iter_data.rhs ()[row] = val;
}

void difference_scheme_solver::set_computed (const laspack_vector &out)
{
  for (int i = 0; i <= m_M; i++)
    get_G_layer (m_last_computed_layer + 1)[i] = out[i];

  int m = 1;
  for (int i = m_M + 1; i < 2 * m_M; i++, m++)
    get_V_layer (m_last_computed_layer + 1)[m] = out[i];
}

double difference_scheme_solver::layer_norm (const int n) const
{
  double max = -1;

  const double *g = get_G_layer (n);

  for (int m = 0; m <= m_M; m++)
    {
      double e = exp (-g[m]);
      if (max < e)
        max = e;
    }
  return max;
}

double difference_scheme_solver::gas_mass_local (int n, int m) const
{
  if (m_state != solver_state::solved)
    return 0;

  return exp (g_val (n, m)) * m_h;
}

int difference_scheme_solver::nodes_count () const
{
  return (m_M + 1) * (m_N + 1);
}

int difference_scheme_solver::M () const
{
  return m_M;
}

int difference_scheme_solver::N () const
{
  return m_N;
}

double difference_scheme_solver::X () const
{
  return m_X;
}

double difference_scheme_solver::T () const
{
  return m_T;
}

double difference_scheme_solver::deriv_any_x (net_func f, int n, int m) const
{
  if (m < 0 || m > M ())
    {
      DEBUG_PAUSE ("Out of range");
      return 0;
    }

  return deriv_any (variable::x, f, n, m);

}

double difference_scheme_solver::deriv_any_t(net_func f, int n, int m) const
{
  if (n < 0 || n > N ())
    {
      DEBUG_PAUSE ("Out of range");
      return 0;
    }

  return deriv_any (variable::t, f, n, m);
}

void difference_scheme_solver::disable_printing ()
{
  m_enable_printing = false;
}
