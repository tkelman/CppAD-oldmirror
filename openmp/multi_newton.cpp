/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin multi_newton.cpp$$

$section Multi-Threaded Newton's Method Main Program$$

$head Subroutines$$
$children%
	openmp/multi_newton.hpp
%$$
$table
$rref multi_newton$$
$rref multi_newton.hpp$$
$tend

$head Example Source$$
$code
$verbatim%openmp/multi_newton.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cmath>
# include "multi_newton.hpp"

# ifdef _OPENMP
# include <omp.h>
# endif

using CppAD::vector;

# define LENGTH_OF_SUMMATION 10   // larger values make fun(x) take longer

// A slow version of the sine function
CppAD::AD<double> fun(const CppAD::AD<double> &x)
{	CppAD::AD<double> sum = 0.;
	size_t i;
	for(i = 0; i < LENGTH_OF_SUMMATION; i++)
		sum += sin(x);

	return sum / double(LENGTH_OF_SUMMATION);
}

void test_once(CppAD::vector<double> &xout, size_t size)
{	double pi      = 4. * atan(1.); 
	size_t n_grid  = size;
	double xlow    = 0.;
	double xup     = 7. * pi;
	double epsilon = 1e-6;
	size_t max_itr = 20;

	multi_newton(
		xout    ,
		fun     ,
		n_grid  ,
		xlow    ,
		xup     ,
		epsilon ,
		max_itr
	);
	return;
}

void test_repeat(size_t size, size_t repeat)
{	size_t i;
	CppAD::vector<double> xout;
	for(i = 0; i < repeat; i++)
		test_once(xout, size);
	return;
}

int main(void)
{
	using std::cout;
	using std::endl;
	using CppAD::vector;

	// size of the test cases
	vector<size_t> size_vec(2);
	size_vec[0] = 10;
	size_vec[1] = 20;

# ifdef _OPENMP
	// No tapes are currently active,
	// so we can set the maximum number of threads
	int i = omp_get_max_threads();
	assert( i > 0 );
	CppAD::AD<double>::omp_max_thread(size_t(i));
	cout << "OpenMP: version = "       << _OPENMP << endl;
	cout << "omp_get_max_threads() = " << i       << endl;
# else
	cout << "_OPENMP is not defined"       << endl;
	cout << "running in single tread mode" << endl;
# endif

	// solve once to check for correctness
	vector<double> xout;
	test_once(xout, size_vec[0]);
	cout << "Zeros found for sin(x) in the interval [0, 7*pi] " << endl;
	cout << xout << endl;

	// minimum time for test
	double time_min = 1.;

	// run the test cases
	vector<size_t> rate_vec( size_vec.size() );
	rate_vec = CppAD::speed_test(test_repeat, size_vec, time_min);

	// results
	cout << "size_vec = " << size_vec << endl;
	cout << "rate_vec = " << rate_vec << endl;

	return 0;
}

// END PROGRAM
