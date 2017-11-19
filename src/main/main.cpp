#include "gui/main_window.h"

#include "kernel/difference_scheme_solver.h"
#include "kernel/solver_checker.h"
#include "kernel/input/known_functions.h"

#include <QApplication>

int main (int argc, char *argv[])
{

//  QApplication app (argc, argv);

//  main_window w;

//  w.show ();

//  return app.exec ();

  setlocale (LC_ALL, "Russian");

  difference_scheme_solver solver;

  solver_checker checker (3, 3, 20, 20, 4, true);

  FILE *file = fopen ("tex.out", "w");
  if (!file)
    {
      DEBUG_PAUSE ("Something wrong");
      return 0;
    }

  checker.start_testing (solver, 10, 1, mu_const, file);

  fclose (file);
  return 0;
}
