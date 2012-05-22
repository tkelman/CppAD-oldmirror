/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sparse_hessian.cpp$$
$spell
	Cpp
	Hessian
$$

$section Sparse Hessian: Example and Test$$

$index Hessian, sparse$$
$index example, sparse Hessian$$
$index test, sparse Hessian$$
$index spare, Hessian example$$

$code
$verbatim%example/sparse_hessian.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
bool sparse_hessian(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	size_t i, j, k, ell;
	typedef CPPAD_TEST_VECTOR< AD<double> >               a_vector;
	typedef CPPAD_TEST_VECTOR<double>                     d_vector;
	typedef CPPAD_TEST_VECTOR<size_t>                     i_vector;
	typedef CPPAD_TEST_VECTOR<bool>                       b_vector;
	typedef CPPAD_TEST_VECTOR< std::set<size_t> >         s_vector;
	double eps = 10. * CppAD::epsilon<double>();

	// domain space vector
	size_t n = 3;
	a_vector a_x(n);
	for(j = 0; j < n; j++)
		a_x[j] = AD<double> (0);

	// declare independent variables and starting recording
	CppAD::Independent(a_x);

	size_t m = 1;
	a_vector a_y(m);
	a_y[0] = a_x[0]*a_x[0] + a_x[0]*a_x[1] + a_x[1]*a_x[1] + a_x[2]*a_x[2];

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(a_x, a_y);

	// new value for the independent variable vector
	d_vector x(n);
	for(j = 0; j < n; j++)
		x[j] = double(j);

	// -------------------------------------------------------------------
	// second derivative of y[0] w.r.t x
	d_vector w(m), h(n * n), check(n * n);
	w[0] = 1.;
	h = f.SparseHessian(x, w);
	/*
	    [ 2 1 0 ]
	h = [ 1 2 0 ]
	    [ 0 0 2 ]
	*/
	check[0] = 2.; check[1] = 1.; check[2] = 0.;
	check[3] = 1.; check[4] = 2.; check[5] = 0.;
	check[6] = 0.; check[7] = 0.; check[8] = 2.;
	for(k = 0; k < n * n; k++)
		ok &=  NearEqual(check[k], h[k], eps, eps );

	// --------------------------------------------------------------------
	// example using vectors of bools to compute sparsity pattern for Hessian
	b_vector r_bool(n * n);
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
			r_bool[i * n + j] = false;
		r_bool[i * n + i] = true;
	}
	f.ForSparseJac(n, r_bool);
	//
	b_vector s_bool(m);
	for(i = 0; i < m; i++)
		s_bool[i] = w[i] != 0;
	b_vector p_bool = f.RevSparseHes(n, s_bool);

	// example passing sparsity pattern to SparseHessian
	h = f.SparseHessian(x, w, p_bool);
	for(ell = 0; ell < n * n; ell++)
		ok &=  NearEqual(check[ell], h[ell], eps, eps );

	// --------------------------------------------------------------------
	// example using vectors of sets to compute sparsity pattern for Hessian
	s_vector r_set(n);
	for(i = 0; i < n; i++)
		r_set[i].insert(i);
	f.ForSparseJac(n, r_set);
	//
	s_vector s_set(m);
	for(i = 0; i < m; i++)
		if( w[i] != 0. )
			s_set[0].insert(i);
	s_vector p_set = f.RevSparseHes(n, s_set);

	// example passing sparsity pattern to SparseHessian
	h = f.SparseHessian(x, w, p_set);
	for(ell = 0; ell < n * n; ell++)
		ok &=  NearEqual(check[ell], h[ell], 1e-10, 1e-10 );

	// --------------------------------------------------------------------
	// use row and column indices to specify upper triangle of Hessian
	size_t K = n * (n + 1) / 2.0;
	i_vector r(K), c(K);
	d_vector hes(K);
	k = 0;
	for(i = 0; i < n; i++)
	{	for(j = i; j < n; j++)
		{	r[k] = i;
			c[k] = j;
			k++;
		}
	}
	ok &= k == K;
	CppAD::sparse_hessian_work work;

	// can use p_set or p_bool.
	f.SparseHessian(x, w, p_set, r, c, hes, work);
	for(k = 0; k < K; k++)
	{	ell = r[k] * n + c[k];
		ok &=  NearEqual(check[ell], hes[k], eps, eps );
	}

	return ok;
}
// END PROGRAM
