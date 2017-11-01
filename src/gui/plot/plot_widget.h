#ifndef PLOT_WIDGET_H
#define PLOT_WIDGET_H

#include <QWidget>
class graph_painter;

class plot_widget : public QWidget
{
  Q_OBJECT
private:
  graph_painter *m_painter;
public:
  plot_widget (graph_painter *painter, QWidget *parent = nullptr);
  ~plot_widget ();
  QSize sizeHint () const override;
  void set_painter (graph_painter *painter);
protected:
  virtual void paintEvent (QPaintEvent *event) override;

signals:


};

#endif // PLOT_WIDGET_H
