#ifndef THREAD_VECTOR_UTILS_H
#define THREAD_VECTOR_UTILS_H

#include <vector>
#include "containers/limited_deque.h"
#include "containers/simple_vector.h"

class thread_handler;

namespace thread_utils
{
//out = in + coef * add;
void lin_combination_1 (thread_handler &handler, simple_vector &shared_buf,
                        const simple_vector &in, simple_vector &out,
                        const simple_vector &add, const double coef);

void lin_combination_1 (thread_handler &handler,
                                      simple_vector &shared_inout,
                                      const simple_vector &add,
                                      const double coef);

double l2_norm (thread_handler &handler, const simple_vector &vect,
                simple_vector &shared_buf /* p size */);

double dot_product (thread_handler &handler, const simple_vector &in1,
                    const simple_vector &in2,
                    simple_vector &shared_buf /* p size */);

void mult_vector_coef (thread_handler &handler, simple_vector &shared_inout,
                       const double coef);

void copy_shared (thread_handler &handler, const simple_vector &shared_source,
                  simple_vector &shared_out);


}

#endif // THREAD_VECTOR_UTILS_H
