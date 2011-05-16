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

	// check that thread number is zero when in sequential mode
	ok &= omp_alloc::get_thread_num() == 0;

	// check initial memory values
	size_t thread;
	for(thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
	{	// how much memory is inuse
		ok &= omp_alloc::inuse(thread) == 0;
		// how much memory is being heald for this thread
		ok &= omp_alloc::available(thread) == 0;
	}

	// repeatedly allocate the same amount of memory
	size_t num_bytes = sizeof(size_t);
	size_t i         = 100;
	while(i--)
	{	// allocate enough memory for a size_t object
		void *v_ptr = omp_alloc::get_memory(num_bytes);
		size_t* ptr = reinterpret_cast<size_t*>(v_ptr);
		// use placement new to call the size_t constructor
		new(ptr) size_t(i + 1);
		// check that the constructor worked
		ok &= (*ptr) == (i+1);
		// return the memrory to omp_alloc
		omp_alloc::return_memory(v_ptr);
	}

	return ok;
}

// END PROGRAM
