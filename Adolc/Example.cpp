/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */

/*
$begin ExampleAdolc.cpp$$
$spell
	Adolc
	Cpp
$$

$index Adolc, compare$$
$section Compare Adolc and CppAD Results$$

$comment This file is in the Adolc subdirectory$$ 
$code
$verbatim%Adolc/Example.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

namespace {
	// function that runs one test
	bool Run(bool TestOk(void), const char *name)
	{	bool ok = true;
		using namespace std;

		ok &= TestOk();

		if( ok )
			std::cout << "Ok:    " << name << std::endl;
		else	std::cout << "Error: " << name << std::endl;

		return ok;
	}
}

// various test routines
extern bool AllocMatTest(void);
extern bool AllocVecTest(void);
extern bool DetMinor(void);
extern bool DetLu(void);

// main program that runs all the tests
int main(void)
{	bool ok = true;
	using namespace std;

	ok &= Run(AllocMatTest,      "AllocMat"   );
	ok &= Run(AllocVecTest,      "AllocVec"   );
	ok &= Run(DetMinor,          "DetMinor"   );
	ok &= Run(DetLu,             "DetLu"      );
	
	if( ok )
		cout << "All the tests passed." << endl;
	else	cout << "At least one test failed." << endl;

	return static_cast<int>( ! ok );
}

// END PROGRAM
