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
$begin omp_alloc.cpp$$
$spell
	openmp
$$

$section OpenMP Memory Allocator: Example and Test$$

$index openmp, memory allocation$$
$index multi-thread, memory allocation$$
$index example, memory allocation$$
$index test, memory allocation$$

$code
$verbatim%example/omp_alloc.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/omp_alloc.hpp>

bool omp_alloc(void)
{	bool ok = true;
	using CppAD::omp_alloc;

	// check not in parallel mode
	ok &= omp_alloc::in_parallel() == false;

	// check initial memory values
	size_t thread;
	for(thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
	{	// how much memory is inuse
		ok &= omp_alloc::inuse(thread) == 0;
		// how much memory is being heald for this thread
		ok &= omp_alloc::available(thread) == 0;
	}

	// determine the currently executing thread
	// (should be zero because not in parallel mode)
	thread = omp_alloc::get_thread_num();
	ok    &= thread == 0;

	// repeatedly allocate enough memory for at least two size_t values.
	size_t min_size_t = 2;
	size_t min_bytes  = min_size_t * sizeof(size_t);
	size_t n_repeat   = 100;
	size_t num_bytes, i, j;
	for(i = 0; i < n_repeat; i++)
	{	// allocate enough memory for a size_t object
		void *v_ptr = omp_alloc::get_memory(min_bytes, num_bytes);
		size_t* ptr = reinterpret_cast<size_t*>(v_ptr);
		// determine the number of size_t valuse we have obtained
		size_t  num_size_t = num_bytes / sizeof(size_t);
		ok                &= min_size_t <= num_size_t;
		// use placement new to call the size_t copy constructor
		for(j = 0; j < num_size_t; j++)
			new(ptr + j) size_t(i + j);
		// check that the constructor worked
		for(j = 0; j < num_size_t; j++)
			ok &= ptr[j] == (i+j);
		// check that num_bytes are inuse (none are available now)
		ok &= omp_alloc::inuse(thread) == num_bytes;
		ok &= omp_alloc::available(thread) == 0;
		// return the memrory to omp_alloc
		omp_alloc::return_memory(v_ptr);
		// check that now num_bytes are available 
		ok &= omp_alloc::available(thread) == num_bytes;
	}
	// return all the available memory to the system
	omp_alloc::free_available(thread);
	ok &= omp_alloc::inuse(thread) == 0;
	ok &= omp_alloc::available(thread) == 0;
	
	return ok;
}

// END PROGRAM
