/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
-------------------------------------------------------------------------- */

/*
Old example now just used for validation testing
*/

# include <CppAD/CppAD.h>

bool Erf(void)
{	bool ok = true;
	using namespace CppAD;
	using CppAD::atan;
	using CppAD::exp;
	using CppAD::sqrt;

	// Construct function object corresponding to erf
	CppADvector< AD<double> > X(1);
	CppADvector< AD<double> > Y(1);
	X[0] = 0.;
	Independent(X);
	Y[0] = erf(X[0]);
	ADFun<double> Erf(X, Y);

	// vectors to use with function object
	CppADvector<double> x(1);
	CppADvector<double> y(1);
	CppADvector<double> dx(1);
	CppADvector<double> dy(1);

	// check value at zero
	x[0]  = 0.;
	y = Erf.Forward(0, x);	
	ok &= NearEqual(0., y[0], 1e-10, 1e-10);

	// check the derivative of error function
	dx[0]         = 1.;
	double pi     = 4. * atan(1.);
	double factor = 2. / sqrt( pi );
	int i;
	for(i = -30; i <= 30; i++)
	{	x[0] = i / 4.;
		y    = Erf.Forward(0, x);	

		// check derivative
		double derf = factor * exp( - x[0] * x[0] );
		dy          = Erf.Forward(1, dx);
		ok         &= NearEqual(derf, dy[0], 1e-10, 1e-10);

		// test using erf with AD< AD<double> >
		AD< AD<double> > X0 = x[0];
		AD< AD<double> > Y0 = erf(X0);

		ok &= ( y[0] == Value( Value(Y0) ) );
	}
	return ok;
}
