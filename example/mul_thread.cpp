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

$section Multiple Active Tapes (Simulated Multi-threading)$$

$index tape_max_active$$
$index example, tape_max_active$$
$index test, tape_max_active$$

$index thread, multiple$$
$index multiple, thread$$
$index multiple, active tapes$$
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
	using CppAD::vector;

	// Number of simulated threads
	size_t n_thread = 10;
	CppAD::AD<double>::tape_max_active(n_thread);

	// Buffer (one per thread)
	vector< vector< AD<double> > > X(n_thread);
	vector< vector< AD<double> > > Y(n_thread);
	vector< CppAD::ADFun<double> > F(n_thread);

	// repeat the tests multiple times to make sure call to
	// F.Dependent makes the corresponding tape inactive
	size_t repeat;
	for(repeat = 0; repeat < 3; repeat++)
	{	// create n_thread active tapes
		size_t n = 1;         
		size_t i_thread;
		for(i_thread = 0; i_thread < n_thread; i_thread++)
		{	// domain space vector for this thread
			size_t n = 1; 
			X[i_thread].resize(n);
			X[i_thread][0] = 0.;

			// declare independent variables and create a new
			// active tape recording.
			Independent( X[i_thread] );
		}

		// compute the function value for each active tape
		size_t m = 1;
		for(i_thread = 0; i_thread < n_thread; i_thread++)
		{	// set Y0 to X0 raised to the i_thread power
			AD<double> X0 = X[i_thread][0];
			AD<double> Y0 = 1.;
			size_t k;
			for(k = 0; k < i_thread; k++)
				Y0 *= X0;

			// range space vector for this thread
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
		{	// F[i_thread] : X[i_thread] -> Y[i_thread]
			F[i_thread].Dependent( X[i_thread], Y[i_thread] );
		}

		// check function values for each F[i_thread]
		vector<double> x(n), y(m);
		x[0]  = 2.;
		double check = 1.;
		for(i_thread = 0; i_thread < n_thread; i_thread++)
		{	y      = F[i_thread].Forward(0, x);
			ok    &= CppAD::NearEqual(y[0], check, 1e-10, 1e-10);
			check *= x[0];
		} 	 
	}

	return ok;
}

// END PROGRAM
