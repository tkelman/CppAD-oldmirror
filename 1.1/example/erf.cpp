/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Erf.cpp$$
$spell
	tan
	erf
$$

$section The AD erf Function: Example and Test$$

$index erf, AD example$$
$index example, AD erf$$
$index test, AD erf$$

$code
$verbatim%example/erf.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <cmath>

bool Erf(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n  = 1;
	double x0 = 0.5;
	CppADvector< AD<double> > x(n);
	x[0]      = x0;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// a temporary value

	// range space vector 
	size_t m = 1;
	CppADvector< AD<double> > y(m);
	y[0] = CppAD::erf(x[0]);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// check value 
	double erf_x0 = 0.5205;
	ok &= NearEqual(y[0] , erf_x0,  1e-4 , 1e-4);

	// value of derivative of erf at x0
	double pi     = 4. * std::atan(1.);
	double factor = 2. / sqrt(pi);
	double check  = factor * std::exp(-x0 * x0);

	// forward computation of first partial w.r.t. x[0]
	CppADvector<double> dx(n);
	CppADvector<double> dy(m);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], check, 1e-10, 1e-10);

	// reverse computation of derivative of y[0]
	CppADvector<double>  w(m);
	CppADvector<double> dw(n);
	w[0]  = 1.;
	dw    = f.Reverse(1, w);
	ok   &= NearEqual(dw[0], check, 1e-10, 1e-10);

	// use a VecAD<Base>::reference object with erf
	CppAD::VecAD<double> v(1);
	AD<double> zero(0);
	v[zero]           = x0;
	AD<double> result = CppAD::erf(v[zero]);
	ok   &= NearEqual(result, y[0], 1e-10, 1e-10);

	// use a double with erf
	ok   &= NearEqual(CppAD::erf(x0), y[0], 1e-10, 1e-10);

	return ok;
}

// END PROGRAM
