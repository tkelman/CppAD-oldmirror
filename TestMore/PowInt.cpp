/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */

/*
Old example now just used for validation testing.
*/

# include <CppAD/CppAD.h>

bool PowInt(void)
{	bool ok = true;

	using CppAD::pow;
	using CppAD::exp;
	using CppAD::log;
	using namespace CppAD;


	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > U(1);
	U[0]     = 2.;
	Independent(U);

	// dependent variable vector and indices
	CppADvector< AD<double> > Z(2);

	// dependent variable values
	Z[0]         = pow(U[0], 5);     // x = u^5
	Z[1]         = pow(U[0], -5);    // y = u^{-5}

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );

	/*
	x_u = 5 * u^4
	y_u = - 5 * u^{-6}
	*/

	// check function values values
	double u = Value(U[0]);
	ok &= NearEqual(Z[0] , exp( log(u) * 5.),              1e-10 , 1e-10);
	ok &= NearEqual(Z[1] , exp( - log(u) * 5.),            1e-10 , 1e-10);

	// forward computation of partials 
	v[0] = 1.;
	w = f.Forward(1, v);
	ok &= NearEqual(w[0] , 5. * exp( log(u) * 4.),         1e-10 , 1e-10);
	ok &= NearEqual(w[1] , - 5. * exp( - log(u) * 6.),     1e-10 , 1e-10);

	return ok;
}
