/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin allocator.cpp$$

$section Allocator: Example and Test$$

$index allocator$$

$code
$verbatim%example/allocator.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/allocator.hpp>
# include <vector>

bool allocator(void)
{	bool ok = true;
	typedef CppAD::Allocator<double>       Allocator;
	typedef std::vector<double, Allocator> Vector;

	Vector v;
	size_t n = 5;
	v.resize(n);
	v[0] = 0.;
	size_t i;
	double sum = 0.;
	for(i = 1; i < n; i++)
	{	v[i] = i + v[i-1];
		sum += double(i);
		ok  &= v[i] == sum;
	}

	return ok;
}

// END PROGRAM
