/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>

namespace {
	template <class Vector>
	void fun(const Vector& x, Vector& y)
	{	typedef typename Vector::value_type Scalar;

		// an operation that is optimized out
		Scalar a = 1. / x[0];

		if( a < x[0] )
			y[0] = x[0] + 3.;
		else	y[0] = x[0] + 2.;

		return;
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
	fun(X, Y);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> F;
	F.Dependent(X, Y); 

	CPPAD_TEST_VECTOR<double> x(n), y(m), check(m);
	size_t i, j;
	for(j = 0; j < n; j++)
		x[j] = Value(X[j]);
	y = F.Forward(0, x);
	fun(x, check);
	for(i = 0; i < m; i++)
		ok &= (y[i] == check[i]);

	// Optimize the operation sequence
	F.optimize();

	// check result now
	y = F.Forward(0, x);
	for(i = 0; i < m; i++)
		ok &= (y[i] == check[i]);

	return ok;
}
