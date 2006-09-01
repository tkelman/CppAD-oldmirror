// BEGIN SHORT COPYRIGHT
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
-------------------------------------------------------------------------- */
// END SHORT COPYRIGHT
/*
$begin DetMinorSpeed.cpp$$
$spell
	Cpp
$$

$section Determinant Using Expansion by Minors: Speed Test$$
$index speed, determinant by minors$$
$index determinant, minors speed$$


$comment This file is in the Speed subdirectory$$ 
$code
$verbatim%Speed/DetLu.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include "../Example/DetByMinor.h"

# include <sstream>
# include <string>

std::string doubleDetMinor(size_t size, size_t repeat)
{	size_t i;
	size_t j;

	using namespace std;
	using namespace CppAD;

	CppADvector<double> A( size * size );

	for( i = 0; i < size; i++)
		for(j = 0; j < size; j++)
			A[i + j * size] = double ( i / (double) (j + 1.) );

	DetByMinor<double> DetDouble(size);

	while(repeat--)
		DetDouble(A);

	return "double: DetMinor";
}

std::string ADdoubleDetMinor(size_t size, size_t repeat)
{	size_t i;
	size_t j;

	using namespace std;
	using namespace CppAD;

	CppADvector< AD<double> > A( size * size );

	for( i = 0; i < size; i++)
		for(j = 0; j < size; j++)
			A[i + j * size] =  AD<double>( i / (double) (j + 1.) );

	DetByMinor< AD<double> > DetADdouble(size);

	while(repeat--)
		DetADdouble(A);

	return "ADdouble: DetMinor";
}

std::string TapeDetMinor(size_t size, size_t repeat)
{	size_t i;
	size_t j;

	using namespace std;
	using namespace CppAD;

	CppADvector<double>       a( size * size );
	CppADvector<double>       J( size * size );

	CppADvector< AD<double> > A( size * size );
	CppADvector< AD<double> > D(1);

	for( i = 0; i < size; i++)
		for(j = 0; j < size; j++)
			A[i + j * size] = a[i] = double( i / (double) (j + 1.));

	DetByMinor< AD<double> > DetADdouble(size);

	size_t memory = 0;
	size_t length = 0;
	while(repeat--)
	{	// declare the independent variables
		Independent(A);

		// tape the evaluation
		D[0] = DetADdouble(A);

		// create f : A -> D
		ADFun<double> f(A, D);

		// save for later return
		length = f.Size();
		memory = f.Memory();
	}

	ostringstream buf;
	buf << "Tape of Expansion by Minors Determinant: Length = "
	    << length << ", Memory = " << memory;

	return buf.str();
}

std::string JacDetMinor(size_t size, size_t repeat)
{	size_t i;
	size_t j;

	using namespace std;
	using namespace CppAD;

	CppADvector<double>       a( size * size );
	CppADvector<double>       J( size * size );

	CppADvector< AD<double> > A( size * size );
	CppADvector< AD<double> > D(1);

	for( i = 0; i < size; i++)
		for(j = 0; j < size; j++)
			A[i + j * size] = a[i] = double( i / (double) (j + 1.));

	DetByMinor< AD<double> > DetADdouble(size);

	size_t memory = 0;
	size_t length = 0;
	while(repeat--)
	{	// declare the independent variables
		Independent(A);

		// tape the evaluation
		D[0] = DetADdouble(A);

		// create f : A -> D
		ADFun<double> f(A, D);

		// evaluate the Jacobian of f
		J = f.Jacobian(a);

		// save for later return
		length = f.Size();
		memory = f.Memory();
	}

	ostringstream buf;
	buf << "Jacobian of Expansion by Minors Determinant: Length = "
	    << length << ", Memory = " << memory;

	return buf.str();
}

std::string HesDetMinor(size_t size, size_t repeat)
{	size_t i;
	size_t j;

	using namespace std;
	using namespace CppAD;

	CppADvector<double>       a( size * size );
	CppADvector<double>       H( size * size * size * size );

	CppADvector< AD<double> > A( size * size );
	CppADvector< AD<double> > D(1);

	for( i = 0; i < size; i++)
		for(j = 0; j < size; j++)
			A[i + j * size] = a[i] = double( i / (double) (j + 1.));

	DetByMinor< AD<double> > DetADdouble(size);

	size_t memory = 0;
	size_t length = 0;
	while(repeat--)
	{	// declare the independent variables
		Independent(A);

		// tape the evaluation
		D[0] = DetADdouble(A);

		// create f : A -> D
		ADFun<double> f(A, D);

		// evaluate the Hessian of f
		H = f.Hessian(a, 0);

		// save for later return
		length = f.Size();
		memory = f.Memory();
	}

	ostringstream buf;
	buf << "Hessian of Expansion by Minors Determinant: Length = "
	    << length << ", Memory = " << memory;

	return buf.str();
}

// END PROGRAM
