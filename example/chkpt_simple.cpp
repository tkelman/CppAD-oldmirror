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
$begin chkpt_simple.cpp$$
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
	F(x) = \left( \begin{array}{c} x_0 x_1   \\ x_1 - x_0 \end{array} \right) 
	\; , \;
	G(y) = \left( \begin{array}{c} y_0 - y_1 \\ y_1  y_0   \end{array} \right) 
\] $$

$code
$verbatim%example/chkpt_simple.cpp%0%// BEGIN C++%// END C++%1%$$
$$


$end
*/
// BEGIN C++

# include <cppad/cppad.hpp>

namespace {
	using CppAD::AD;
	typedef CPPAD_TESTVECTOR(AD<double>) ADVector;

	bool f_algo(const ADVector& x, ADVector& y)
	{	y[0] = 1.0;
		y[1] = 1.0;
		for(size_t k = 0; k < 5; k++)
		{	y[0] *= x[0];
			y[1] *= x[1];
		}
		return true;
	}
	bool g_algo(const ADVector& y, ADVector& z)
	{	z[0] = 0.0;
		z[1] = 0.0;
		for(size_t k = 0; k < 3; k++)
		{	z[0] += y[0];
			z[1] += y[1];
		}
		return true;
	}
}

bool chkpt_simple(void)
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
	ADFun<double> no_check(ax, az);

	// Record a version of z = g[f(x)] with checkpointing
	Independent(ax);
	f_check(ax, ay);
	g_check(ay, az);
	ADFun<double> yes_check(ax, az);

	// checkpointing should use fewer operations
	ok &= yes_check.size_var() < no_check.size_var();

	// compare forward mode results for orders 0, 1, 2
	size_t p = 2;
	CPPAD_TESTVECTOR(double) x_p(n*(p+1)), z_no(m*(p+1)), z_yes(m*(p+1));
	z_no  = no_check.Forward(p, x_p);
	z_yes = yes_check.Forward(p, x_p);
	for(i = 0; i < m; i++)
	{	for(k = 0; k <= p; k++)
		{	double zik_no  = z_no[ i * (p+1) + k];
			double zik_yes = z_yes[ i * (p+1) + k];
			ok &= NearEqual(zik_no, zik_yes, eps, eps);
		}
	}

	
	checkpoint<double>::clear();
	return ok;
}
// END C++
