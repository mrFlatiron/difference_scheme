#include "kernel/msr_matrix.h"

int main (const int argc, const char *argv[])
{
  (void)argc;
  (void)argv;

  msr_matrix matr;

  std::vector<matrix_el> els;

  els.emplace_back (0, 0, 1);
  els.emplace_back (0, 3, 2);
  els.emplace_back (1, 0 ,3);
  els.emplace_back (1, 1, 4);
  els.emplace_back (1, 3, 5);
  els.emplace_back (2, 2, 7);
  els.emplace_back (3, 2, 10);
  els.emplace_back (3, 3, 11);
  els.emplace_back (4, 4, 12);

  matr.construct_from (els);

  matr.dump ();

  return 0;
}
