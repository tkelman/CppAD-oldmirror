/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */

/*
$begin Integer.cpp$$
$spell
	Cpp
	cstddef
$$

$section Convert From AD to Integer: Example and Test$$

$index Integer$$
$index example, Integer$$
$index test, Integer$$

$code
$verbatim%Example/Integer.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool Integer(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::Integer;

	// domain space vector
	size_t n = 2;
	CppADvector< AD<double> > x(n);
	x[0] = 3.5;
	x[1] = 4.5;

	// check integer before recording
	ok &= (Integer(x[0]) == 3);
	ok &= (Integer(x[1]) == 4);

	// start recording

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// check integer during recording
	ok &= (Integer(x[0]) == 3);
	ok &= (Integer(x[1]) == 4);

	// check integer for VecAD element
	CppAD::VecAD<double> v(1);
	AD<double> zero(0);
	v[zero] = 2;
	ok &= (Integer(v[zero]) == 2);

	// range space vector 
	size_t m = 1;
	CppADvector< AD<double> > y(m);
	y[0] = - x[1];

	// create f: x -> y and stop recording
	CppAD::ADFun<double> f(x, y);

	// check integer after recording
	ok &= (Integer(x[0]) ==  3.);
	ok &= (Integer(x[1]) ==  4.);
	ok &= (Integer(y[0]) == -4.);

	return ok;
}
// END PROGRAM
