// BEGIN SHORT COPYRIGHT
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
-------------------------------------------------------------------------- */
// END SHORT COPYRIGHT
/*
$begin SpeedAdolc.cpp$$
$spell
	Adolc
	Cpp
$$

$mindex run all Adolc speed test$$
$section Run All The Adolc Speed Comparison Tests$$

$comment This file is in the Adolc subdirectory$$ 
$code
$verbatim%Adolc/Speed.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <cstddef>


std::string DetMinorCp  (size_t size, size_t repeat);
std::string DetMinorAd  (size_t size, size_t repeat);
std::string DetLuCp     (size_t size, size_t repeat);
std::string DetLuAd     (size_t size, size_t repeat);

int main()
{	using CppAD::SpeedTest;

	SpeedTest(DetMinorCp,    1, 2, 7);
	SpeedTest(DetMinorAd,    1, 2, 7);
	SpeedTest(DetLuCp,       1, 2, 7);
	SpeedTest(DetLuAd,       1, 2, 7);

	return 0;
}

// END PROGRAM
