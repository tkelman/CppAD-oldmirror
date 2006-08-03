/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */

/*
Two old example now used just for valiadation testing
*/
# include <CppAD/CppAD.h>

namespace { // BEGIN empty namespace

bool AddEqOne(void)
{	bool ok = true;

	using namespace CppAD;


	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > U(2);
	size_t s = 0;
	size_t t = 1;
	U[s]     = 3.;
	U[t]     = 2.;
	Independent(U);

	// dependent variable vector and indices
	CppADvector< AD<double> > Z(2);
	size_t x = 0;
	size_t y = 1;

	// dependent variable values
	Z[x]  = 4.;
	Z[y]  = U[t];
	Z[x] += U[s];  // parameter += variable
	Z[x] += U[t];  // variable  += variable
	Z[y] += 5.;    // variable  += double

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );

	// check function values
	ok &= ( Z[x] == 4. + 3. + 2. );
	ok &= ( Z[y] == 2. + 5. );

	// forward computation of partials w.r.t. s
	v[s] = 1.;
	v[t] = 0.;
	w    = f.Forward(1, v);
	ok &= ( w[x] == 1. );  // dx/ds
	ok &= ( w[y] == 0. );  // dy/ds

	// reverse computation of second partials of x
	CppADvector<double> r( f.Domain() * 2 );
	w[x] = 1.;
	w[y] = 0.;
	r    = f.Reverse(2, w);
	ok &= ( r[2 * s + 1] == 0. );
	ok &= ( r[2 * t + 1] == 0. );

	return ok;
}

bool AddEqTwo(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector
	double u0 = .5;
	CppADvector< AD<double> > U(1);
	U[0]      = u0; 
	Independent(U);

	// dependent variable vector 
	CppADvector< AD<double> > Z(1);
	Z[0] = U[0];       // initial value
	Z[0] += 2;         // AD<double> += int
	Z[0] += 4.;        // AD<double> += double
	Z[0] += U[0];      // AD<double> += AD<double> 

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z); 
	CppADvector<double> v(1);
	CppADvector<double> w(1);

	// check value 
	ok &= NearEqual(Z[0] , u0+2+4+u0,  1e-10 , 1e-10);

	// forward computation of partials w.r.t. u
	size_t j;
	size_t p     = 5;
	double jfac  = 1.;
	double value = 2.;
	v[0]         = 1.;
	for(j = 1; j < p; j++)
	{	jfac *= j;
		w     = f.Forward(j, v);	
		ok &= NearEqual(jfac*w[0], value, 1e-10 , 1e-10); // d^jz/du^j
		v[0]  = 0.;
		value = 0.;
	}

	// reverse computation of partials of Taylor coefficients
	CppADvector<double> r(p); 
	w[0]  = 1.;
	r     = f.Reverse(p, w);
	jfac  = 1.;
	value = 2.;
	for(j = 0; j < p; j++)
	{	ok &= NearEqual(jfac*r[j], value, 1e-10 , 1e-10); // d^jz/du^j
		jfac *= (j + 1);
		value = 0.;
	}

	return ok;
}

} // END empty namespace

bool AddEq(void)
{	bool ok = true;
	ok     &= AddEqOne();
	ok     &= AddEqTwo();
	return ok;
}

// END PROGRAM
