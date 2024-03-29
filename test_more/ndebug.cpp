/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Tests that that expect NDEBUG to be defined.
*/

# include <cppad/cppad.hpp>

namespace {
	// make sure that this function can be compiled
	bool ad_fun_copy(void)
	{	bool                ok = true;
		CppAD::ADFun<double> f;

		// This operation uses the ADFun copy constructor which is defined,
		// but should not be used
		CppAD::ADFun<double> g(f);

		ok &= (g.size_var() == 0);

		return ok;
	}
}


bool ndebug(void)
{	bool ok = true;


	return ok;
}
