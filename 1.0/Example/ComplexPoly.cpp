// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

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
// END SHORT COPYRIGHT

/*
$begin ComplexPoly.cpp$$
$spell
	Cpp
$$

$section Complex Polynomial: Example and Test$$

$mindex complex polynomial$$
$index example, complex polynomial$$
$index test, complex polynomial$$

Select this link to view specifications for $xref/Poly/$$:

$comment This file is in the Example subdirectory$$ 
$code
$verbatim%Example/ComplexPoly.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <complex>

// define abbreviation for doubler precision complex
typedef std::complex<double> Complex; 

bool ComplexPoly(void)
{	bool ok = true;

	using namespace CppAD;

	size_t deg = 4;

	// polynomial coefficients
	CppADvector< Complex >     a   (deg + 1); // coefficients for p(z)
	CppADvector< AD<Complex> > A   (deg + 1); 
	size_t i;
	for(i = 0; i <= deg; i++)
		A[i] = a[i] = Complex(i, i);

	// independent variable vector, indices, values, and declaration
	CppADvector< AD<Complex> > Z(1);
	size_t z = 0;
 	Z[z]     = Complex(1., 1.);
	Independent(Z);

	// dependent variable vector and indices
	CppADvector< AD<Complex> > P(1);
	size_t p = 0;

	// dependent variable values
	P[p] = Poly(0, A, Z[z]);

	// create f: Z -> P and vectors used for derivative calculations
	ADFun<Complex> f(Z, P);
	CppADvector<Complex> v( f.Domain() );
	CppADvector<Complex> w( f.Range() );

	// check first derivative w.r.t z
	v[z] = 1.;
	w    = f.Forward(1, v);
	ok &= ( w[p]  == Poly(1, a, Value(Z[z]) ) );

	// second derivative w.r.t z is 2 times its second order Taylor coeff
	v[z] = 0.;
	w    = f.Forward(2, v);
	ok &= ( 2. * w[p]  == Poly(2, a, Value(Z[z]) ) );

	return ok;
}

// END PROGRAM
