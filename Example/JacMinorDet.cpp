// BEGIN SHORT COPYRIGHT
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */
// END SHORT COPYRIGHT

/*
$begin JacMinorDet.cpp$$
$spell
	Cpp
$$

$section Gradient of Determinant Using Expansion by Minors: Example and Test$$
$mindex gradient minors expansion$$
$index example, gradient$$
$index test, gradient$$
$index example, minors expansion$$
$index test, minors expansion$$

$comment This file is in the Example subdirectory$$ 
$code
$verbatim%Example/JacMinorDet.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include "DetByMinor.h"
# include <complex>

typedef std::complex<double>   Complex;
typedef CppAD::AD<Complex>   ADComplex;

// ----------------------------------------------------------------------------

bool JacMinorDet()
{	bool ok = true;

	using namespace CppAD;

	size_t n = 2;

	// object for computing determinant
	DetByMinor<ADComplex> Det(n);

	// independent and dependent variable vectors
	CppADvector<ADComplex>  X(n * n);
	CppADvector<ADComplex>  D(1);

	// value of the independent variable
	size_t i;
	for(i = 0; i < n * n; i++)
		X[i] = Complex(int(i), -int(i));

	// set the independent variables
	Independent(X);

	// comupute the determinant
	D[0] = Det(X); 

	// create the function object
	ADFun<Complex> f(X, D);

	// argument value
	CppADvector<Complex>     x( n * n );
	for(i = 0; i < n * n; i++)
		x[i] = Complex(2 * i, i);

	// first derivative of the determinant
	CppADvector<Complex> J( n * n );
	J = f.Jacobian(x);

	/*
	f(x)     = x[0] * x[3] - x[1] * x[2]
	f'(x)    = ( x[3], -x[2], -x[1], x[0] )
	*/
	Complex Jtrue[] = { x[3], -x[2], -x[1], x[0] };
	for(i = 0; i < n * n; i++)
		ok &= Jtrue[i] == J[i];

	return ok;

}

// END PROGRAM
