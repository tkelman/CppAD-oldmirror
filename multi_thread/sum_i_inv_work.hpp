/* $Id$ */
# ifndef CPPAD_SUM_I_INV_WORK_INCLUDED
# define CPPAD_SUM_I_INV_WORK_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>

extern void sum_i_inv_worker(void);
extern bool sum_i_inv_split(size_t num_sum, size_t num_threads);
extern bool sum_i_inv_combine(double& sum);

# endif
