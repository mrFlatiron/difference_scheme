#include "main_window.h"


#include "plot/plot_widget.h"
#include "kernel/input/known_functions.h"
#include "kernel/difference_scheme_solver.h"
#include "plot/dif_scheme_plot_model.h"
#include "plot/graph_painter.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>

main_window::main_window (QWidget *parent)
  : QDialog (parent)
{
  create_widgets ();
  set_layout ();
  make_connections ();
}

main_window::~main_window ()
{

}

void main_window::create_widgets()
{
  int M = 800;
  int N = 800;
  double X = 10;
  double T = 100;

  m_painter_G = make_unique<graph_painter> ();
  m_painter_V = make_unique<graph_painter> ();
  m_plot_widget_G = new plot_widget (m_painter_G.get (), this);
  m_plot_widget_V = new plot_widget (m_painter_V.get (), this);
  m_solver = make_unique<difference_scheme_solver> (M, N, X, T, mu_const, false);
  m_solver->solve ();
  T = m_solver->T ();
  m_plot_model_G = make_unique<dif_scheme_plot_model> (m_solver.get (), net_func::G);
  m_plot_model_V = make_unique<dif_scheme_plot_model> (m_solver.get (), net_func::V);
  m_slider = new QSlider (Qt::Horizontal, this);
  m_slider->setMinimum (0);
  m_slider->setMaximum (N);
  m_slider->setValue (0);
  m_painter_G->set_model (m_plot_model_G.get ());
  m_painter_V->set_model (m_plot_model_V.get ());
  m_pre_gas_mass_lbl = new QLabel ("Mass : ", this);
  m_gas_mass_lbl = new QLabel (this);
  m_gas_mass_lbl->setText (QString::number (m_solver->gas_mass (0)));

  m_pre_t_lbl = new QLabel ("t : ", this);
  m_t_cut_lbl = new QLabel ("0", this);

}

void main_window::set_layout ()
{
  QVBoxLayout *vlo_0 = new QVBoxLayout;
  {
    vlo_0->addWidget (m_plot_widget_G);
    vlo_0->addWidget (m_plot_widget_V);
    vlo_0->addWidget (m_slider);
    QHBoxLayout *hlo_1 = new QHBoxLayout;
    {
      hlo_1->addWidget (m_pre_t_lbl);
      hlo_1->addWidget (m_t_cut_lbl);
      hlo_1->addStretch ();
    }
    vlo_0->addLayout (hlo_1);
    QHBoxLayout *hlo_0 = new QHBoxLayout;
    {
      hlo_0->addWidget (m_pre_gas_mass_lbl);
      hlo_0->addWidget (m_gas_mass_lbl);
      hlo_0->addStretch ();
    }
    vlo_0->addLayout (hlo_0);

  }
  setLayout (vlo_0);
}

void main_window::make_connections ()
{
  connect (m_plot_model_G.get (), SIGNAL (model_changed ()), m_plot_widget_G, SLOT (update ()));
  connect (m_plot_model_V.get (), SIGNAL (model_changed ()), m_plot_widget_V, SLOT (update ()));
  connect (m_slider, SIGNAL (valueChanged (int)), this, SLOT (on_slider_moved (int)));
}

void main_window::on_slider_moved (int val)
{
  m_plot_model_G->set_cut (val);
  m_plot_model_V->set_cut (val);
  m_gas_mass_lbl->setText (QString::number (m_solver->gas_mass (val)));
  m_t_cut_lbl->setText (QString::number (val * m_solver->var_incr (variable::t)));
}
