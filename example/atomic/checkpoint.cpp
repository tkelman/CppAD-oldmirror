/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin checkpoint.cpp$$
$spell
	checkpointing
	Taylor
$$

$section Simple Checkpointing: Example and Test$$

$index checkpoint, example$$
$index example, checkpoint$$
$index test, checkpoint$$

$head Purpose$$
Break a large computation into pieces and only store values at the 
interface of the pieces.
In actual applications, there may be many functions, but 
for this example there are only two.
The functions 
$latex F : \B{R}^2 \rightarrow \B{R}^2$$ 
and
$latex G : \B{R}^2 \rightarrow \B{R}^2$$ 
defined by
$latex \[
	F(x) = \left( \begin{array}{c} 
		x_0 \cdot x_0 \cdot x_0 \cdot x_0 \cdot x_0
		\\
		x_1 \cdot x_1 \cdot x_1 \cdot x_1 \cdot x_1
	\end{array} \right) 
	\; , \;
	G(y) = \left( \begin{array}{c} 
		y_0 + y_0 + y_0 
		\\ 
		y_1 + y_1 + y_1 
	\end{array} \right) 
\] $$

$code
$verbatim%example/atomic/checkpoint.cpp%0%// BEGIN C++%// END C++%1%$$
$$


$end
*/
// BEGIN C++

# include <cppad/cppad.hpp>

namespace {
	using CppAD::AD;
	typedef CPPAD_TESTVECTOR(AD<double>) ADVector;

	void f_algo(const ADVector& x, ADVector& y)
	{	y[0] = 1.0;
		y[1] = 1.0;
		for(size_t k = 0; k < 5; k++)
		{	y[0] *= x[0];
			y[1] *= x[1];
		}
		return;
	}
	void g_algo(const ADVector& y, ADVector& z)
	{	z[0] = 0.0;
		z[1] = 0.0;
		for(size_t k = 0; k < 3; k++)
		{	z[0] += y[0];
			z[1] += y[1];
		}
		return;
	}
}

bool checkpoint(void)
{	bool ok = true;
	using CppAD::checkpoint;
	using CppAD::ADFun;
	using CppAD::NearEqual;
	size_t i, j, k, n = 2, m = n;
	double eps = 10. * std::numeric_limits<double>::epsilon();

	// checkpoint version of the function F(x)
	ADVector ax(n), ay(n), az(m);
	for(j = 0; j < n; j++)
		ax[j] = double(j);
	checkpoint<double> f_check("f_check", f_algo, ax, ay); 
	checkpoint<double> g_check("g_check", g_algo, ay, az); 

	// Record a version of z = g[f(x)] without checkpointing
	Independent(ax);
	f_algo(ax, ay);
	g_algo(ay, az);
	ADFun<double> check_not(ax, az);

	// Record a version of z = g[f(x)] with checkpointing
	Independent(ax);
	f_check(ax, ay);
	g_check(ay, az);
	ADFun<double> check_yes(ax, az);

	// checkpointing should use fewer operations
	ok &= check_yes.size_var() < check_not.size_var();

	// compare forward mode results for orders 0, 1, 2
	size_t p = 2;
	CPPAD_TESTVECTOR(double) x_p(n*(p+1)), z_not(m*(p+1)), z_yes(m*(p+1));
	for(j = 0; j < n; j++)
	{	for(k = 0; k <= p; k++)
			x_p[ j * (p+1) + k ] = 1.0 / (p + 1 - k);
	}
	z_not = check_not.Forward(p, x_p);
	z_yes = check_yes.Forward(p, x_p);
	for(i = 0; i < m; i++)
	{	for(k = 0; k <= p; k++)
		{	double zik_not = z_not[ i * (p+1) + k];
			double zik_yes = z_yes[ i * (p+1) + k];
			ok &= NearEqual(zik_not, zik_yes, eps, eps);
		}
	}

	// compare reverse mode results
	CPPAD_TESTVECTOR(double) w(m*(p+1)), dw_not(n*(p+1)), dw_yes(n*(p+1));
	dw_not = check_not.Reverse(p+1, w);
	dw_yes = check_yes.Reverse(p+1, w);
	for(j = 0; j < n; j++)
	{	for(k = 0; k <= p; k++)
		{	double dwjk_not = dw_not[ j * (p+1) + k];
			double dwjk_yes = dw_yes[ j * (p+1) + k];
			ok &= NearEqual(dwjk_not, dwjk_yes, eps, eps);
		}
	}

	// mix sparsity so test both cases
	f_check.option( CppAD::atomic_base<double>::bool_sparsity_enum );
	g_check.option( CppAD::atomic_base<double>::set_sparsity_enum );

	// compare forward mode Jacobian sparsity patterns
	CppAD::vector< std::set<size_t> > r(n), s_not(m), s_yes(m);
	for(j = 0; j < n; j++)
		r[j].insert(j);
	s_not = check_not.ForSparseJac(n, r);
	s_yes = check_yes.ForSparseJac(n, r);
	for(i = 0; i < m; i++)
		ok &= s_not[i] == s_yes[i];

	// compare reverse mode Jacobian sparsity patterns
	CppAD::vector< std::set<size_t> > s(m), r_not(m), r_yes(m);
	for(i = 0; i < m; i++)
		s[i].insert(i);
	r_not = check_not.RevSparseJac(m, s);
	r_yes = check_yes.RevSparseJac(m, s);
	for(i = 0; i < m; i++)
		ok &= s_not[i] == s_yes[i];
	

	// compare reverse mode Hessian sparsity patterns
	CppAD::vector< std::set<size_t> > s_one(1), h_not(n), h_yes(n); 
	for(i = 0; i < m; i++)
		s_one[0].insert(i);
	h_not = check_not.RevSparseHes(n, s_one);
	h_yes = check_yes.RevSparseHes(n, s_one);
	for(i = 0; i < n; i++)
		ok &= h_not[i] == h_yes[i];
	
	return ok;
}
// END C++
