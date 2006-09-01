/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
-------------------------------------------------------------------------- */
/*
$begin ExpApx.cpp$$
$spell
	ExpApx
$$

$section ExpApx: Example and Test$$

$index ExpApx, introduction$$
$index introduction, ExpApx$$

$code
$verbatim%Introduction/ExpApx.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cmath>             // for fabs function
# include "ExpApx.hpp"        // definition of ExpApx algorithm
bool ExpApx(void)
{	double x     = .5;
	double e     = .2;
	double check = 1 + .5 + .125; // include 1 and only 1 term less than e
	bool   ok    = std::fabs( ExpApx(x, e) - check ) <= 1e-10; 
	return ok;
}
// END PROGRAM
