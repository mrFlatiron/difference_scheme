#include "dif_scheme_plot_model.h"
#include "kernel/difference_scheme_solver.h"
#include <QColor>

dif_scheme_plot_model::dif_scheme_plot_model (difference_scheme_solver *solver, net_func func)
{
  m_solver = solver;
  m_func = func;
}

dif_scheme_plot_model::~dif_scheme_plot_model()
{

}

int dif_scheme_plot_model::graphs_count () const
{
  return 1;
}

QPointF dif_scheme_plot_model::point_by_x (const int graph_num, const double x) const
{
  (void)graph_num;
  (void)x;
  return QPointF ();
}

QPointF dif_scheme_plot_model::point_by_num (const int graph_num, const int point_num) const
{
  (void)graph_num;
  int m = point_num;

  return QPointF (m * m_solver->var_incr (variable::x), m_solver->val (m_func, m_cut, m));
}

QVariant dif_scheme_plot_model::paint_config (const int graph_num, const graph_role role) const
{
  (void)graph_num;
  switch (role)
  {
    case graph_role::discrete:
      return QVariant (true);
    case graph_role::points_count:
      return QVariant (m_solver->M ());
    case graph_role::shown:
      return m_solver && m_solver->state () == solver_state::solved;
    case graph_role::color:
      return QColor (Qt::red);
    case graph_role::width:
      return 2;
    }
  return QVariant ();
}

void dif_scheme_plot_model::bounds (const int graph_num, double &left, double &right) const
{
  (void)graph_num;
  left = 0;
  right = m_solver->X ();
}

void dif_scheme_plot_model::set_cut (int cut)
{
  m_cut = cut;
  model_changed ();
}


