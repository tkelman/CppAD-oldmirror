/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin multi_thread.cpp$$
$spell
	Cpp
$$

$section Multi-Threading Examples and Correctness Tests$$
$index multi_thread, example$$
$index example, multi_thread$$

$head Running Tests$$
To build this program and run its correctness tests,
execute the following commands starting in the
$cref/work directory/InstallUnix/Download/Work Directory/$$:
$codei%
	cd multi_thread
	make test
%$$

$childtable%
	multi_thread/openmp/openmp.cpp%

	multi_thread/bthread_a11c.cpp%
	multi_thread/bthread_ad.cpp%

	multi_thread/pthread_a11c.cpp%
	multi_thread/pthread_ad.cpp
%$$


$code
$verbatim%multi_thread/multi_thread.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

// system include files used for I/O
# include <iostream>

// C style asserts
# include <cassert>

// standard string
# include <string>

// memory leak utility
# include <cppad/memory_leak.hpp>

// external complied tests
extern bool bthread_a11c(void);
extern bool bthread_ad(void);
extern bool pthread_a11c(void);
extern bool pthread_ad(void);

namespace {
	// function that runs one test
	static size_t Run_ok_count    = 0;
	static size_t Run_error_count = 0;
	bool Run(bool TestOk(void), std::string name)
	{	bool ok      = true;
		std::streamsize width =  20;         
		std::cout.width( width );
		std::cout.setf( std::ios_base::left );
		std::cout << name;
		//
		ok &= name.size() < size_t(width);
		ok &= TestOk();
		if( ok )
		{	std::cout << "OK" << std::endl;
			Run_ok_count++;
		}
		else
		{	std::cout << "Error" << std::endl;
			Run_error_count++;
		}
		return ok;
	}
}

// main program that runs all the tests
int main(void)
{	bool ok = true;

	// external compiled tests
# ifdef CPPAD_BTHREAD_EXAMPLES
	ok &= Run( bthread_a11c,      "bthread_a11c"     );
	ok &= Run( bthread_ad,        "bthread_ad"       );
# endif
# ifdef CPPAD_PTHREAD_EXAMPLES
	ok &= Run( pthread_a11c,      "pthread_a11c"     );
	ok &= Run( pthread_ad,        "pthread_ad"     );
# endif

	// check for errors
	using std::cout;
	using std::endl;
	assert( ok || (Run_error_count > 0) );
	if( CppAD::memory_leak() )
	{	ok = false;
		Run_error_count++;
		cout << "Error: " << "memory leak detected" << endl;
	}
	else
	{	Run_ok_count++;
		cout << "OK:    " << "No memory leak detected" << endl;
	}
	// convert int(size_t) to avoid warning on _MSC_VER systems
	if( ok )
		cout << "All " << int(Run_ok_count) << " tests passed." << endl;
	else	cout << int(Run_error_count) << " tests failed." << endl;

	return static_cast<int>( ! ok );
}
// END PROGRAM
