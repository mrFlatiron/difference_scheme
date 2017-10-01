#ifndef MSR_DQGMRES_INITIALIZER_H
#define MSR_DQGMRES_INITIALIZER_H
#include "msr_thread_dqgmres_solver.h"
#include "kernel/msr_matrix.h"
#include <vector>

class msr_dqgmres_initializer
{
public:
  const int m_first_t;
  int m_p;
  pthread_barrier_t m_barrier;
  simple_vector m_shared_buf;
  msr_matrix m_matrix;
  msr_matrix m_precond; //shared precond
  preconditioner_type m_precond_type;
  int m_dim;
  int m_max_iter;
  double m_stop_criterion;
  bool m_flag;
  simple_vector m_rhs;
  simple_vector m_rhs_save;
  limited_deque<simple_vector> m_basis;
  limited_deque<simple_vector> m_basis_derivs;
  limited_deque<simple_vector> m_turns;
  simple_vector m_hessenberg;
  simple_vector m_p_sized_buf;
  simple_vector &m_x_inout;
  simple_vector *m_v1;
  simple_vector m_v2;
public:
  msr_dqgmres_initializer (const int first_t,
                           const int p,
                           msr_matrix &matrix,
                           preconditioner_type type,
                           const int dim,
                           const int max_iter,
                           const double stop_criterion,
                           const simple_vector &x_ini,
                           simple_vector &x_out,
                           const simple_vector &rhs
                           );
//  msr_dqgmres_initializer (const simple_vector &x_out);
  ~msr_dqgmres_initializer ();
};

#endif // MSR_DQGMRES_INITIALIZER_H
