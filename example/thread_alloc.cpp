/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin thread_alloc.cpp$$
$spell
	pthread
	openmp
$$

$section Fast Multi-Threading Memory Allocator: Example and Test$$

$index openmp, memory allocation$$
$index pthread, memory allocation$$
$index boost, multi-thread memory allocation$$
$index multi-thread, memory allocation$$
$index example, memory allocation$$
$index test, memory allocation$$

$code
$verbatim%example/thread_alloc.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/thread_alloc.hpp>
# include <cppad/memory_leak.hpp>
# include <vector>


namespace { // Begin empty namespace

bool raw_allocate(void)
{	bool ok = true;
	using CppAD::thread_alloc;
	size_t thread;

	// check that no memory is initilaly inuse 
	ok &= thread_alloc::free_all();

	// amount of static memory used by thread zero
	size_t static_inuse = 0;

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
		ok &= thread_alloc::inuse(thread) == n_inner*cap_bytes + static_inuse;
		ok &= thread_alloc::available(thread) == 0;
		// return the memrory to thread_alloc
		for(j = 0; j < n_inner; j++)
			thread_alloc::return_memory(v_ptr[j]);
		// check that now n_inner * cap_bytes are now available
		// and none are in use
		ok &= thread_alloc::inuse(thread) == static_inuse;
		ok &= thread_alloc::available(thread) == n_inner * cap_bytes;
	}
	thread_alloc::free_available(thread);
	
	// check that the tests have not held onto memory
	ok &= ! CppAD::memory_leak();

	return ok;
}

class my_char {
public:
	char ch_ ;
	my_char(void) : ch_(' ')
	{ }
	my_char(const my_char& my_ch) : ch_(my_ch.ch_)
	{ }
};

bool type_allocate(void)
{	bool ok = true;
	using CppAD::thread_alloc;
	size_t i; 

	// check initial memory values
	size_t thread = thread_alloc::thread_num();
	ok &= thread == 0;
	ok &= thread_alloc::free_all();
	size_t static_inuse = 0;

	// initial allocation of an array
	size_t  size_min  = 3;
	size_t  size_one;
	my_char *array_one  = 
		thread_alloc::create_array<my_char>(size_min, size_one);

	// check the values and change them to null 'x'
	for(i = 0; i < size_one; i++)
	{	ok &= array_one[i].ch_ == ' ';
		array_one[i].ch_ = 'x';
	}

	// now create a longer array
	size_t size_two;
	my_char *array_two = 
		thread_alloc::create_array<my_char>(2 * size_min, size_two);

	// check the values in array one
	for(i = 0; i < size_one; i++)
		ok &= array_one[i].ch_ == 'x';

	// check the values in array two
	for(i = 0; i < size_two; i++)
		ok &= array_two[i].ch_ == ' ';

	// check the amount of inuse and available memory
	// (an extra size_t value is used for each memory block).
	size_t check = static_inuse + sizeof(my_char)*(size_one + size_two);
	ok   &= thread_alloc::inuse(thread) - check < sizeof(my_char);
	ok   &= thread_alloc::available(thread) == 0;

	// delete the arrays 
	thread_alloc::delete_array(array_one);
	thread_alloc::delete_array(array_two);
	ok   &= thread_alloc::inuse(thread) == static_inuse;
	check = sizeof(my_char)*(size_one + size_two);
	ok   &= thread_alloc::available(thread) - check < sizeof(my_char);

	// free the memory for use by this thread
	thread_alloc::free_available(thread);
	
	// check that the tests have not held onto memory
	ok &= ! CppAD::memory_leak();

	return ok;
}

} // End empty namespace


bool thread_alloc(void)
{	bool ok  = true;
	using CppAD::thread_alloc;

	// check that there is only on thread
	ok  &= thread_alloc::num_threads() == 1;
	// so thread number must be zero
	ok  &= thread_alloc::thread_num() == 0;
	// and we are in sequential execution mode
	ok  &= thread_alloc::in_parallel() == false;

	// Instruct thread_alloc to hold onto memory.  This makes memory 
	// allocation faster (especially when there are multiple threads).
	thread_alloc::hold_memory(true);

	// run raw allocation tests
	ok &= raw_allocate();

	// run typed allocation tests
	ok &= type_allocate();
	
	// return allocator to its default mode
	thread_alloc::hold_memory(false);
	return ok;
}


// END C++
