#ifndef DIFFERENCE_SCHEME_SOLVER_H
#define DIFFERENCE_SCHEME_SOLVER_H


class difference_scheme_solver
{
public:
  enum class solver_state
  {
    invalid,
    ready,
    in_progress,
    solved,
    failed
  };

private:
  int m_M;
  int m_N;
  double m_X;
  double m_T;
  double m_mu;
  double m_h;
  double m_t;
  solver_state m_state;
public:
  difference_scheme_solver ();
  ~difference_scheme_solver ();

  difference_scheme_solver (const int M, const int N, const int X, const int T, const double mu);

  void solve ();

  double g_val (const int m, const int n) const;
  double u_val (const int m, const int n) const;
  solver_state state () const;
private:
  void make_first_system ();
  void make_second_system ();
  void merge_systems ();
  void solve_system ();

  void init ();
};

#endif // DIFFERENCE_SCHEME_SOLVER_H
