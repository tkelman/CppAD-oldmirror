/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin openmp_sum_i_inv.cpp$$
$spell
	inv
$$
$index OpenMP, sum_i_inv$$
$index sum_i_inv, OpenMP$$


$section OpenMP Implementation of Multi-Threaded Summation of 1/i$$

See $cref sum_i_inv$$ for this routines specifications.
See $cref sum_i_inv_work.cpp$$ for the specifications of
$code sum_i_inv_setup$$, $code sum_i_inv_worker$$, 
and $code sum_i_inv_combine$$.

$head Source$$
$code
$verbatim%multi_thread/openmp/sum_i_inv.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <omp.h>

// special utilities for the sum_i_inv problem
# include "../sum_i_inv_work.hpp"

bool sum_i_inv(double& sum, size_t num_sum, size_t num_threads)
{	// sum = 1/num_sum + 1/(num_sum-1) + ... + 1
	bool ok = true;

	// setup the work for num_threads threads
	ok &= sum_i_inv_setup(num_sum, num_threads);

	// now do the work for each thread
	int thread_num;
	if( num_threads > 0 )
	{	int number_threads = int(num_threads);
# pragma omp parallel for 
		for(thread_num = 0; thread_num < number_threads; thread_num++)
			sum_i_inv_worker();
// end omp parallel for
	}
	else	sum_i_inv_worker();

	// now combine the result for all the threads
	ok &= sum_i_inv_combine(sum);

	return ok;
}
// END PROGRAM
