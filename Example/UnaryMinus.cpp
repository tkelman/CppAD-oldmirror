/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */

/*
$begin UnaryMinus.cpp$$
$spell
	Cpp
	cstddef
$$

$section AD Unary Minus Operator: Example and Test$$

$index unary minus, example$$
$index example, unary minus$$
$index test, unary minus$$

$code
$verbatim%Example/UnaryMinus.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool UnaryMinus(void)
{	bool ok = true;
	using CppAD::AD;


	// domain space vector
	size_t n = 1;
	CppADvector< AD<double> > x(n);
	x[0]      = 3.;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// range space vector 
	size_t m = 1;
	CppADvector< AD<double> > y(m);
	y[0] = - x[0];

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y);

	// check values
	ok &= ( y[0] == -3. );

	// forward computation of partials w.r.t. x[0]
	CppADvector<double> dx(n);
	CppADvector<double> dy(m);
	size_t p = 1;
	dx[0]    = 1.;
	dy       = f.Forward(p, dx);
	ok      &= ( dy[0] == -1. );   // dy[0] / dx[0]

	// reverse computation of dertivative of y[0]
	CppADvector<double>  w(m);
	CppADvector<double> dw(n);
	w[0] = 1.;
	dw   = f.Reverse(p, w);
	ok &= ( dw[0] == -1. );       // dy[0] / dx[0]

	// use a VecAD<Base>::reference object with unary minus
	CppAD::VecAD<double> v(1);
	AD<double> zero(0);
	v[zero] = x[0];
	AD<double> result = - v[zero];
	ok     &= (result == y[0]);
	 
	return ok;
}
// END PROGRAM
