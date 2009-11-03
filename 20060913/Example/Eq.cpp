/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */

/*
$begin Eq.cpp$$
$spell
	Cpp
	cstddef
$$

$section AD Assignment Operator: Example and Test$$

$index assignment, AD$$
$index example, AD assignment$$
$index test, AD assignment$$

$code
$verbatim%Example/Eq.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool Eq(void)
{	bool ok = true;
	using CppAD::AD;

	// domain space vector
	size_t n = 2;
	CppADvector< AD<double> > x(n);
	x[0]     = 5.;     // AD<double> = double
	x[1]     = x[0];   // AD<double> = AD<double>

	// declare independent variables and start tape recording
	CppAD::Independent(x);
	
	// range space vector 
	size_t m = 3;
	CppADvector< AD<double> > y(m);

	// assign an AD<Base> object equal to an expression and 
	// use the value returned by the assignment (for another assignment)
	y[0] = y[1] = x[1] + 1.;  

	// assign an AD<Base> object equal to an independent variable
	// (choose the first independent variable to check a special case)
	y[2] = x[0];  

	// check that all the resulting components of y depend on x
	ok &= Variable(y[0]);  // y[0] = x[1] + 1
	ok &= Variable(y[1]);  // y[1] = x[1] + 1
	ok &= Variable(y[2]);  // y[2] = x[0]

	// construct f : x -> y and stop the tape recording
	CppAD::ADFun<double> f(x, y);

	// check variable values
	ok &= ( y[0] == 6.);
	ok &= ( y[1] == 6.);
	ok &= ( y[2] == 5.);

	// compute partials w.r.t x[1]
	CppADvector<double> dx(n);
	CppADvector<double> dy(m);
	dx[0] = 0.;
	dx[1] = 1.;
	dy   = f.Forward(1, dx);
	ok  &= (dy[0] == 1.);  // dy[0] / dx[1]
	ok  &= (dy[1] == 1.);  // dy[1] / dx[1]
	ok  &= (dy[2] == 0.);  // dy[2] / dx[1]

	// compute the derivative y[2]
	CppADvector<double>  w(m);
	CppADvector<double> dw(n);
	w[0] = 0.;
	w[1] = 0.;
	w[2] = 1.;
	dw   = f.Reverse(1, w);
	ok  &= (dw[0] == 1.);  // dy[2] / dx[0]
	ok  &= (dw[1] == 0.);  // dy[2] / dx[1]

	// assign a VecAD<Base>::reference
	CppAD::VecAD<double> v(1);
	AD<double> zero(0);
	v[zero] = 5.;
	ok     &= (v[0] == 5.);

	return ok;
}

// END PROGRAM