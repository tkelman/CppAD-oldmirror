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
$begin openmp_multi_newton.cpp$$
$spell
$$

$index multi_newton, OpenMP$$
$index OpenMP, multi_newton$$
$index newton, OpenMP$$

$section OpenMP Implementation of Multi-Threaded Newton's Method$$

See $cref multi_newton$$ for this routines specifications.
See $cref multi_newton_work.cpp$$ for the specifications of
$code multi_newton_setup$$, $code multi_newton_worker$$, 
and $code multi_newton_combine$$.


$code
$verbatim%multi_thread/openmp/multi_newton.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
---------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
# include <omp.h>

// special utilities for the multi_newton problem
# include "../multi_newton_work.hpp"

bool multi_newton(
	CppAD::vector<double> &xout                , 
	void fun(double x, double& f, double& df)  , 
	size_t num_sub                             , 
	double xlow                                , 
	double xup                                 , 
	double epsilon                             , 
	size_t max_itr                             ,
	size_t num_threads                         )
{	
	bool ok = true;
	using CppAD::AD;
	using CppAD::vector;
	using CppAD::abs;

	// setup the work for num_threads threads
	ok &= multi_newton_setup(
		fun, num_sub, xlow, xup, epsilon, max_itr, num_threads
	);

	// now do the work for each thread
	int thread_num;
	if( num_threads > 0 )
	{	int number_threads = int(num_threads);
# pragma omp parallel for 
		for(thread_num = 0; thread_num < number_threads; thread_num++)
			multi_newton_worker();
// end omp parallel for
	}
	else	multi_newton_worker();

	// now combine the result for all the threads
	ok &= multi_newton_combine(xout);

	return ok;
}
// END PROGRAM

