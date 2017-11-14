#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QDialog>

#include"precompiled/precompiled_header.h"


class plot_widget;
class graph_painter;
class dif_scheme_plot_model;
class difference_scheme_solver;
class QSlider;
class QLabel;

class main_window : public QDialog
{
  Q_OBJECT
private:
  std::unique_ptr<graph_painter> m_painter_G;
  std::unique_ptr<graph_painter> m_painter_V;
  plot_widget *m_plot_widget_G;
  plot_widget *m_plot_widget_V;
  std::unique_ptr<dif_scheme_plot_model> m_plot_model_G;
  std::unique_ptr<dif_scheme_plot_model> m_plot_model_V;
  std::unique_ptr<difference_scheme_solver> m_solver;
  QSlider *m_slider;
  QLabel *m_pre_gas_mass_lbl;
  QLabel *m_gas_mass_lbl;
  QLabel *m_pre_t_lbl;
  QLabel *m_t_cut_lbl;
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
