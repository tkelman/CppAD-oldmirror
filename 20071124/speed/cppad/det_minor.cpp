/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_det_minor.cpp$$
$spell
	vector Vector
	typedef
	cppad
	Lu
	CppAD
	det
	hpp
	const
	CPPAD_TEST_VECTOR
	bool
	srand
$$

$section CppAD Speed: Gradient of Determinant Using Expansion by Minors$$

$index cppad, speed minor$$
$index speed, cppad minor$$
$index minor, speed cppad$$

$head Operation Sequence$$
Note that the expansion by minors 
$cref/operation sequence/glossary/Operation/Sequence/$$
does not depend on the matrix being factored.
Hence we use the same $cref/ADFun/$$ object for all the matrices.

$head compute_det_minor$$
$index compute_det_minor$$
Routine that computes the gradient of determinant using CppAD:
$codep */
# include <cppad/vector.hpp>
# include <cppad/speed/det_by_minor.hpp>
# include <cppad/speed/uniform_01.hpp>

void compute_det_minor(
	size_t                     size     , 
	size_t                     repeat   , 
	CppAD::vector<double>     &matrix   ,
	CppAD::vector<double>     &gradient )
{
	// -----------------------------------------------------
	// setup

	// object for computing determinant
	typedef CppAD::AD<double>       ADScalar; 
	typedef CppAD::vector<ADScalar> ADVector; 
	CppAD::det_by_minor<ADScalar>   Det(size);

	size_t i;               // temporary index
	size_t m = 1;           // number of dependent variables
	size_t n = size * size; // number of independent variables
	ADVector   A(n);        // AD domain space vector
	ADVector   detA(m);     // AD range space vector
	
	// vectors of reverse mode weights 
	CppAD::vector<double> w(1);
	w[0] = 1.;

	// choose a matrix
	CppAD::uniform_01(n, matrix);
	for( i = 0; i < size * size; i++)
		A[i] = matrix[i];

	// declare independent variables
	Independent(A);

	// AD computation of the determinant
	detA[0] = Det(A);

	// create function object f : A -> detA
	CppAD::ADFun<double> f(A, detA);

	// ------------------------------------------------------
	while(repeat--)
	{	// get the next matrix
		CppAD::uniform_01(n, matrix);

		// evaluate the determinant at the new matrix value
		f.Forward(0, matrix);

		// evaluate and return gradient using reverse mode
		gradient = f.Reverse(1, w);
	}
	return;
}
/* $$
$end
*/