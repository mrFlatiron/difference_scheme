#ifndef THREAD_HANDLER_H
#define THREAD_HANDLER_H

#include <pthread.h>

class thread_handler
{
public:
  int m_t;
  int m_p;
  int m_first_t;
  pthread_barrier_t *m_barrier;
public:
  thread_handler ();
  thread_handler (const int t_id, const int p,
               pthread_barrier_t *barrier, const int first_t_id = 0);
  virtual ~thread_handler ();
  int t_id  () const;
  int p     () const;
  bool is_first () const;
  int stride () const;
  void barrier_wait ();
  void divide_work (const int n, int &begin, int &work);
};

#endif // THREAD_HANDLER_H
