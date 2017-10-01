#include "msr_thread_handler.h"
#include "kernel/msr_matrix.h"
#include "containers/simple_vector.h"

msr_thread_handler::msr_thread_handler (const int t, const int p,
                                        pthread_barrier_t *barrier,
                                        simple_vector &shared_buf, msr_matrix &matrix, const int first_t) :
  thread_handler (t, p, barrier, first_t),
  m_matrix (matrix),
  m_shared_buf (shared_buf)
{

}

msr_thread_handler::~msr_thread_handler ()
{

}

int msr_thread_handler::n() const
{
  return m_matrix.n ();
}

double msr_thread_handler::aa (const int i) const
{
  return m_matrix.aa (i);
}

void msr_thread_handler::aa (const int i, const double val)
{
  m_matrix.aa (i, val);
}

int msr_thread_handler::ja (const int i) const
{
  return m_matrix.ja (i);
}

void msr_thread_handler::ja (const int i, const double val)
{
  m_matrix.ja (i, val);
}

simple_vector &msr_thread_handler::shared_ref () const
{
  return m_shared_buf;
}

msr_matrix &msr_thread_handler::matrix () const
{
  return m_matrix;
}

void msr_thread_handler::mult_vector (const msr_matrix &shared_matrix, const simple_vector &in,
                               simple_vector &out /*must be resized to n*/)
{
  int n = shared_matrix.n ();
  int i1, work;

  divide_work (n, i1, work);

  for (int i = i1; i < i1 + work; i++)
    {
      double s = 0;
      for (int ja_iter = shared_matrix.ja (i); ja_iter < shared_matrix.ja (i + 1); ja_iter++)
        s += shared_matrix.aa (ja_iter) * in[shared_matrix.ja (ja_iter)];

      s += shared_matrix.aa (i) * in[i];
      m_shared_buf[i] = s;
    }


  barrier_wait ();

  for (int i = 0; i < n; i++)
    out[i] = m_shared_buf[i];

  barrier_wait ();
}

void msr_thread_handler::mult_vector_shared_out (const msr_matrix &shared_matrix, const simple_vector &in,
                               simple_vector &shared_out /*must be resized to n*/)
{
  int n = shared_matrix.n ();
  int i1, work;

  divide_work (n, i1, work);

  simple_vector loc_out (work);
  simple_vector loc_in (n);

  for (int i = 0; i < n; i++)
    loc_in[i] = in[i];

  int i = i1;
  int loc_i = 0;
  for (; loc_i < work - 8; loc_i += 8, i += 8)
    {
      loc_out[loc_i] = shared_matrix.aa (i) * loc_in[i];
      loc_out[loc_i + 1] = shared_matrix.aa (i + 1) * loc_in[i + 1];
      loc_out[loc_i + 2] = shared_matrix.aa (i + 2) * loc_in[i + 2];
      loc_out[loc_i + 3] = shared_matrix.aa (i + 3) * loc_in[i + 3];
      loc_out[loc_i + 4] = shared_matrix.aa (i + 4) * loc_in[i + 4];
      loc_out[loc_i + 5] = shared_matrix.aa (i + 5) * loc_in[i + 5];
      loc_out[loc_i + 6] = shared_matrix.aa (i + 6) * loc_in[i + 6];
      loc_out[loc_i + 7] = shared_matrix.aa (i + 7) * loc_in[i + 7];
    }
  for (; loc_i < work; loc_i++, i++)
    loc_out[loc_i] = shared_matrix.aa (i) * loc_in[i];


  for (int i = i1; i < i1 + work; i++)
    {
      double s = 0;
      for (int ja_iter = shared_matrix.ja (i); ja_iter < shared_matrix.ja (i + 1); ja_iter++)
        s += shared_matrix.aa (ja_iter) * loc_in[shared_matrix.ja (ja_iter)];

      loc_out[i - i1] += s;
    }

  for (int i = i1; i < i1 + work; i++)
    shared_out[i]  = loc_out[i - i1];



  barrier_wait ();
}
