#ifndef MSR_MATRIX_H
#define MSR_MATRIX_H

#include "containers/simple_vector.h"
#include "matrix_el.h"

class msr_thread_dqgmres_solver;

class msr_matrix
{
private:
  friend class msr_thread_dqgmres_solver;
  int m_n;
  int m_arr_size;
  std::vector<double> m_aa;
  std::vector<int> m_ja;
public:
  msr_matrix ();
  ~msr_matrix ();

  void dump (FILE *fout = stdout);

  int n () const;
  void set_n (const int n);
  int arr_size () const;
  void set_arr_size (const int size);
  double aa (const int i) const;
  void aa (const int i, const double val);
  int ja (const int i) const;
  void ja (const int i, const int val);
  void set_diagonal (const std::vector<double> &diag_vals);
  void mult_vector (const std::vector<double> &in, std::vector<double> &out);
  bool is_symmetrical () const;
  double ij (const int i, const int j) const;

  void mult_coef (const double coef);

  void construct_from (const int n, std::vector<double> matrix);
  void construct_from (const std::vector<matrix_el> &els);
  void construct_from (std::vector<matrix_el> &&els);


private:
  void print_row (FILE *fout, const int i, const int row_begin, const int row_end);
  void get_ja_row_bounds (const int i, int &begin, int &end) const;

};

#endif // MSR_MATRIX_H
