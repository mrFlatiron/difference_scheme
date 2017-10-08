#include "simple_vector.h"

simple_vector::simple_vector ()
{
  m_data = nullptr;
  m_size = 0;
}

simple_vector::simple_vector (double *data, const int size, const bool need_copy)
{
  if (!need_copy)
    {
      m_data = data;
      m_size = size;
      m_is_owner = false;
    }
  else
    {
      resize (size);
      for (int i = 0; i < size; i++)
        m_data[i] = data[i];
      m_is_owner = true;
    }
}

simple_vector::simple_vector (const int size)
{
  m_is_owner =  true;

  if (size < 0)
    return;
  m_size = size;
  if (size > 0)
    m_data = new double[size];
  if (!m_data)
    {
      fprintf (stderr, "Allocation failed\n");
      DEBUG_PAUSE ("Allocation failed");
    }
  for (int i = 0; i < size; i++)
    m_data[i] = 0;
}

simple_vector::simple_vector (const simple_vector &vect)
{
  m_size = vect.size ();
  if (m_size > 0)
    m_data = new double[m_size];
  if (!m_data)
    {
      fprintf (stderr, "Allocation failed\n");
      DEBUG_PAUSE ("Allocation failed");
    }

  for (int i = 0; i < m_size; i++)
    m_data[i] = vect[i];

  m_is_owner = true;
}

simple_vector::simple_vector (simple_vector &&vect)
{
  m_size = vect.size ();
  m_data = vect.data ();
  m_is_owner = vect.m_is_owner;

  vect.m_data = nullptr;
  vect.m_size = 0;
  vect.m_is_owner = false;
}

simple_vector &simple_vector::operator= (const simple_vector &vector)
{
  if (m_size < vector.size ())
    {
      delete[] m_data;
      m_data = new double[vector.size ()];
      if (!m_data)
        {
          fprintf (stderr, "Allocation failed\n");
          DEBUG_PAUSE ("Allocation failed")
        }
    }
  m_size = vector.size ();
  for (int i = 0; i < m_size; i++)
    m_data[i] = vector[i];

  return *this;
}

simple_vector &simple_vector::operator= (simple_vector &&vect)
{
  m_size = vect.size ();
  m_data = vect.data ();
  m_is_owner = vect.m_is_owner;

  vect.m_data = nullptr;
  vect.m_size = 0;
  vect.m_is_owner = false;

  return *this;
}

simple_vector::simple_vector (const std::vector<double> &stdvec)
{
  resize (isize (stdvec));

  for (int i = 0; i < m_size; i++)
    m_data[i] = stdvec[i];

  m_is_owner = true;
}

simple_vector::~simple_vector()
{
    if (m_data && m_is_owner)
  delete[] m_data;
}

int simple_vector::size () const
{
  return m_size;
}

double &simple_vector::operator[] (const int i)
{
  return m_data[i];
}

const double &simple_vector::operator[] (const int i) const
{
  return m_data[i];
}

double simple_vector::at (const int i) const
{
  return m_data[i];
}

double *simple_vector::data ()
{
  return m_data;
}

const double *simple_vector::data () const
{
  return m_data;
}

void simple_vector::resize (const int size)
{
  if (!m_is_owner)
    {
      m_size = size;
      return;
    }

  if (size != m_size && size > 0)
    {
      if (m_data)
          delete[] m_data;
      m_data = new double[size];
      if (!m_data)
        {
          fprintf (stderr, "Allocation failed\n");
          DEBUG_PAUSE ("Allocation failed");
        }
    }
  m_size = size;
}

std::vector<double> simple_vector::to_std_vector () const
{
  std::vector<double> retval;
  for (int i = 0; i < m_size; i++)
    retval.push_back (m_data[i]);

  return retval;
}
