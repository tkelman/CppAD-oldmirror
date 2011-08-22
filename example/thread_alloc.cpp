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
$begin thread_alloc.cpp$$
$spell
	openmp
$$

$section OpenMP Memory Allocator: Example and Test$$

$index openmp, memory allocation$$
$index multi-thread, memory allocation$$
$index example, memory allocation$$
$index test, memory allocation$$

$code
$verbatim%example/thread_alloc.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/thread_alloc.hpp>
# include <vector>

namespace {
	// Setup for one thread in sequential execution mode
	bool in_parallel_false(void)
	{	return false; }
	size_t thread_num_zero(void)
	{	return 0; }
}

bool thread_alloc(void)
{	bool ok  = true;
	using CppAD::thread_alloc;

	// check initial state of allocator
	ok  &= thread_alloc::num_threads() == 1;

	// Tell thread_alloc that there are two threads so it starts holding
	// onto memory (but actuall the there is only on thread with id zero).
	thread_alloc::parallel_setup(2, in_parallel_false, thread_num_zero);
	ok  &= thread_alloc::num_threads() == 2;
	ok  &= thread_alloc::thread_num() == 0;
	ok  &= thread_alloc::in_parallel() == false;

	// check that no memory is initilaly inuse or available
	size_t thread;
	for(thread = 0; thread < 2; thread++)
	{	ok &= thread_alloc::inuse(thread) == 0;
		ok &= thread_alloc::available(thread) == 0;
	}

	// repeatedly allocate enough memory for at least two size_t values.
	size_t min_size_t = 2;
	size_t min_bytes  = min_size_t * sizeof(size_t);
	size_t n_outter   = 10;
	size_t n_inner    = 5;
	size_t cap_bytes, i, j, k;
	for(i = 0; i < n_outter; i++)
	{	// Do not use CppAD::vector here because its use of thread_alloc
		// complicates the inuse and avaialble results.	
		std::vector<void*> v_ptr(n_inner);
		for( j = 0; j < n_inner; j++)
		{	// allocate enough memory for min_size_t size_t objects
			v_ptr[j]    = thread_alloc::get_memory(min_bytes, cap_bytes);
			size_t* ptr = reinterpret_cast<size_t*>(v_ptr[j]);
			// determine the number of size_t values we have obtained
			size_t  cap_size_t = cap_bytes / sizeof(size_t);
			ok                &= min_size_t <= cap_size_t;
			// use placement new to call the size_t copy constructor
			for(k = 0; k < cap_size_t; k++)
				new(ptr + k) size_t(i + j + k);
			// check that the constructor worked
			for(k = 0; k < cap_size_t; k++)
				ok &= ptr[k] == (i + j + k);
		}
		// check that n_inner * cap_bytes are inuse and none are available
		thread = thread_alloc::thread_num();
		ok    &= thread_alloc::inuse(thread) == n_inner * cap_bytes;
		ok    &= thread_alloc::available(thread) == 0;
		// return the memrory to thread_alloc
		for(j = 0; j < n_inner; j++)
			thread_alloc::return_memory(v_ptr[j]);
		// check that now n_inner * cap_bytes are now available
		// and none are in use
		ok &= thread_alloc::inuse(thread) == 0;
		ok &= thread_alloc::available(thread) == n_inner * cap_bytes;
	}
	thread_alloc::free_available(thread);
	
	// check that the tests have not held onto memory
	for(thread = 0; thread < 2; thread++)
	{	ok &= thread_alloc::inuse(thread) == 0;
		ok &= thread_alloc::available(thread) == 0;
	}

	// set the maximum number of threads back to one 
	// so that thread_alloc no longer holds onto memory
	thread_alloc::parallel_setup(1, in_parallel_false, thread_num_zero);

	return ok;
}


// END PROGRAM
