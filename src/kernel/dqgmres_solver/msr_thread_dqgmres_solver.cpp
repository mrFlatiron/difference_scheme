#include "msr_thread_dqgmres_solver.h"
#include "msr_dqgmres_initializer.h"
#include "kernel/msr_matrix.h"
#include "kernel/threads/thread_vector_utils.h"
#include "containers/simple_vector.h"
#include <cmath>

msr_thread_dqgmres_solver::msr_thread_dqgmres_solver (const int t,
                                                      msr_dqgmres_initializer &initializer) :
  msr_thread_handler (t, initializer.m_p, &(initializer.m_barrier),
                      initializer.m_shared_buf, initializer.m_matrix,
                      initializer.m_first_t),
  m_precond (initializer.m_precond),
  m_precond_type (initializer.m_precond_type),
  m_dim (initializer.m_dim),
  m_max_iter (initializer.m_max_iter),
  m_stop_criterion (initializer.m_stop_criterion),
  m_flag (initializer.m_flag),
  m_rhs (initializer.m_rhs),
  m_rhs_save (initializer.m_rhs_save),
  m_basis (initializer.m_basis),
  m_basis_derivs (initializer.m_basis_derivs),
  m_turns (initializer.m_turns),
  m_hessenberg (initializer.m_hessenberg),
  m_p_sized_buf (initializer.m_p_sized_buf),
  m_x (initializer.m_x_inout),
  m_v1 (&(initializer.m_v1)),
  m_v2 (initializer.m_v2)
{

}

msr_thread_dqgmres_solver::~msr_thread_dqgmres_solver ()
{

}

simple_vector &msr_thread_dqgmres_solver::p_sized_buf () const
{
  return m_p_sized_buf;
}



msr_matrix &msr_thread_dqgmres_solver::precond () const
{
  return m_precond;
}



dqgmres_solver_state msr_thread_dqgmres_solver::dqgmres_solve ()
{
  const double EPS = 1e-10;



  compute_preconditioner ();
  apply_preconditioner ();

  mult_vector_shared_out (m_precond, m_rhs, m_v2);
  thread_utils::copy_shared (*this, m_v2, m_rhs);
  thread_utils::copy_shared (*this, m_rhs, m_rhs_save);

  if (is_first ())
    {
      m_turns.clear ();
      m_basis_derivs.clear ();
      m_basis.clear ();
    }
  barrier_wait ();

  mult_vector_shared_out (m_matrix, m_x, m_v2);
  thread_utils::lin_combination_1 (*this, m_rhs, m_v2, -1);

  double rhs_norm = thread_utils::l2_norm (*this, m_rhs, m_p_sized_buf);
  double g1 = rhs_norm;


  if (g1 < EPS)
    return dqgmres_solver_state::OK;

  double g2;

  thread_utils::mult_vector_coef (*this, m_rhs, 1 / g1);

  if (is_first ())
    m_basis.insert (m_rhs);

  barrier_wait ();

  for (int iter = 1; iter <= m_max_iter; iter++)
    {
      bool exact_solution = false;
      if (compute_hessenberg_col ())
        {
          if (is_first ())
            printf ("Exact solution!\n");
          exact_solution = true;
        }
      apply_turn_matrices (iter);
      compute_turn_matrix (iter);
      apply_last_turn (iter, g1, g2);
      if (compute_basis_deriv (iter))
        {
          return dqgmres_solver_state::TOO_SLOW;
        }

      if (is_first ())
        *m_v1 = m_basis_derivs.get_newest ();
      barrier_wait ();
      thread_utils::lin_combination_1 (*this, m_x, **m_v1, g1);
      if (exact_solution)
        return dqgmres_solver_state::OK;
      double residual = fabs (g2);

      if (is_first ())
        printf ("\tit=%3d, residual = %le\n", iter, residual);

      if (residual < m_stop_criterion)
        return dqgmres_solver_state::OK;


      g1 = g2;

    }
  return dqgmres_solver_state::MAX_ITER;
}

void msr_thread_dqgmres_solver::compute_preconditioner ()
{
  int n = m_matrix.n ();
  int begin, work;
  divide_work (n, begin, work);

  if (is_first ())
    {
      m_precond.set_n (n);
      m_precond.set_arr_size (n + 1);
      m_precond.ja (n, n);
    }

  barrier_wait ();

  for (int i = begin; i < begin + work; i++)
    {
      switch (m_precond_type)
        {
        case preconditioner_type::identity:
          m_precond.aa (i, 1.);
          m_precond.ja (i, n);
          break;
        case preconditioner_type::jacobi:
          m_precond.aa (i, 1 / m_matrix.aa (i));
          m_precond.ja (i, n);
          break;
        case preconditioner_type::ilu0:
          break;
        }
    }

  barrier_wait ();
}

void msr_thread_dqgmres_solver::apply_preconditioner ()
{
  switch (m_precond_type)
    {
    case preconditioner_type::jacobi:
      {
        int begin, work;
        int n = m_matrix.n ();
        divide_work (n, begin, work);
        for (int i = begin; i < begin + work; i++)
          {
            double coef = precond ().aa (i);
            for (int ja_iter = ja (i); ja_iter < ja (i + 1); ja_iter ++)
              {
                aa (ja_iter, aa (ja_iter) * coef);
              }
            aa (i, 1);
          }
        break;
      }
    case preconditioner_type::identity:
      break;
    case preconditioner_type::ilu0:
      {
        if (!is_first ())
          break;
        for (int i = 1; i < m_matrix.n (); i++)
          {
            int begin, end;
            m_matrix.get_ja_row_bounds (i, begin, end);
            int ja_iter = begin;
            while (m_matrix.ja (ja_iter) < i && ja_iter < end)
              {
                int k = m_matrix.ja (ja_iter);
                int k_begin, k_end;
                m_matrix.get_ja_row_bounds (k, k_begin, k_end);
                m_matrix.aa (ja_iter, m_matrix.aa (ja_iter) / m_matrix.aa (k));
                double val = m_matrix.aa (ja_iter);
                int ja_iter1 = ja_iter + 1;
                int k_ja_iter = k_begin;
                for (; k_ja_iter < k_end && m_matrix.ja (k_ja_iter) <= k; k_ja_iter++)
                while (ja_iter1 < end && k_ja_iter < k_end)
                  {
                    double newval = m_matrix.aa (ja_iter1);
                    newval -= val * m_matrix.aa (k_ja_iter);
                    m_matrix.aa (ja_iter1, newval);
                    k_ja_iter++;
                    ja_iter1++;
                  }
                ja_iter++;
              }
          }
      }
      break;
    }
  barrier_wait ();
  return;
}

int msr_thread_dqgmres_solver::compute_hessenberg_col ()
{
  int h_iter = 1;
  if (is_first ())
    *m_v1 = m_basis.get_newest ();
  barrier_wait ();
  mult_vector_shared_out (matrix (), **m_v1, m_v2);

  if (is_first ())
    m_basis.to_preoldest ();

  while (thread_utils::limited_deque_get_next<simple_vector>
         (*this, m_basis, m_v1, m_flag))
    {
      double h = thread_utils::dot_product (*this, **m_v1, m_v2, m_p_sized_buf);
      if (is_first ())
        m_hessenberg[h_iter] = h;
      thread_utils::lin_combination_1 (*this, m_v2, **m_v1, -h);
      h_iter++;
    }

  double norm = thread_utils::l2_norm (*this, m_v2, m_p_sized_buf);

  if (is_first ())
    m_hessenberg[h_iter] = norm;

  if (norm < 1e-10)
    {
      if (is_first ())
        m_basis.insert (m_v2);
      barrier_wait ();
      return -1;
    }

  thread_utils::mult_vector_coef (*this, m_v2, 1 / norm);

  if (is_first ())
    m_basis.insert (m_v2);

  barrier_wait ();
  return 0;
}

void msr_thread_dqgmres_solver::apply_turn_matrices (const int cur_iter)
{
  if (!is_first ())
    {
      barrier_wait ();
      return;
    }
  int h_iter = (cur_iter <= m_dim) ? 1 : 0;
  auto turn_ptr = m_turns.get_oldest ();
  if (!turn_ptr)
    {
      barrier_wait ();
      return;
    }
  double cos = turn_ptr->at (0);
  double sin = turn_ptr->at (1);
  double x = m_hessenberg[h_iter];
  double y = m_hessenberg[h_iter + 1];

  if (h_iter == 0)
    {
      m_hessenberg[h_iter] = y * sin;
      m_hessenberg[h_iter + 1] = y * cos;
      turn_ptr = m_turns.get_next ();
      h_iter++;
    }

  for (; turn_ptr; turn_ptr = m_turns.get_next (), h_iter++)
    {
      x = m_hessenberg[h_iter];
      y = m_hessenberg[h_iter + 1];
      cos = turn_ptr->at (0);
      sin = turn_ptr->at (1);
      m_hessenberg[h_iter] = cos * x + sin * y;
      m_hessenberg[h_iter + 1] = -sin * x + cos * y;
    }

  barrier_wait ();
}

void msr_thread_dqgmres_solver::compute_turn_matrix (const int cur_iter)
{
  if (!is_first ())
    {
      barrier_wait ();
      return;
    }
  int h_iter = (cur_iter <= m_dim) ? cur_iter : m_dim;
  double h_mm = m_hessenberg[h_iter];
  double h_m1m = m_hessenberg[h_iter + 1];

  double s = sqrt (h_mm * h_mm + h_m1m * h_m1m);

  double cos = h_mm / s;
  double sin = h_m1m / s;

  simple_vector to_insert (2);
  to_insert[0] = cos;
  to_insert[1] = sin;

  m_turns.insert (to_insert);
  barrier_wait ();
}

void msr_thread_dqgmres_solver::apply_last_turn (const int cur_iter, double &g1, double &g2)
{
  if (is_first ())
    {
      double cos = m_turns.get_newest ()->at (0);
      double sin = m_turns.get_newest ()->at (1);
      int h_iter = (cur_iter <= m_dim) ? cur_iter : m_dim;
      m_hessenberg[h_iter] =  cos * m_hessenberg[h_iter] + sin * m_hessenberg[h_iter + 1];

      g2 = -sin * g1;
      g1 = cos * g1;
      if (p () >= 2)
        {
          m_p_sized_buf[0] = g1;
          m_p_sized_buf[1] = g2;
        }
      barrier_wait ();
    }
  else
    {
      barrier_wait ();
      g1 = m_p_sized_buf[0];
      g2 = m_p_sized_buf[1];
    }
  barrier_wait ();
}

int msr_thread_dqgmres_solver::compute_basis_deriv (const int cur_iter)
{
  if (is_first ())
    {
      m_basis.get_newest ();
      m_v2 = *(m_basis.get_prev ());
    }
    barrier_wait ();

  int h_iter = (cur_iter <= m_dim) ? 1 : 0;

  if (is_first ())
    m_basis_derivs.to_preoldest ();

  barrier_wait ();

  while (thread_utils::limited_deque_get_next<simple_vector >
         (*this, m_basis_derivs, m_v1, m_flag))
    {
      thread_utils::lin_combination_1 (*this, m_v2, **m_v1, -m_hessenberg[h_iter]);
      h_iter++;
    }

  if (fabs (m_hessenberg[h_iter]) < 1e-64)
    {
      barrier_wait ();
      return -1;
    }

  thread_utils::mult_vector_coef (*this, m_v2, 1 / m_hessenberg[h_iter]);

  if (is_first ())
    m_basis_derivs.insert (m_v2);

  barrier_wait ();
  return 0;
}


