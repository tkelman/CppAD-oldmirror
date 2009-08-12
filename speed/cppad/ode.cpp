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
$begin cppad_ode.cpp$$
$spell
	endif
	var
	Jacobian
	std
	cout
	endl
	CppAD
	cppad
	hpp
	bool
	cstring
	retape
	typedef
	cassert
$$

$section CppAD Speed: Gradient of Ode Solution$$

$index cppad, speed ode jacobian$$
$index speed, cppad ode jacobian$$
$index jacobian, ode speed cppad$$
$index ode, jacobian speed cppad$$

$head link_ode$$
$index link_ode$$
$codep */
# include <cstring>
# include <cppad/cppad.hpp>
# include <cppad/speed/ode_evaluate.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cassert>

bool link_ode(
	size_t                     size       ,
	size_t                     repeat     ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<double>      &jacobian
)
{	// -------------------------------------------------------------
	// setup
	typedef CppAD::AD<double>       ADScalar;
	typedef CppAD::vector<ADScalar> ADVector;
	typedef CppAD::vector<double>   DblVector;

	size_t j;
	size_t m = 0;
	size_t n = size;
	assert( x.size() == n );
	assert( jacobian.size() == n * n );

	ADVector  X(n);
	ADVector  Y(n);

	using std::cout;
	using std::endl;
	static bool print = true;
	while(repeat--)
	{ 	// choose next x value
		uniform_01(n, x);
		for(j = 0; j < n; j++)
			X[j] = x[j];

		// declare the independent variable vector
		Independent(X);

		// evaluate function
		CppAD::ode_evaluate(X, m, Y);

		// create function object f : X -> Y
		CppAD::ADFun<double>   F;
		F.Dependent(X, Y);
		extern bool global_optimize;
		if( global_optimize & (repeat > 1) & (n >= 3) & print ) 
			cout << "before optimize: F.size_var() = " 
		             << F.size_var() << endl; 
		if( global_optimize )
			F.optimize();
		jacobian = F.Jacobian(x);
		if( global_optimize & (repeat > 1) & (n >= 3) & print ) 
		{
			cout << "after optimize:  F.size_var() = "
			     << F.size_var() << endl; 
			print = false;
		}
	}
	return true;
}
/* $$
$end
*/
