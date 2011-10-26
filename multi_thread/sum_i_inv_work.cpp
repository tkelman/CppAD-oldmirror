/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>
# include "sum_i_inv_work.hpp"

# define MAX_NUMBER_THREADS 48

namespace {
	// number of threads for previous call to sum_i_inv_split
	size_t num_threads_ = 0;
	// structure with information for one thread
	typedef struct {
		// index to start summation at (worker input)
		size_t start;
		// index to end summation at (worker input)
		size_t stop;
		// summation for this thread (worker output)
		double sum;
		// false if an error occurs, true otherwise (worker output)
		bool   ok;
	} work_one_t;
	// vector with information for all threads
	work_one_t work_all_[MAX_NUMBER_THREADS];
}
// -----------------------------------------------------------------------
// do the work for one thread
void sum_i_inv_worker(void)
{	// sum =  1/(stop-1) + 1/(stop-2) + ... + 1/start
	size_t thread_num = CppAD::thread_alloc::thread_num();
	bool   ok         = thread_num < num_threads_;
	size_t start      = work_all_[thread_num].start;
	size_t stop       = work_all_[thread_num].stop;
	double sum        = 0.;

	ok &= stop > start;
	size_t i = stop;
	while( i > start )
	{	i--;
		sum += 1. / double(i);	
	}

	work_all_[thread_num].sum = sum;
	work_all_[thread_num].ok  = ok;
}
// -----------------------------------------------------------------------
// split the work up for multiple threads
bool sum_i_inv_split(size_t num_sum, size_t num_threads)
{	// sum = 1/num_sum + 1/(num_sum-1) + ... + 1
	num_threads_ = num_threads;

	bool ok = num_sum >= num_threads;
	work_all_[0].start = 1;
	size_t thread_num;
	for(thread_num = 1; thread_num < num_threads; thread_num++)
	{	size_t index        = (num_sum * thread_num) / num_threads;
		work_all_[thread_num-1].stop = index; 
		work_all_[thread_num].start  = index;
	}
	work_all_[num_threads-1].stop = num_sum + 1;
	return ok;
}
// -----------------------------------------------------------------------
// get the result of the work 
bool sum_i_inv_combine(double& sum)
{	// sum = 1/num_sum + 1/(num_sum-1) + ... + 1
	bool ok = true;
	sum     = 0.;
	size_t thread_num;
	for(thread_num = 0; thread_num < num_threads_; thread_num++)
	{	sum += work_all_[thread_num].sum;
		ok  &= work_all_[thread_num].ok;
	}
	return ok;
}
