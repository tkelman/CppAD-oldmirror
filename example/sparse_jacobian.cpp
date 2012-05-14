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
$begin sparse_jacobian.cpp$$
$spell
	Cpp
	Jacobian
$$

$section Sparse Jacobian: Example and Test$$

$index Jacobian, sparse$$
$index example, sparse Jacobian$$
$index test, sparse Jacobian$$
$index spare, Jacobian example$$

$code
$verbatim%example/sparse_jacobian.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
namespace { // ---------------------------------------------------------
bool reverse()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	typedef CPPAD_TEST_VECTOR< AD<double> > a_vector;
	typedef CPPAD_TEST_VECTOR<double>       d_vector;
	typedef CPPAD_TEST_VECTOR<size_t>       i_vector;
	size_t i, j, k, ell;
	double eps = 10. * CppAD::epsilon<double>();

	// domain space vector
	size_t n = 4;
	a_vector  a_x(n);
	for(j = 0; j < n; j++)
		a_x[j] = AD<double> (0);

	// declare independent variables and starting recording
	CppAD::Independent(a_x);

	size_t m = 3;
	a_vector  a_y(m);
	a_y[0] = a_x[0] + a_x[1];
	a_y[1] = a_x[2] + a_x[3];
	a_y[2] = a_x[0] + a_x[1] + a_x[2] + a_x[3] * a_x[3] / 2.;

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(a_x, a_y);

	// new value for the independent variable vector
	d_vector x(n);
	for(j = 0; j < n; j++)
		x[j] = double(j);

	// Jacobian of y without sparsity pattern
	d_vector jac(m * n);
	jac = f.SparseJacobian(x);
	/*
	      [ 1 1 0 0  ]
	jac = [ 0 0 1 1  ]
	      [ 1 1 1 x_3]
	*/
	d_vector check(m * n);
	check[0] = 1.; check[1] = 1.; check[2]  = 0.; check[3]  = 0.;
	check[4] = 0.; check[5] = 0.; check[6]  = 1.; check[7]  = 1.;
	check[8] = 1.; check[9] = 1.; check[10] = 1.; check[11] = x[3];
	for(ell = 0; ell < check.size(); ell++)
		ok &=  NearEqual(check[ell], jac[ell], eps, eps );

	// using packed boolean sparsity patterns
	CppAD::vectorBool s_b(m * m), p_b(m * n);
	for(i = 0; i < m; i++)
	{	for(ell = 0; ell < m; ell++)
			s_b[i * m + ell] = false;
		s_b[i * m + i] = true;
	}
	p_b   = f.RevSparseJac(m, s_b);
	jac   = f.SparseJacobian(x, p_b);
	for(ell = 0; ell < check.size(); ell++)
		ok &=  NearEqual(check[ell], jac[ell], eps, eps );

	// using vector of sets sparsity patterns
	std::vector< std::set<size_t> > s_s(m),  p_s(m);
	for(i = 0; i < m; i++)
		s_s[i].insert(i);
	p_s   = f.RevSparseJac(m, s_s);
	jac   = f.SparseJacobian(x, p_s);
	for(ell = 0; ell < check.size(); ell++)
		ok &=  NearEqual(check[ell], jac[ell], eps, eps );

	// using row and column indices to compute non-zero in second and
	// third row. Use column major order (reverse mode) because there are
	// more rows than columns.
	size_t K = 6;
	i_vector r(K), c(K);
	jac.resize(K);
	k = 0;
	for(i = 1; i < m; i++)
	{	for(j = 0; j < n; j++)
		{	ell = i * n + j;
			if( p_b[ell] )
			{	ok &= check[ell] != 0.;
				r[k] = i;
				c[k] = j;
				k++;
			}
		}
	} 
	ok &= k == K;
	// could use p_b 
	f.SparseJacobian(x, p_b, r, c, jac);
	for(k = 0; k < K; k++)
	{    ell = r[k] * n + c[k];
		ok &= NearEqual(check[ell], jac[k], eps, eps);
	}

	return ok;
}

bool forward()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	typedef CPPAD_TEST_VECTOR< AD<double> > a_vector;
	typedef CPPAD_TEST_VECTOR<double>       d_vector;
	size_t j, ell;
	double eps = 10. * CppAD::epsilon<double>();

	// domain space vector
	size_t n = 3;
	a_vector  a_x(n);
	for(j = 0; j < n; j++)
		a_x[j] = AD<double> (0);

	// declare independent variables and starting recording
	CppAD::Independent(a_x);

	size_t m = 4;
	a_vector  a_y(m);
	a_y[0] = a_x[0] + a_x[2];
	a_y[1] = a_x[0] + a_x[2];
	a_y[2] = a_x[1] + a_x[2];
	a_y[3] = a_x[1] + a_x[2] * a_x[2] / 2.;

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(a_x, a_y);

	// new value for the independent variable vector
	d_vector x(n);
	for(j = 0; j < n; j++)
		x[j] = double(j);

	// Jacobian of y without sparsity pattern
	d_vector jac(m * n);
	jac = f.SparseJacobian(x);
	/*
	      [ 1 0 1   ]
	jac = [ 1 0 1   ]
	      [ 0 1 1   ]
	      [ 0 1 x_2 ]
	*/
	d_vector check(m * n);
	check[0] = 1.; check[1]  = 0.; check[2]  = 1.; 
	check[3] = 1.; check[4]  = 0.; check[5]  = 1.;
	check[6] = 0.; check[7]  = 1.; check[8]  = 1.; 
	check[9] = 0.; check[10] = 1.; check[11] = x[2];
	for(ell = 0; ell < check.size(); ell++)
		ok &=  NearEqual(check[ell], jac[ell], eps, eps );

	// test using packed boolean vectors for sparsity pattern
	CppAD::vectorBool r_b(n * n), p_b(m * n);
	for(j = 0; j < n; j++)
	{	for(ell = 0; ell < n; ell++)
			r_b[j * n + ell] = false;
		r_b[j * n + j] = true;
	}
	p_b = f.ForSparseJac(n, r_b);
	jac = f.SparseJacobian(x, p_b);
	for(ell = 0; ell < check.size(); ell++)
		ok &=  NearEqual(check[ell], jac[ell], eps, eps );

	// test using vector of sets for sparsity pattern
	std::vector< std::set<size_t> > r_s(n), p_s(m);
	for(j = 0; j < n; j++)
		r_s[j].insert(j);
	p_s = f.ForSparseJac(n, r_s);
	jac = f.SparseJacobian(x, p_s);
	for(ell = 0; ell < check.size(); ell++)
		ok &=  NearEqual(check[ell], jac[ell], eps, eps );

	return ok;
}
} // End empty namespace 

bool sparse_jacobian(void)
{	bool ok = true;
	ok &= forward();
	ok &= reverse();

	return ok;
}
// END PROGRAM
