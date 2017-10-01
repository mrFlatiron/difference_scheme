#include "msr_dqgmres_initializer.h"
#include "kernel/msr_matrix.h"
#include "containers/simple_vector.h"
#include "containers/limited_deque.h"

msr_dqgmres_initializer::msr_dqgmres_initializer (const int first_t,
                                                  const int p,
                                                  msr_matrix &matrix,
                                                  preconditioner_type type,
                                                  const int dim,
                                                  const int max_iter,
                                                  const double stop_criterion,
                                                  const simple_vector &x_ini,
                                                  simple_vector &x_out,
                                                  const simple_vector &rhs) :
  m_first_t (first_t),
  m_p (p),
  m_matrix (matrix),
  m_precond (),
  m_precond_type (type),
  m_dim (dim),
  m_max_iter (max_iter),
  m_stop_criterion (stop_criterion),
  m_rhs (rhs),
  m_rhs_save (matrix.n ()),
  m_basis (dim),
  m_basis_derivs (dim),
  m_turns (dim),
  m_hessenberg (dim + 2),
  m_p_sized_buf (p),
  m_x_inout (x_out),
  m_v1 (),
  m_v2 (matrix.n ())
{
  pthread_barrier_init (&m_barrier, 0, p);
  m_shared_buf.resize (matrix.n ());
  m_x_inout = x_ini;
}

//msr_dqgmres_initializer::msr_dqgmres_initializer (const simple_vector &x_out) :
//  m_x_inout (x_out)
//{
//  pthread_barrier_init (&m_barrier, 0, p);
//  m_shared_bufs.resize (p);
//  for (int i = 0; i < p; i++)
//    {
//      m_shared_bufs[i].resize (matrix.n ());
//    }
//}

msr_dqgmres_initializer::~msr_dqgmres_initializer ()
{
  pthread_barrier_destroy (&m_barrier);
}
