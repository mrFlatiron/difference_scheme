#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QDialog>

#include"precompiled/precompiled_header.h"


class plot_widget;
class graph_painter;
class dif_scheme_plot_model;
class difference_scheme_solver;
class QSlider;

class main_window : public QDialog
{
  Q_OBJECT
private:
  std::unique_ptr<graph_painter> m_painter;
  plot_widget *m_plot_widget;
  std::unique_ptr<dif_scheme_plot_model> m_plot_model;
  std::unique_ptr<difference_scheme_solver> m_solver;
  QSlider *m_slider;
public:
  main_window (QWidget *parent = nullptr);
  ~main_window ();

private:
  void create_widgets ();
  void set_layout ();
  void make_connections ();
private slots:
  void on_slider_moved (int val);
};

#endif // MAIN_WINDOW_H
