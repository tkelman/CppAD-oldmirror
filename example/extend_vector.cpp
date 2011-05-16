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
$begin extend_vector.cpp$$
$spell
	openmp
$$

$section Extend Vector: Example and Test$$

$index openmp, vector allocation$$
$index multi-thread, vector allocation$$
$index example, vector allocation$$
$index test, vector allocation$$
$index vector, allocation$$
$code
$verbatim%example/extend_vector.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/extend_vector.hpp>

namespace { // Begin empty namespace
	class my_char {
	public:
		char ch_ ;
		my_char(void) : ch_(' ')
		{ }
		my_char(const my_char& my_ch) : ch_(my_ch.ch_)
		{ }
	};
}

bool extend_vector(void)
{	bool ok = true;
	using CppAD::extend_vector;
	using CppAD::omp_alloc;
	size_t i; 

	// check initial memory values
	size_t thread = omp_alloc::get_thread_num();
	ok &= omp_alloc::inuse(thread) == 0;
	ok &= omp_alloc::available(thread) == 0;

	// initial allocation of a vector
	my_char *vec_in   = 0;
	size_t  size_in   = 0;
	size_t  size_min  = 3;
	size_t  size_out;
	my_char *vec_out  = extend_vector(size_in, vec_in, size_min, size_out);

	// check the values and change them to null 'x'
	for(i = 0; i < size_out; i++)
	{	ok &= vec_out[i].ch_ == ' ';
		vec_out[i].ch_ = 'x';
	}

	// now extend the vector to at least 
	vec_in   = vec_out;
	size_in  = size_out;
	size_min = 2 * size_in;
	vec_out  = extend_vector(size_in, vec_in, size_min, size_out);

	// check the values the output vector
	for(i = 0; i < size_in; i++)
		ok &= vec_out[i].ch_ == 'x';
	for(i = size_in; i < size_out; i++)
		ok &= vec_out[i].ch_ == ' ';

	// check the amount of inuse and available memory
	ok &= omp_alloc::inuse(thread) == sizeof(my_char) * size_out;
	ok &= omp_alloc::available(thread) == sizeof(my_char) * size_in;

	// delete the output vector
	size_in  = size_out;
	vec_in   = vec_out;
	size_min = 0;
	vec_out = extend_vector(size_in, vec_in, size_min, size_out);
	ok &= size_out == 0;
	ok &= vec_out == 0;

	// free the memory for use by any thread
	omp_alloc::free_available(thread);
	ok &= omp_alloc::inuse(thread) == 0;
	ok &= omp_alloc::available(thread) == 0;

	return ok;
}
// END PROGRAM
