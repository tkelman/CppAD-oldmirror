/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin mul_thread.cpp$$

$section Simulate Multiple Threads Using Multiple Active Tapes$$

$index multiple, active tapes$$
$index tape, multiple active$$
$index active, multiple tapes$$

$code
$verbatim%example/mul_thread.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>

# define CPPAD_DEMONSTRATE_ERROR 0 // change 0 to 1 to demonstrate error

bool mul_thread(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	using CppAD::vector;
	using CppAD::ADFun;

	// Allow for n_thread tapes at once
	size_t n_thread = 10;
	CppAD::AD<double>::tape_max_active(n_thread);

	// create an array of vectors
	vector< vector< AD<double> > > X(n_thread);
	vector< vector< AD<double> > > Y(n_thread);
	vector< ADFun<double> >        F(n_thread);

	// repeat multiple times to make sure tapes become inactive
	// when ADFun<double>::Dependent is called.
	size_t repeat;
	for(repeat = 0; repeat < 3; repeat++)
	{	// create n_thread active tapes
		size_t n = 1;
		size_t i_thread;
		for(i_thread = 0; i_thread < n_thread; i_thread++)
		{	X[i_thread].resize(n);
			X[i_thread][0] = 0.;
			Independent( X[i_thread] );
		}

		// compute the function value for each active tape
		size_t m = 1;
		for(i_thread = 0; i_thread < n_thread; i_thread++)
		{	AD<double> X0 = X[i_thread][0];
			AD<double> Y0 = 1.;
			size_t k;
			for(k = 0; k < i_thread; k++)
				Y0 *= X0;
			Y[i_thread].resize(m);
			Y[i_thread][0] = Y0;
		}

		// Y[i_thread][0] is a variable for the corresponding tape
		// It is an error to perform a binary operation using 
		// variables from two different tapes. 
# if CPPAD_DEMONSTRATE_ERROR
		Y[0][0] = Y[0][0] * Y[1][0];
# endif

		// create a function and inactive tapes
		for(i_thread = 0; i_thread < n_thread; i_thread++)
			F[i_thread].Dependent( X[i_thread], Y[i_thread] );

		// check the function value for each of the functions
		vector<double> x(n), y(m);
		x[0]  = 2.;
		double check = 1.;
		for(i_thread = 0; i_thread < n_thread; i_thread++)
		{	y      = F[i_thread].Forward(0, x);
			ok    &= NearEqual(y[0], check, 1e-10, 1e-10);
			check *= x[0];
		} 	 
	}

	return ok;
}

// END PROGRAM
