#ifndef GRAPH_PAINTER_H
#define GRAPH_PAINTER_H

#include <QPainter>
#include "abstract_plot_model.h"

class abstract_plot_model;

class graph_painter : public QPainter
{
private:
  double m_x_scale = 1;
  double m_y_scale = 1;
  double m_smooth = 100;
  double m_graphs_shift = 15;
  double m_ox_shift = 50;
  double m_oy_shift = 70;
  double m_y_min = -1;
  double m_y_max = 1;
  double m_max_abs_val = 0;
  double m_x_min;
  double m_x_max;
  int m_pivot_count;
  abstract_plot_model *m_plot_model = nullptr;
public:
  graph_painter ();
  graph_painter (QPaintDevice *);
  void draw_axis ();
  void draw_graph (const int graph_num);
  void redraw_all ();
  void set_model (abstract_plot_model *model);
  virtual ~graph_painter ();
private:
  QPointF get_first_graph_point (const int graph_num, const bool discrete) const;
  QPointF get_next_graph_point (const int graph_num, const bool discrete, int &inout, bool &end) const;
  void calculate_pivot_count ();
  void calculate_plot_params ();
  void calculate_graph_vert_bounds (const int graph_num, double &y_max, double &loc_max);
  QPen set_pen (const int graph_num) const;
  QPointF to_scale (QPointF point);
  void draw_line (QPointF point_a, QPointF point_b);
private slots:
  void on_model_changed ();
};

#endif // GRAPH_PAINTER_H
