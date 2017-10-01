#ifndef MSR_THREAD_DQGMRES_SOLVER_H
#define MSR_THREAD_DQGMRES_SOLVER_H

#include <vector>
#include "msr_thread_handler.h"
#include "containers/limited_deque.h"
#include "containers/simple_vector.h"

class msr_dqgmres_initializer;

class msr_matrix;

enum class preconditioner_type
{
  identity,
  jacobi,
  ilu0
};

enum class dqgmres_solver_state
{
  OK,
  TOO_SLOW,
  MAX_ITER
};

class msr_thread_dqgmres_solver : public msr_thread_handler
{
public:
  msr_matrix &m_precond; //shared precond
  preconditioner_type m_precond_type;
  int m_dim;
  int m_max_iter;
  double m_stop_criterion;
  bool &m_flag;
  simple_vector &m_rhs;
  simple_vector &m_rhs_save;
  limited_deque<simple_vector> &m_basis;
  limited_deque<simple_vector> &m_basis_derivs;
  limited_deque<simple_vector> &m_turns;
  simple_vector &m_hessenberg;
  simple_vector &m_p_sized_buf;
  simple_vector &m_x;
  simple_vector **m_v1;
  simple_vector &m_v2;

public:
  msr_thread_dqgmres_solver (const int t, msr_dqgmres_initializer &initializer);
  ~msr_thread_dqgmres_solver ();

  simple_vector &p_sized_buf () const;

  msr_matrix &precond () const;

  dqgmres_solver_state dqgmres_solve ();
  void compute_preconditioner ();
  void apply_preconditioner ();
  int compute_hessenberg_col ();
  void apply_turn_matrices (const int cur_iter);
  void compute_turn_matrix (const int cur_iter);
  void apply_last_turn (const int cur_iter, double &g1, double &g2);
  int compute_basis_deriv (const int cur_iter);
private:
  void synchronize ();
};

#endif // MSR_THREAD_DQGMRES_SOLVER_H
