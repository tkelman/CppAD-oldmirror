/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin adolc_mat_mul.cpp$$
$spell
	sq
	retape
	adouble
	typedef
	alloc
	zos
	fos
	Adolc
	cppad.hpp
	bool
	mul
	dz
	CppAD
$$

$section Adolc Speed: Matrix Multiplication$$

$index link_mat_mul, adolc$$
$index adolc, link_mat_mul$$
$index speed, adolc$$
$index adolc, speed$$
$index matrix, speed adolc$$
$index multiply, speed adolc$$

$head Specifications$$
See $cref link_mat_mul$$.

$head Implementation$$

$codep */
# include <adolc/adolc.h>
# include <cppad/vector.hpp>
# include <cppad/speed/mat_sum_sq.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/vector.hpp>
bool link_mat_mul(
	size_t                           size     , 
	size_t                           repeat   , 
	CppAD::vector<double>&           x        ,
	CppAD::vector<double>&           z        ,
	CppAD::vector<double>&           dz       )
{
	// speed test global option values
	extern bool global_retape, global_atomic, global_optimize;
	if( global_atomic || global_optimize )
		return false; 

	// -----------------------------------------------------
	// setup
	typedef adouble    ADScalar;
	typedef ADScalar*  ADVector;

	int tag  = 0;         // tape identifier
	int m    = 1;         // number of dependent variables
	int n    = size*size; // number of independent variables
	double f;             // function value
	int j;                // temporary index

	// set up for thread_alloc memory allocator (fast and checks for leaks)
	using CppAD::thread_alloc; // the allocator
	size_t capacity;           // capacity of an allocation

	// AD domain space vector
	ADVector X = thread_alloc::create_array<ADScalar>(size_t(n), capacity);

	// Product matrix
	ADVector Y = thread_alloc::create_array<ADScalar>(size_t(n), capacity);

	// AD range space vector
	ADVector Z = thread_alloc::create_array<ADScalar>(size_t(m), capacity);

	// vector with matrix value
	double* mat = thread_alloc::create_array<double>(size_t(n), capacity);

	// vector of reverse mode weights
	double* u  = thread_alloc::create_array<double>(size_t(m), capacity);
	u[0] = 1.;

	// gradient
	double* grad = thread_alloc::create_array<double>(size_t(n), capacity);

	// ----------------------------------------------------------------------
	if( global_retape ) while(repeat--)
	{	// choose a matrix
		CppAD::uniform_01(n, mat);

		// declare independent variables
		int keep = 1; // keep forward mode results
		trace_on(tag, keep);
		for(j = 0; j < n; j++)
			X[j] <<= mat[j];

		// do computations
		CppAD::mat_sum_sq(size, X, Y, Z);

		// create function object f : X -> Z
		Z[0] >>= f;
		trace_off();

		// evaluate and return gradient using reverse mode
		fos_reverse(tag, m, n, u, grad);
	}
	else
	{	// choose a matrix
		CppAD::uniform_01(n, mat);

		// declare independent variables
		int keep = 0; // do not keep forward mode results
		trace_on(tag, keep);
		for(j = 0; j < n; j++)
			X[j] <<= mat[j];

		// do computations
		CppAD::mat_sum_sq(size, X, Y, Z);

		// create function object f : X -> Z
		Z[0] >>= f;
		trace_off();

		while(repeat--)
		{	// choose a matrix
			CppAD::uniform_01(n, mat);

			// evaluate the determinant at the new matrix value
			keep = 1; // keep this forward mode result
			zos_forward(tag, m, n, keep, mat, &f); 

			// evaluate and return gradient using reverse mode
			fos_reverse(tag, m, n, u, grad);
		}
	}
	// return function, matrix, and gradient
	z[0] = f;
	for(j = 0; j < n; j++)
	{	x[j]  = mat[j];
		dz[j] = grad[j];
	}

	// tear down
	thread_alloc::delete_array(X);
	thread_alloc::delete_array(Y);
	thread_alloc::delete_array(Z);
	thread_alloc::delete_array(mat);
	thread_alloc::delete_array(u);
	thread_alloc::delete_array(grad);

	return true;
}


/* $$
$end
*/
