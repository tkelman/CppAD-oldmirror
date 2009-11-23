/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin optimize.cpp$$

$section ADFun Operation Sequence Optimization: Example and Test$$

$index optimize, operation sequence$$
$index operation, optimize sequence$$
$index sequence, optimize operation$$
$index test, optimize$$
$index example, optimize$$

$code
$verbatim%example/optimize.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
namespace {
	template <class Float>
	Float fun(const Float& x)
	{	// return value
		Float y;

	 	// This variable is optimized out because it is only used
	 	// in the comparision operation
		Float a = 1. / x;

		if( a < x )  
			y = x + 3.; // only one variable created by this choice
		else	y = x + 2.;

		return y;
	}
}

bool optimize(void)
{	bool ok = true;
	using CppAD::AD;

	// domain space vector
	size_t n  = 1;
	CPPAD_TEST_VECTOR< AD<double> > X(n);
	X[0]      = .5; 

	// declare independent variables and start tape recording
	CppAD::Independent(X);

	// range space vector 
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > Y(m);
	Y[0] = fun(X[0]);

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> F(X, Y);

	// Check the number of variables in original operation sequence:
	// NonOp: at the beginning of every operation sequence.
	// X[0]:  the independent variable.
	// a:     the temporary variable inside the function fun.
	// y:     the return value for the function fun and dependent variable.
	// NonOp: at the end of every operation sequence.
	ok &= (F.size_var() == 5);

	// Check zero order forward mode on the original operation sequence
	CPPAD_TEST_VECTOR<double> x(n), y(m), check(m);
	x[0] = Value(X[0]);
	y = F.Forward(0, x);
	ok &= y[0] == fun(x[0]);

	// Optimize the operation sequence
	F.optimize();

	// Check the number of variables in original operation sequence:
	// NonOp: at the beginning of every operation sequence.
	// X[0]:  the independent variable.
	// y:     the return value for the function fun and dependent variable.
	// NonOp: at the end of every operation sequence.
	ok &= (F.size_var() == 4);

	// Check result for a zero order calculation.
	// This has already been checked if NDEBUG is not defined.
	y = F.Forward(0, x);
	ok &= y[0] == fun(x[0]);
	return ok;
}

// END PROGRAM
