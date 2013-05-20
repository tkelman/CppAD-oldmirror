/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// system include files used for I/O
# include <iostream>

// C style asserts
# include <cassert>

// ipopt solve include file
# include <cppad/ipopt/solve.hpp>

// external complied tests
extern bool checkpoint(void);
extern bool old_reciprocal(void);
extern bool reciprocal(void);
extern bool old_mat_mul(void);
extern bool old_tan(void);
extern bool old_usead_1(void);
extern bool old_usead_2(void);

namespace {
	// function that runs one test
	static size_t Run_ok_count    = 0;
	static size_t Run_error_count = 0;
	bool Run(bool TestOk(void), const char *name)
	{	bool ok = true;
		ok &= TestOk();
		if( ok )
		{	std::cout << "OK:    " << "atomic: " << name << std::endl;
			Run_ok_count++;
		}
		else
		{	std::cout << "Error: " << "atomic: " << name << std::endl;
			Run_error_count++;
		}
		return ok;
	}
}

// main program that runs all the tests
int main(void)
{	bool ok = true;

	// external compiled tests
	ok &= Run( checkpoint,          "checkpoint"     );
	ok &= Run( old_reciprocal,      "old_reciprocal" );
	ok &= Run( reciprocal,          "reciprocal"     );
	ok &= Run( old_mat_mul,        "old_mat_mul"   );
	ok &= Run( old_tan,            "old_tan"       );
	ok &= Run( old_usead_1,        "old_usead_1"   );
	ok &= Run( old_usead_2,        "old_usead_2"   );
	
	// check for errors
	using std::cout;
	using std::endl;
	assert( ok || (Run_error_count > 0) );
	if( CppAD::thread_alloc::free_all() )
	{	Run_ok_count++;
		cout << "OK:    " << "No memory leak detected" << endl;
	}
	else
	{	ok = false;
		Run_error_count++;
		cout << "Error: " << "memory leak detected" << endl;
	}
	// convert int(size_t) to avoid warning on _MSC_VER systems
	if( ok )
		cout << "All " << int(Run_ok_count) << " tests passed." << endl;
	else	cout << int(Run_error_count) << " tests failed." << endl;

	return static_cast<int>( ! ok );
}
