/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/thread_alloc.hpp>
# include <cppad/vector.hpp>
# include <vector>
# include <omp.h>

namespace {
	// used to inform CppAD when we are in parallel execution mode
	bool in_parallel(void)
	{	return static_cast<bool> ( omp_in_parallel() ); }

	// used to inform CppAD of the current thread number
	size_t thread_number(void)
	{	return static_cast<size_t>( omp_get_thread_num() ); } 
}

bool openmp_alloc(void)
{	bool all_ok = true;
	using CppAD::thread_alloc;
	using CppAD::vector;

	all_ok     &= ! thread_alloc::in_parallel();
	all_ok     &= thread_alloc::thread_num() == 0;;
	all_ok     &= thread_alloc::num_threads() == 1;

	// Turn off dynamic thread adjustment
	omp_set_dynamic(0);

	// Set the number of OpenMP threads
	size_t num_threads = 4;
	omp_set_num_threads( int(num_threads) );

	// setup for openmp multi-threading allocation using thread_alloc
	thread_alloc::parallel_setup(num_threads, in_parallel, thread_number);

	int thread_num;
	size_t thread;
	bool ok[CPPAD_MAX_NUM_THREADS];
# pragma omp parallel for
	for(thread_num = 0; thread_num < int(num_threads); thread_num++)
	{	thread   = static_cast<size_t>(thread_num);
		ok[thread]  = thread_alloc::inuse(thread) == 0;
		ok[thread] &= thread_alloc::available(thread) == 0;
		// make one minimal size allocation in parallel
		vector<size_t> x(1);
		ok[thread] &= thread_alloc::inuse(thread) == CPPAD_MIN_CAPACITY;
		ok[thread] &= thread_alloc::available(thread) == 
			CPPAD_MIN_CHUNK - CPPAD_MIN_CAPACITY;
	}
// end omp parallel for
	thread_alloc::parallel_setup(1, CPPAD_NULL, CPPAD_NULL);
	for(thread = 0; thread < num_threads; thread++)
	{	all_ok &= ok[thread];
		all_ok &= thread_alloc::inuse(thread) == 0;
		all_ok &= thread_alloc::available(thread) == CPPAD_MIN_CHUNK;
		thread_alloc::free_available(thread);
		all_ok &= thread_alloc::available(thread) == 0;
	}
	return all_ok;
}
