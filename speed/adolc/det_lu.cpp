/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin adolc_det_lu.cpp$$
$spell
	omp_alloc
	cppad
	fos
	adouble
	CppAD
	typedef
	adolc
	Lu
	Adolc
	det
	hpp
	const
	bool
	srand
$$

$section Adolc Speed: Gradient of Determinant Using Lu Factorization$$

$index adolc, speed lu$$
$index speed, adolc lu$$
$index lu, speed adolc$$

$head Specifications$$
See $cref/link_det_lu/$$.

$head Implementation$$
$index adolc, link_det_lu$$
$index link_det_lu, adolc$$
$codep */
# include <cppad/speed/det_by_lu.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/track_new_del.hpp>

# include <adolc/adouble.h>
# include <adolc/taping.h>
# include <adolc/interfaces.h>

bool link_det_lu(
	size_t                     size     , 
	size_t                     repeat   , 
	CppAD::vector<double>     &matrix   ,
	CppAD::vector<double>     &gradient )
{
	// -----------------------------------------------------
	// setup
	int tag  = 0;         // tape identifier
	int keep = 1;         // keep forward mode results in buffer
	int m    = 1;         // number of dependent variables
	int n    = size*size; // number of independent variables
	double f;             // function value
	int j;                // temporary index

	// set up for omp_alloc memory allocator (fast and checks for leaks)
	using CppAD::omp_alloc; // the allocator
	size_t capacity;        // capacity of an allocation

	// object for computing determinant
	typedef adouble    ADScalar;
	typedef ADScalar*  ADVector;
	CppAD::det_by_lu<ADScalar> Det(size);

	// AD value of determinant
	ADScalar   detA;

	// AD version of matrix
	ADVector A  = omp_alloc::create_array<ADScalar>(n, capacity);
	
	// vectors of reverse mode weights 
	double* u   = omp_alloc::create_array<double>(m, capacity);
	u[0] = 1.;

	// vector with matrix value
	double* mat  = omp_alloc::create_array<double>(n, capacity);

	// vector to receive gradient result
	double* grad = omp_alloc::create_array<double>(n, capacity);
	// ------------------------------------------------------
	while(repeat--)
	{	// get the next matrix
		CppAD::uniform_01(n, mat);

		// declare independent variables
		trace_on(tag, keep);
		for(j = 0; j < n; j++)
			A[j] <<= mat[j];

		// AD computation of the determinant
		detA = Det(A);

		// create function object f : A -> detA
		detA >>= f;
		trace_off();

		// evaluate and return gradient using reverse mode
		fos_reverse(tag, m, n, u, grad);
	}
	// ------------------------------------------------------

	// return matrix and gradient
	for(j = 0; j < n; j++)
	{	matrix[j] = mat[j];
		gradient[j] = grad[j];
	}
	// tear down
	omp_alloc::delete_array(grad);
	omp_alloc::delete_array(mat);
	omp_alloc::delete_array(u);
	omp_alloc::delete_array(A);

	return true;
}
/* $$
$end
*/
