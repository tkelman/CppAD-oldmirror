// BEGIN SHORT COPYRIGHT
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */
// END SHORT COPYRIGHT

/*
Old Value example now used just for valiadation testing
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool Value(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > U(2);
	size_t s = 0;
	size_t t = 1;
	U[s] = 3.;
	U[t] = 4.;
	Independent(U);

	// cannot call Value after Independent (tape is recording)

	// dependent variable vector and indices
	CppADvector< AD<double> > Z(1);
	size_t x = 0;

	// dependent variable values
	Z[x] = - U[t];

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );

	// can call Value after ADFun constructor (tape is no longer recording)

	// check value of s
	double sValue = Value(U[s]);
	ok &= ( sValue == 3. );

	// check value of x
	double xValue = Value(Z[x]);
	ok &= ( xValue == -4. );

	return ok;
}
// END PROGRAM
