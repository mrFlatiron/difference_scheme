#ifndef DIF_SCHEME_PLOT_MODEL_H
#define DIF_SCHEME_PLOT_MODEL_H

#include "abstract_plot_model.h"

#include "kernel/difference_scheme_solver.h"


class dif_scheme_plot_model : public abstract_plot_model
{
private:
  difference_scheme_solver *m_solver;
  int m_cut = 0;
  grid_func m_func = grid_func::V;
public:
  dif_scheme_plot_model (difference_scheme_solver *solver = nullptr, grid_func func = grid_func::G);
  ~dif_scheme_plot_model ();

  virtual int graphs_count () const override;

  virtual QPointF point_by_x (const int graph_num, const double x) const override;

  virtual QPointF point_by_num (const int graph_num , const int point_num) const override;

  virtual QVariant paint_config (const int graph_num, const graph_role role) const override;

  virtual void bounds (const int graph_num, double &left, double &right) const override;

  void set_cut (int cut);

  grid_func func () const;

};

#endif // DIF_SCHEME_PLOT_MODEL_H
