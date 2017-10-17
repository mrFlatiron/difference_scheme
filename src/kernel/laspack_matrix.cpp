#include "precompiled/precompiled_header.h"
#include "laspack_matrix.h"

#include "matrix_el.h"

laspack_matrix::laspack_matrix ()
{
  m_raw = new QMatrix;
}

laspack_matrix::laspack_matrix (const std::vector<matrix_el> &els, const int matrix_size)
{
  construct_from (els, matrix_size);
}

laspack_matrix::~laspack_matrix ()
{
  if (m_raw)
    Q_Destr (m_raw);
  delete m_raw;
}

QMatrix *laspack_matrix::raw ()
{
  return m_raw;
}

const QMatrix *laspack_matrix::raw () const
{
  return m_raw;
}

int laspack_matrix::n () const
{
  return m_raw->Dim;
}

void laspack_matrix::construct_from (const std::vector<matrix_el> &els, const int matrix_size)
{
  auto sorted_els = els;
  std::sort (sorted_els.begin (), sorted_els.end ());

  Q_Constr (m_raw, "Matrix", matrix_size, Boolean::False, ElOrderType::Rowws, InstanceType::Normal, Boolean::True);

  int els_iter = 0;
  int els_size = isize (els);

  for (int row = 0; row < matrix_size; row++)
    {
      int ei = els_iter;
      int row_nz_count = 0;

      while (ei < els_size && els[ei].i () == row)
        {
          ei++;
          row_nz_count++;
        }

      if (row_nz_count != 0)
        {
          Q_SetLen (m_raw, row + 1, row_nz_count);
          int pos = 0;
          for (int ei_row = els_iter; ei_row < ei; ei_row++, pos++)
            {
              Q_SetEntry (m_raw, row + 1, pos, els[ei_row].j () + 1, els[ei_row].val ());
            }
        }
      else
        Q_SetLen (m_raw, row + 1, 0);

      els_iter = ei;
    }
}

void laspack_matrix::dump (FILE *out) const
{
  fprintf (out, "laspack_matrix::dump ()\n");
  int size = m_raw->Dim;
  for (int i = 0; i < size; i++)
    {
      for (int j = 0; j < size; j++)
        {
          printf ("%.3lf ", Q_GetEl (m_raw, i + 1, j + 1));
        }
      printf ("\n");
    }
}
