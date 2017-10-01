#ifndef CYCLE_BUF_H
#define CYCLE_BUF_H

#include <vector>
#include <cstdio>
#include "kernel/threads/thread_handler.h"

template <class T>
class limited_deque
{
private:
  int m_capacity;
  int m_size;
  int m_pos;
  bool m_preoldest;
  int m_oldest;
  int m_newest;
  std::vector<T> m_vals;
public:
  limited_deque (const int capacity);
  ~limited_deque ();
  void to_preoldest ();
  T *get_oldest ();
  T *get_newest ();
  T *get_prev ();
  T *get_next ();
  void insert (const T &new_val);
  void clear ();
};



template <class T>
limited_deque<T>::~limited_deque ()
{

}

template<class T>
void limited_deque<T>::to_preoldest ()
{
  m_preoldest = true;
  m_pos = m_oldest;
}

template <class T>
T *limited_deque<T>::get_oldest ()
{
  m_preoldest = false;
  if (m_size == 0)
    return nullptr;

  m_pos = m_oldest;
  return m_vals.data () + m_pos;
}

template <class T>
T *limited_deque<T>::get_newest ()
{
  m_preoldest = false;
  if (m_size == 0)
    return nullptr;

  m_pos = m_newest;
  return m_vals.data () + m_pos;
}

template <class T>
T *limited_deque<T>::get_prev ()
{
  m_preoldest = false;
  if  (m_pos == m_oldest || m_size == 0)
    return nullptr;

  if (m_pos > 0)
    m_pos--;
  else
    m_pos = m_size - 1;

  return m_vals.data () + m_pos;
}

template <class T>
T *limited_deque<T>::get_next ()
{
  if ((m_pos == m_newest && !m_preoldest) || m_size == 0)
    return nullptr;

  if (m_preoldest)
    {
      m_pos = m_oldest;
      m_preoldest = false;
      return m_vals.data () + m_pos;
    }

  if (m_pos == m_size - 1)
    m_pos = 0;
  else
    m_pos++;

  return m_vals.data () + m_pos;
}

template <class T>
void limited_deque<T>::insert (const T &new_val)
{
  if (m_size < m_capacity)
    {
      if (m_size != 0)
        m_newest++;

      m_vals[m_newest] = new_val;
      m_size++;
      return;
    }
  m_vals[m_oldest] = new_val;
  m_newest = m_oldest;
  m_oldest = (m_oldest + 1) % m_size;
}

template<class T>
void limited_deque<T>::clear ()
{
  m_size = 0;
  m_pos = 0;
  m_preoldest = false;
  m_oldest = 0;
  m_newest = 0;
}

template <class T>
limited_deque<T>::limited_deque (const int capacity) :
  m_size (0),
  m_pos (0),
  m_preoldest (false),
  m_oldest (0),
  m_newest (0)
{
  if (capacity <= 0)
    {
      fprintf (stderr, "WARNING: cycle_buf::cycle_buf: size <= 0. Assuming size = 1\n");
      m_capacity = 1;
    }
  else
    m_capacity = capacity;

  m_vals.resize (m_capacity);
}

namespace thread_utils
{
  template <class T>
  bool limited_deque_get_next (thread_handler &handler, limited_deque<T> &deque,
                               T **shared_out,
                               bool &shared_flag)
  {
    T *ptr;
    if (handler.is_first ())
      {
        ptr = deque.get_next ();
        if (!ptr)
          shared_flag = false;
        else
          {
            shared_flag = true;
            *shared_out = ptr;
          }
        handler.barrier_wait ();
      }
    else
      handler.barrier_wait ();
    return shared_flag;
  }
}
#endif // CYCLE_BUF_H
