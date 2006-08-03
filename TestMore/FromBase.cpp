/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */


/*
Old FromBase example now used just for valiadation testing
*/
# include <CppAD/CppAD.h>

bool FromBase(void)
{	bool ok = true;

	using namespace CppAD;

	// construct directly form Base where Base = double 
	AD<double> x(1.); 
	AD<double> y = 2.;

	// construct from a type that can be converted to Base 
	// where Base = AD<double>
	AD< AD<double> > X(1.); 
	AD< AD<double> > Y(2);

	// check that resulting objects are parameters
	ok &= Parameter(x);
	ok &= Parameter(y);

	ok &= Parameter(X);
	ok &= Parameter(Y);

	// check values of objects
	ok &= (x == 1.);
	ok &= (X == x);

	ok &= (y == 2.);
	ok &= (Y == y);

	// user constructor through the static_cast template function
	x   = static_cast < AD<double> >( 4 );
	X   = static_cast < AD< AD<double> > >( 4 );

	ok &= (x == 4.);
	ok &= (X == x);

	return ok;
}

// END PROGRAM
