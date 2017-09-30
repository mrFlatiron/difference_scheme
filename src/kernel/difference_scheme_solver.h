#ifndef DIFFERENCE_SCHEME_SOLVER_H
#define DIFFERENCE_SCHEME_SOLVER_H

#include "containers/simple_vector.h"

#include "scheme_defs.h"
#include "scheme_point.h"
#include "scheme_iter_data.h"



class difference_scheme_solver
{
private:
  int m_M;
  int m_N;
  double m_X;
  double m_T;
  double m_mu;
  double m_h;
  double m_t;
  std::vector<double> m_G;
  std::vector<double> m_V;
  solver_state m_state;

  scheme_iter_data m_iter_data;
  int m_last_computed_layer;
public:
  difference_scheme_solver ();
  ~difference_scheme_solver ();

  difference_scheme_solver (const int M, const int N, const int X, const int T, const double mu);

  void solve ();

  double g_val (const int n, const int m) const;
  double u_val (const int n, const int m) const;
  double v_val (const int n, const int m) const;

  double val (const net_func func, const int n, const int m) const;
  double val (const net_func func, scheme_point p) const;

  double var_incr (const variable var) const;

  solver_state state () const;
  int nodes_count () const;
private:
  void make_first_system ();
  void make_second_system ();
  void merge_systems ();
  void solve_system ();

  void init ();

  double *get_G_layer (const int layer);
  const double *get_G_layer (const int layer) const;

  double *get_V_layer (const int layer);
  const double *get_V_layer (const int layer) const;

  void fill_zero_layer ();
  void fill_V_borders ();


  //for expample
  //(GV)x\overline{x} = deriv ({net_func::G, net_func::V}, {deriv_type::fw, deriv_type::bw}, variable::x, scheme_point (n, m));
  double deriv (const std::vector<net_func> &product,
                const std::vector<deriv_type> &types,
                const variable var,
                scheme_point p) const;

  void set_coef (const net_func f, const int row, const int m, const double val);
  void set_rhs_val (const int row, const double val);
};

#endif // DIFFERENCE_SCHEME_SOLVER_H
