// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin pthread_simple_ad.cpp$$
$spell
	pthread
	pthreads
	CppAD
$$

$section Simple Pthread AD: Example and Test$$

$index pthread, simple AD$$
$index AD, simple pthread$$
$index simple, AD pthread$$
$index thread, pthread simple AD$$

$head Purpose$$
This example demonstrates how CppAD can be used with multiple pthreads.

$head Discussion$$
The function $code arc_tan$$ below
is an implementation of the inverse tangent function where the
$cref/operation sequence/glossary/Operation/Sequence/$$ depends on the
argument values. 
Hence the function needs to be re-taped 
for different argument values.
The $cref/atan2/$$ function uses $cref/CondExp/$$ operations
to avoid the need to re-tape.

$head Source Code$$
$code
$verbatim%multi_thread/pthread/simple_ad.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
# include "../thread_team.hpp"
# include "../arc_tan.hpp"

# define NUMBER_THREADS            4

namespace {
	// structure with information for one thread
	typedef struct {
		// angle for this work (worker input)
		double          theta;
		// false if an error occurs, true otherwise (worker output)
		bool            ok;
	} work_one_t;
	// vector with information for all threads
	work_one_t work_all_[NUMBER_THREADS];
	// --------------------------------------------------------------------
	// function that does the work for one thread
	void worker(void)
	{	using CppAD::NearEqual;
		using CppAD::AD;
		bool ok = true;
		size_t thread_num = CppAD::thread_alloc::thread_num();

		// CppAD::vector uses the CppAD fast multi-threading allocator
		CppAD::vector< AD<double> > Theta(1), Z(1);
		Theta[0] = work_all_[thread_num].theta;
		Independent(Theta);
		AD<double> x = cos(Theta[0]);
		AD<double> y = sin(Theta[0]);
		Z[0]  = arc_tan( x, y );
		CppAD::ADFun<double> f(Theta, Z); 

		// Check function value corresponds to the identity 
		double eps = 10. * CppAD::epsilon<double>();
		ok        &= NearEqual(Z[0], Theta[0], eps, eps);

		// Check derivative value corresponds to the identity.
		CppAD::vector<double> d_theta(1), d_z(1);
		d_theta[0] = 1.;
		d_z        = f.Forward(1, d_theta);
		ok        &= NearEqual(d_z[0], 1., eps, eps);

		// pass back ok information for this thread
		work_all_[thread_num].ok = ok;
	}
}

// This test routine is only called by the master thread (thread_num = 0).
bool simple_ad(void)
{	bool ok = true;
	using CppAD::thread_alloc;

	size_t num_threads = NUMBER_THREADS;

	// Check that no memory is in use or avialable at start
	// (using thread_alloc in sequential mode)
	size_t thread_num;
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	ok &= thread_alloc::inuse(thread_num) == 0; 
		ok &= thread_alloc::available(thread_num) == 0; 
	}

	// initialize work_all_
 	double pi = 4. * atan(1.);
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	// set to value by worker for this thread
		work_all_[thread_num].ok           = false;
		// theta 
		work_all_[thread_num].theta        = thread_num * pi / num_threads;
	}

	ok &= start_team(num_threads);
	ok &= work_team(worker);
	ok &= stop_team();


	// Check that no memory currently in use, and free avialable memory.
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	ok &= thread_alloc::inuse(thread_num) == 0; 
		thread_alloc::free_available(thread_num); 
	}

	return ok;
}
// END PROGRAM
