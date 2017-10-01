#ifndef MSR_THREAD_HANDLER_H
#define MSR_THREAD_HANDLER_H


#include <vector>
#include <pthread.h>
#include <containers/simple_vector.h>
#include "kernel/threads/thread_handler.h"

class msr_matrix;



class msr_thread_handler : public thread_handler
{
protected:
  msr_matrix &m_matrix;
  simple_vector &m_shared_buf;
public:
  msr_thread_handler (const int t, const int p, pthread_barrier_t *barrier,
                      simple_vector &shared_buf,
                      msr_matrix &matrix, const int first_t = 0);
  virtual ~msr_thread_handler ();

  int n () const;

  double aa (const int i) const;
  void aa   (const int i, const double val);
  int ja    (const int i) const;
  void ja   (const int i, const double val);

  simple_vector &shared_ref () const;
  msr_matrix &matrix () const;

  void mult_vector (const msr_matrix &shared_matrix, const simple_vector &in,
                      simple_vector &out /*must be resized to n*/);

  void mult_vector_shared_out (const msr_matrix &shared_matrix, const simple_vector &in,
                                 simple_vector &shared_out /*must be resized to n*/);
};

#endif // MSR_THREAD_HANDLER_H
