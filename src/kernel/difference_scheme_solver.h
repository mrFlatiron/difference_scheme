#ifndef DIFFERENCE_SCHEME_SOLVER_H
#define DIFFERENCE_SCHEME_SOLVER_H
#include "precompiled/precompiled_header.h"

#include "scheme_defs.h"
#include "scheme_point.h"
#include "scheme_iter_data.h"

class laspack_vector;


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
  bool m_enable_printing = true;
  bool m_until_stab = false;
  double m_stab_precision = 1e-2;
public:
  difference_scheme_solver ();
  ~difference_scheme_solver ();

  difference_scheme_solver (int M, int N, double X, double T, double mu, bool until_stab = false);

  void reset (int M, int N, double X, double T, double mu, bool until_stab);

  void solve ();

  double g_val (const int n, const int m) const;
  double u_val (const int n, const int m) const;
  double v_val (const int n, const int m) const;

  double val (const grid_func func, const int n, const int m) const;
  double val (const grid_func func, scheme_point p) const;

  double gas_mass (int n) const;

  double var_incr (const variable var) const;
  int max_node (variable var) const;

  solver_state state () const;
  int nodes_count () const;
  int M () const;
  int N () const;
  double X () const;
  double T () const;

  double deriv_any_x (grid_func f, int n, int m) const;
  double deriv_any_t (grid_func f, int n, int m) const;

  double deriv_x (const std::vector<grid_func> &product,
                  const std::vector<deriv_type> &types,
                  const int n, const int m) const;

  void disable_printing ();
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

  double product_val (const std::vector<grid_func> &product, scheme_point p) const;

  void fill_zero_layer ();
  void fill_V_borders ();


  //for expample
  //(GV)x\overline{x} = deriv ({grid_func::G, grid_func::V}, {deriv_type::fw, deriv_type::bw}, variable::x, scheme_point (n, m));
  double deriv (const std::vector<grid_func> &product,
                const std::vector<deriv_type> &types,
                const variable var,
                scheme_point p) const;

  double deriv_v2 (const std::vector<grid_func> &product,
                   const std::vector<deriv_type> &types,
                   const variable var,
                   scheme_point p) const;

//  double deriv_x (const std::vector<grid_func> &product,
//                  const std::vector<deriv_type> &types,
//                  const int n, const int m) const;

  double deriv_t (const std::vector<grid_func> &product,
                  const std::vector<deriv_type> &types,
                  const int n, const int m) const;

  double deriv_any (variable var, grid_func f, int n, int m) const;

  void set_coef (const grid_func f, const int row, const int m, const double val);
  void set_rhs_val (const int row, const double val);
  void set_computed (const laspack_vector &out);

  double layer_norm (const int n) const; // ||e^(-G)||_C

  double gas_mass_local (int n, int m) const;

  bool check_stabilized () const;
};

#endif // DIFFERENCE_SCHEME_SOLVER_H
