#include "difference_scheme_solver.h"

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

difference_scheme_solver::solver_state difference_scheme_solver::state () const
{
  return m_state;
}

void difference_scheme_solver::make_first_system ()
{

}

void difference_scheme_solver::make_second_system ()
{

}

void difference_scheme_solver::merge_systems ()
{

}

void difference_scheme_solver::solve_system ()
{

}

void difference_scheme_solver::init ()
{
  if (m_M <= 0 ||
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

  m_state = solver_state::ready;
}
