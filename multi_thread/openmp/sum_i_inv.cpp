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
	num
	bool
	mega
	inv
	CppAD
	parallelize
$$
$index OpenMP, speed$$
$index speed, OpenMP$$


$section OpenMP Sum of 1/i Example$$

$head Syntax$$
$icode%ok_out% = sum_i_inv(%rate_out%, %num_threads%, %mega_sum%)%$$

$head Summation$$
Runs an example and test of 
OpenMP multi-threaded computation of the sum
$latex \[
	1 + 1/2 + 1/3 + ... + 1/n
\] $$

$head ok_out$$
This return value has prototype
$codei%
	bool %ok_out%
%$$
If it is true,
$code sum_i_inv$$ passed the correctness test.
Otherwise it is false.

$head rate_out$$
This argument has prototype
$codei%
	size_t& %rate_out%
%$$
The input value of the argument does not matter.
Upon return it is the number of times per second that
$code sum_i_inv$$ can compute the 
$cref/summation/openmp_sum_i_inv.cpp/Summation/$$.

$head num_threads$$
This argument has prototype
$codei%
	size_t %num_threads%
%$$
It specifies the number of OpenMP threads that are available for this test.
If it is zero, the test is run without the OpenMP environment; 
i.e. as a normal routine.

$head mega_sum$$
This argument has prototype
$codei%
	size_t& %mega_sum%
%$$
and is greater than zero.
The value $latex n$$ in the 
$cref/summation/openmp_sum_i_inv.cpp/Summation/$$.
is equal to $latex 10^6$$ times $icode mega_sum$$. 

$head Method$$
Note that this routine starts all its summations with the
smallest terms to reduce the effects of round off error.

$head Source$$
$code
$verbatim%multi_thread/openmp/sum_i_inv.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <omp.h>
# include <cassert>
# include <cstring>
# include <limits>
# include <vector>

// Note there is no mention of parallel mode in the documentation for
// speed_test (so it is safe to use without special consideration).
# include <cppad/speed_test.hpp>

// special utilities for the sum_i_inv problem
# include "../sum_i_inv_work.hpp"

namespace { // empty namespace

	// True if num_threads is greater that zero in previous call to sum_i_inv
	bool use_openmp_;

	// Same as num_threads in previous call to sum_i_inv,
	// except that if that value is zero, this value is one.
	size_t num_threads_;

	double sum_using_multiple_threads(size_t num_sum)
	{	// sum = 1/num_sum + 1/(num_sum-1) + ... + 1
		bool ok = true;

		// split the work for num_threads_ threads
		ok &= sum_i_inv_split(num_sum, num_threads_);

		// now do the work for each thread
		size_t thread_num;
# pragma omp parallel for 
		for(thread_num = 0; thread_num < int(num_threads_); thread_num++)
			sum_i_inv_worker();
// end omp parallel for

		// now combine the result for all the threads
		double combined_sum;
		ok &= sum_i_inv_combine(combined_sum);

		if( ! ok )
		{	std::cerr << "sum_using_multiple_threads: error" << std::endl;
			exit(1);
		}
		return combined_sum;
	}

	void test_once(double &sum, size_t mega_sum)
	{	assert( mega_sum >= 1 );
		size_t n_sum = mega_sum * 1000000;
		sum = sum_using_multiple_threads(n_sum); 
		return;
	}

	void test_repeat(size_t size, size_t repeat)
	{	size_t i;
		double sum;
		for(i = 0; i < repeat; i++)
			test_once(sum, size);
		return;
	}
} // end empty namespace

bool sum_i_inv(size_t& rate_out, size_t num_threads, size_t mega_sum)
{	bool ok = true;
	using std::vector;

	// Set local namespace environment variables
	use_openmp_   = (num_threads > 0);
	if( num_threads == 0 )
		num_threads_  = 1;
	else	num_threads_  = num_threads;

	// expect number of threads to already be set up
	if( use_openmp_ )
		ok &= num_threads == CppAD::thread_alloc::num_threads();

	// minimum time for test (repeat until this much time)
	double time_min = 1.;

	// size of the one test case
	vector<size_t> size_vec(1);
	size_vec[0] = mega_sum;

	// run the test case
	vector<size_t> rate_vec = CppAD::speed_test(
		test_repeat, size_vec, time_min
	);

	// return the rate (times per second) at which test_once runs
	rate_out = rate_vec[0];

	// Call test_once for a correctness check
	double sum;
	test_once(sum, mega_sum);
	double eps   = 1e3 * std::numeric_limits<double>::epsilon();
	size_t i     = mega_sum * 1000000;
	double check = 0.;
	while(i)
		check += 1. / double(i--); 
	ok &= std::fabs(sum - check) <= eps;

	return ok;
}

// END PROGRAM
