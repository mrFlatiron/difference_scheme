#include "plot_widget.h"
#include "graph_painter.h"

plot_widget::plot_widget (graph_painter *painter, QWidget *parent)
  : QWidget (parent)
{
  m_painter = painter;
}

plot_widget::~plot_widget()
{

}

QSize plot_widget::sizeHint () const
{
  return QSize (1500, 1500);
}

void plot_widget::paintEvent (QPaintEvent *event)
{
  (void)event;
  m_painter->begin (this);
  m_painter->redraw_all ();
  m_painter->end ();
}

void plot_widget::set_painter (graph_painter *painter)
{
  m_painter = painter;
}
