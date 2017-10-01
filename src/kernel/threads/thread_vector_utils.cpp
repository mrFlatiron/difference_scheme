#include "thread_vector_utils.h"
#include "thread_handler.h"
#include <cmath>
#include <cstdio>
#include "containers/simple_vector.h"


void thread_utils::lin_combination_1 (thread_handler &handler,
                        simple_vector &shared,
                        const simple_vector &in,
                        simple_vector &out,
                        const simple_vector &add,
                        const double coef)
{
  const int n = in.size ();

  int begin, work;

  handler.divide_work (n, begin, work);

  for (int i = begin; i < begin + work; i++)
    {
      shared[i] = in[i] + add[i] * coef;
    }

  handler.barrier_wait ();

  for (int i = 0; i < n; i++)
    out[i] = shared[i];

  handler.barrier_wait ();
}

void thread_utils::lin_combination_1 (thread_handler &handler,
                                      simple_vector &shared_inout,
                                      const simple_vector &add,
                                      const double coef)
{
  const int n = shared_inout.size ();
  int i1, work;
  handler.divide_work (n, i1, work);

  simple_vector loc_inout (work);

  for (int i = 0; i < work; i++)
    loc_inout[i] = shared_inout[i + i1];

  simple_vector loc_add (work);

  for (int i = 0; i < work; i++)
    loc_add[i] = add[i + i1];


  int i;
  for (i = 0; i < work - 8; i += 8)
    {
      loc_inout[i    ] += loc_add[i    ] * coef;
      loc_inout[i + 1] += loc_add[i + 1] * coef;
      loc_inout[i + 2] += loc_add[i + 2] * coef;
      loc_inout[i + 3] += loc_add[i + 3] * coef;
      loc_inout[i + 4] += loc_add[i + 4] * coef;
      loc_inout[i + 5] += loc_add[i + 5] * coef;
      loc_inout[i + 6] += loc_add[i + 6] * coef;
      loc_inout[i + 7] += loc_add[i + 7] * coef;
    }
  for (; i < work; i++)
    {
      loc_inout[i] += loc_add[i] * coef;
    }

  handler.barrier_wait ();

  for (int i = i1; i < i1 + work; i++)
    shared_inout[i] = loc_inout[i - i1];


}

double thread_utils::l2_norm (thread_handler &handler, const simple_vector &vect,
                simple_vector &shared_buf /* p size */)
{
  int n = vect.size ();
  int begin, work;
  handler.divide_work (n, begin, work);

  shared_buf[handler.t_id () - handler.stride ()] = 0;

  for (int i = begin; i < begin + work; i++)
    {
      shared_buf[handler.t_id () - handler.stride ()] += vect[i] * vect[i];
    }

  handler.barrier_wait ();

  double s = 0;
  for (int i = 0; i < handler.p (); i++)
    s += shared_buf[i];

  handler.barrier_wait ();
  return sqrt (s);
}

void thread_utils::mult_vector_coef (thread_handler &handler, simple_vector &shared_inout,
                                     const double coef)
{
  int n = shared_inout.size ();
  int begin, work;
  handler.divide_work (n, begin, work);

  for (int i = begin; i < begin + work; i++)
    shared_inout[i] *= coef;

  handler.barrier_wait ();
}

double thread_utils::dot_product (thread_handler &handler, const simple_vector &in1,
                                  const simple_vector &in2,
                                  simple_vector &shared_buf)
{
  int n = in1.size ();
  int begin, work;
  handler.divide_work (n, begin, work);

  double s = 0;
  for (int i = begin; i < begin + work; i++)
    s += in1[i] * in2[i];

  shared_buf[handler.t_id () - handler.stride ()] = s;
  s = 0;

  handler.barrier_wait ();

  for (int i = 0; i < handler.p (); i++)
    s += shared_buf[i];

  handler.barrier_wait ();
  return s;
}

void thread_utils::copy_shared (thread_handler &handler, const simple_vector &shared_source,
                                simple_vector &shared_out)
{
  int n = shared_source.size ();
  int begin, work;
  handler.divide_work (n, begin, work);

  for (int i = begin; i < begin + work; i++)
    shared_out[i] = shared_source[i];

  handler.barrier_wait ();
}
