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
$begin mul_level_adolc.cpp$$
$spell
	CppAD
	AdolcDir
	adouble
	Vec
$$

$section Using Adolc with Multiple Levels of Taping: Example and Test$$
$index multiple, Adolc$$
$index level, multiple Adolc$$
$index Adolc, multiple level$$

$head Purpose$$
In this example, we use $code AD< adouble> >$$ (level two taping),
the compute values of the function $latex f : \B{R}^n \rightarrow \B{R}$$ where 
$latex \[
	f(x) = \frac{1}{2} \left( x_0^2 + \cdots + x_{n-1}^2 \right)
\] $$
We then use Adolc's $code adouble$$ (level one taping) to compute
the directional derivative
$latex \[
f^{(1)} (x) * v  = x_0 v_0 + \cdots + x_{n-1} v_{n-1}
\] $$.
where $latex v \in \B{R}^n$$.
We then use $code double$$ (no taping) to compute
$latex \[
\frac{d}{dx} \left[ f^{(1)} (x) * v \right] = v
\] $$
This is only meant as an example of multiple levels of taping.
The example $cref HesTimesDir.cpp$$ computes the same value more
efficiently by using the identity:
$latex \[
	\frac{d}{dx} \left[ f^{(1)} (x) * v \right] = f^{(2)} (x) * v
\] $$
The example $cref mul_level.cpp$$ computes the same values using
$code AD< AD<double> >$$ and $code AD<double>$$.

$head Memory Management$$
Adolc uses raw memory arrays that depend on the number of 
dependent and independent variables.
The memory management utility $cref omp_alloc$$ 
is used to manage this memory allocation.

$head Configuration Requirement$$
This example will be compiled and tested provided that
the value $cref/adolc_dir/InstallUnix/adolc_dir/$$ is specified on the 
$cref/configure/InstallUnix/Configure/$$ command line.

$code
$verbatim%example/mul_level_adolc.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <adolc/adouble.h>
# include <adolc/taping.h>
# include <adolc/interfaces.h>

// adouble definitions not in Adolc distribution and 
// required in order to use CppAD::AD<adouble>
# include <cppad/example/base_adolc.hpp>

# include <cppad/cppad.hpp>

namespace { 
	// f(x) = |x|^2 / 2 = .5 * ( x[0]^2 + ... + x[n-1]^2 )
	template <class Type>
	Type f(CPPAD_TEST_VECTOR(Type) &x)
	{	Type sum;

		sum  = 0.;
		size_t i = x.size();
		for(i = 0; i < x.size(); i++)
			sum += x[i] * x[i];

		return .5 * sum;
	} 
}

bool mul_level_adolc(void) 
{	bool ok = true;                // initialize test result
	using CppAD::omp_alloc;        // The CppAD memory allocator

	typedef adouble             ADdouble;  // for first level of taping
	typedef CppAD::AD<ADdouble> ADDdouble; // for second level of taping
	size_t n = 5;                          // number independent variables
	size_t j;

	CPPAD_TEST_VECTOR(double)       x(n);
	CPPAD_TEST_VECTOR(ADdouble)   a_x(n);
	CPPAD_TEST_VECTOR(ADDdouble) aa_x(n);

	// Values for the independent variables while taping the function f(x)
	for(j = 0; j < n; j++)
		aa_x[j] = double(j);
	// Declare the independent variable for taping f(x)
	CppAD::Independent(aa_x); 

	// Use AD<adouble> to tape the evaluation of f(x)
	CPPAD_TEST_VECTOR(ADDdouble) aa_f(1); 
	aa_f[0] = f(aa_x); 

	// Declare a_F as the corresponding ADFun<adouble> function f(x)
	// (make sure we do not run zero order forward during constructor)
	CppAD::ADFun<ADdouble> a_F;
	a_F.Dependent(aa_x, aa_f);

	// Value of the independent variables whitle taping f'(x) * v
	int tag = 0; 
	int keep = 1;
	trace_on(tag, keep);
	for(j = 0; j < n; j++)
		a_x[j] <<= double(j);

	// set the argument value x for computing f'(x) * v
	a_F.Forward(0, a_x);

	// compute f'(x) * v
	CPPAD_TEST_VECTOR(ADdouble) a_v(n);
	CPPAD_TEST_VECTOR(ADdouble) a_df(1);
	for(j = 0; j < n; j++)
		a_v[j] = double(n - j);
	a_df = a_F.Forward(1, a_v); 

	// declare Adolc function corresponding to f'(x) * v
	double df; 
	a_df[0] >>= df;
	trace_off();

	// compute the d/dx of f'(x) * v = f''(x) * v
	size_t m      = 1;                     // # dependent in f'(x) * v

	// w = new double[capacity] where capacity >= m
	size_t capacity;
	double* w  = omp_alloc::create_array<double>(m, capacity);
	// w = new double[capacity] where capacity >= n
	double* dw = omp_alloc::create_array<double>(n, capacity);

	w[0]  = 1.;
	fos_reverse(tag, int(m), int(n), w, dw);

	for(j = 0; j < n; j++)
	{	double vj = a_v[j].value();
		ok &= CppAD::NearEqual(dw[j], vj, 1e-10, 1e-10);
	}

	// make memory avaialble for other use by this thread
	omp_alloc::delete_array(w);
	omp_alloc::delete_array(dw);
	return ok;
}
// END C++
