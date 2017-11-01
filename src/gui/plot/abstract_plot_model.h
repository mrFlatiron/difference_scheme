#ifndef ABSTRACT_PLOT_MODEL_H
#define ABSTRACT_PLOT_MODEL_H
#include <QPointF>
#include <QVariant>
#include "abstract_plot_defs.h"

class abstract_plot_model : public QObject
{
  Q_OBJECT
public:
  abstract_plot_model ();
  virtual ~abstract_plot_model ();
  virtual int graphs_count () const = 0;

  //reimplement empty if all are discrete
  virtual QPointF point_by_x (const int graph_num, const double x) const = 0;

  //reimplement empty if all are continuous
  virtual QPointF point_by_num (const int graph_num , const int point_num) const = 0;

  virtual QVariant paint_config (const int graph_num, const graph_role role) const = 0;

  virtual void bounds (const int graph_num, double &left, double &right) const = 0;
signals:
  void model_changed ();
};

#endif // ABSTRACT_PLOT_MODEL_H
