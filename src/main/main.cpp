#include "gui/main_window.h"

#include "kernel/difference_scheme_solver.h"
#include "kernel/solver_checker.h"
#include "kernel/input/known_functions.h"

#include <QApplication>

int main (int argc, char *argv[])
{

  QApplication app (argc, argv);

  main_window w;

  w.show ();

  return app.exec ();

//  difference_scheme_solver solver;

//  solver_checker checker (2, 2, 640, 20, 1);

//  checker.start_testing (solver, M_PI, 1, mu_const);
  return 0;
}
