/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin exp_eps.cpp$$
$spell
	exp_eps
$$

$section exp_eps: Example and Test$$

$index exp_eps, introduction$$
$index introduction, exp_eps$$

$code
$verbatim%introduction/exp_apx/exp_eps.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cmath>             // for fabs function
# include "exp_eps.hpp"       // definition of exp_eps algorithm
bool exp_eps(void)
{	double x     = .5;
	double e     = .2;
	double check = 1 + .5 + .125; // include 1 and only 1 term less than e
	bool   ok    = std::fabs( exp_eps(x, e) - check ) <= 1e-10; 
	return ok;
}
// END PROGRAM
