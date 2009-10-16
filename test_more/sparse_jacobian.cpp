/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Old sparse Jacobian example
*/

# include <cppad/cppad.hpp>
namespace { // ---------------------------------------------------------

template <class VectorBase, class VectorBool> 
bool reverse_bool()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	size_t i, j, k;

	// domain space vector
	size_t n = 4;
	CPPAD_TEST_VECTOR< AD<double> >  X(n);
	for(j = 0; j < n; j++)
		X[j] = AD<double> (0);

	// declare independent variables and starting recording
	CppAD::Independent(X);

	size_t m = 3;
	CPPAD_TEST_VECTOR< AD<double> >  Y(m);
	Y[0] = X[0] + X[1];
	Y[1] = X[2] + X[3];
	Y[2] = X[0] + X[1] + X[2] + X[3] * X[3] / 2.;

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// new value for the independent variable vector
	VectorBase x(n);
	for(j = 0; j < n; j++)
		x[j] = double(j);

	// Jacobian of y without sparsity pattern
	VectorBase jac(m * n);
	jac = f.SparseJacobian(x);
	/*
	      [ 1 1 0 0  ]
	jac = [ 0 0 1 1  ]
	      [ 1 1 1 x_3]
	*/
	VectorBase check(m * n);
	check[0] = 1.; check[1] = 1.; check[2]  = 0.; check[3]  = 0.;
	check[4] = 0.; check[5] = 0.; check[6]  = 1.; check[7]  = 1.;
	check[8] = 1.; check[9] = 1.; check[10] = 1.; check[11] = x[3];
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	// test passing sparsity pattern
	VectorBool s(m * m);
	VectorBool p(m * n);
	for(i = 0; i < m; i++)
	{	for(k = 0; k < m; k++)
			s[i * m + k] = false;
		s[i * m + i] = true;
	}
	p   = f.RevSparseJac(m, s);
	jac = f.SparseJacobian(x);
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	return ok;
}

template <class VectorBase, class VectorSet> 
bool reverse_set()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	size_t i, j, k;

	// domain space vector
	size_t n = 4;
	CPPAD_TEST_VECTOR< AD<double> >  X(n);
	for(j = 0; j < n; j++)
		X[j] = AD<double> (0);

	// declare independent variables and starting recording
	CppAD::Independent(X);

	size_t m = 3;
	CPPAD_TEST_VECTOR< AD<double> >  Y(m);
	Y[0] = X[0] + X[1];
	Y[1] = X[2] + X[3];
	Y[2] = X[0] + X[1] + X[2] + X[3] * X[3] / 2.;

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// new value for the independent variable vector
	VectorBase x(n);
	for(j = 0; j < n; j++)
		x[j] = double(j);

	// Jacobian of y without sparsity pattern
	VectorBase jac(m * n);
	jac = f.SparseJacobian(x);
	/*
	      [ 1 1 0 0  ]
	jac = [ 0 0 1 1  ]
	      [ 1 1 1 x_3]
	*/
	VectorBase check(m * n);
	check[0] = 1.; check[1] = 1.; check[2]  = 0.; check[3]  = 0.;
	check[4] = 0.; check[5] = 0.; check[6]  = 1.; check[7]  = 1.;
	check[8] = 1.; check[9] = 1.; check[10] = 1.; check[11] = x[3];
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	// test passing sparsity pattern
	VectorSet s(m), p(m);
	for(i = 0; i < m; i++)
		s[i].insert(i);
	p   = f.RevSparseJac(m, s);
	jac = f.SparseJacobian(x);
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	return ok;
}

template <class VectorBase, class VectorBool> 
bool forward_bool()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	size_t j, k;

	// domain space vector
	size_t n = 3;
	CPPAD_TEST_VECTOR< AD<double> >  X(n);
	for(j = 0; j < n; j++)
		X[j] = AD<double> (0);

	// declare independent variables and starting recording
	CppAD::Independent(X);

	size_t m = 4;
	CPPAD_TEST_VECTOR< AD<double> >  Y(m);
	Y[0] = X[0] + X[2];
	Y[1] = X[0] + X[2];
	Y[2] = X[1] + X[2];
	Y[3] = X[1] + X[2] * X[2] / 2.;

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// new value for the independent variable vector
	VectorBase x(n);
	for(j = 0; j < n; j++)
		x[j] = double(j);

	// Jacobian of y without sparsity pattern
	VectorBase jac(m * n);
	jac = f.SparseJacobian(x);
	/*
	      [ 1 0 1   ]
	jac = [ 1 0 1   ]
	      [ 0 1 1   ]
	      [ 0 1 x_2 ]
	*/
	VectorBase check(m * n);
	check[0] = 1.; check[1]  = 0.; check[2]  = 1.; 
	check[3] = 1.; check[4]  = 0.; check[5]  = 1.;
	check[6] = 0.; check[7]  = 1.; check[8]  = 1.; 
	check[9] = 0.; check[10] = 1.; check[11] = x[2];
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	// test passing sparsity pattern
	VectorBool r(n * n);
	VectorBool p(m * n);
	for(j = 0; j < n; j++)
	{	for(k = 0; k < n; k++)
			r[j * n + k] = false;
		r[j * n + j] = true;
	}
	p   = f.ForSparseJac(n, r);
	jac = f.SparseJacobian(x);
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	return ok;
}

template <class VectorBase, class VectorSet> 
bool forward_set()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	size_t j, k;

	// domain space vector
	size_t n = 3;
	CPPAD_TEST_VECTOR< AD<double> >  X(n);
	for(j = 0; j < n; j++)
		X[j] = AD<double> (0);

	// declare independent variables and starting recording
	CppAD::Independent(X);

	size_t m = 4;
	CPPAD_TEST_VECTOR< AD<double> >  Y(m);
	Y[0] = X[0] + X[2];
	Y[1] = X[0] + X[2];
	Y[2] = X[1] + X[2];
	Y[3] = X[1] + X[2] * X[2] / 2.;

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// new value for the independent variable vector
	VectorBase x(n);
	for(j = 0; j < n; j++)
		x[j] = double(j);

	// Jacobian of y without sparsity pattern
	VectorBase jac(m * n);
	jac = f.SparseJacobian(x);
	/*
	      [ 1 0 1   ]
	jac = [ 1 0 1   ]
	      [ 0 1 1   ]
	      [ 0 1 x_2 ]
	*/
	VectorBase check(m * n);
	check[0] = 1.; check[1]  = 0.; check[2]  = 1.; 
	check[3] = 1.; check[4]  = 0.; check[5]  = 1.;
	check[6] = 0.; check[7]  = 1.; check[8]  = 1.; 
	check[9] = 0.; check[10] = 1.; check[11] = x[2];
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	// test passing sparsity pattern
	VectorSet r(n), p(m);
	for(j = 0; j < n; j++)
		r[j].insert(j);
	p   = f.ForSparseJac(n, r);
	jac = f.SparseJacobian(x);
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool sparse_jacobian(void)
{	bool ok = true;
	typedef std::vector< std::set<size_t> >   vector_set;
	typedef std::valarray< std::set<size_t> > valarray_set;
	// ---------------------------------------------------------------
	// vector of bool cases
	ok &= forward_bool< CppAD::vector<double>, CppAD::vectorBool   >();
	ok &= reverse_bool< CppAD::vector<double>, CppAD::vector<bool> >();
	//
	ok &= forward_bool< std::vector<double>,   std::vector<bool>   >();
	ok &= reverse_bool< std::vector<double>,   std::valarray<bool> >();
	//
	ok &= forward_bool< std::valarray<double>, CppAD::vectorBool   >();
	ok &= reverse_bool< std::valarray<double>, CppAD::vector<bool> >();
	// ---------------------------------------------------------------
	// vector of set cases
	ok &= forward_set< CppAD::vector<double>, vector_set   >();
	ok &= reverse_set< CppAD::vector<double>, valarray_set >();
	//
	ok &= forward_set< std::vector<double>,   vector_set   >();
	ok &= reverse_set< std::vector<double>,   valarray_set >();
	//
	ok &= forward_set< std::valarray<double>, vector_set   >();
	ok &= reverse_set< std::valarray<double>, valarray_set >();
	// ---------------------------------------------------------------
	return ok;
}
