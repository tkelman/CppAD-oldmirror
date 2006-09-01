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
$begin Speed.cpp$$
$spell
	Cpp
$$

$mindex run all speed test$$
$section Run All The Speed Tests$$

$comment This file is in the Speed subdirectory$$ 
$code
$verbatim%Speed/Speed.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

# include <cstddef>
# include <iostream>
# include <complex>

// For an unknown reason, cannot move other includes (using Sun's CC compiler)

typedef std::complex<double> Complex;

std::string doubleDetMinor    (size_t size, size_t repeat);
std::string ADdoubleDetMinor  (size_t size, size_t repeat);
std::string TapeDetMinor      (size_t size, size_t repeat);
std::string JacDetMinor       (size_t size, size_t repeat);
std::string HesDetMinor       (size_t size, size_t repeat);

std::string doubleDetLu       (size_t size, size_t repeat);
std::string ADdoubleDetLu     (size_t size, size_t repeat);
std::string TapeDetLu         (size_t size, size_t repeat);
std::string JacDetLu          (size_t size, size_t repeat);
std::string HesDetLu          (size_t size, size_t repeat);

std::string doubleOde         (size_t size, size_t repeat);
std::string ADdoubleOde       (size_t size, size_t repeat);
std::string TapeOde           (size_t size, size_t repeat);
std::string JacOde            (size_t size, size_t repeat);
std::string HesOde            (size_t size, size_t repeat);

std::string LuSolve           (size_t size, size_t repeat);
std::string LuVecAD           (size_t size, size_t repeat);

int main()
{	using CppAD::SpeedTest;
	std::cout << PACKAGE_STRING << " Speed Test " << std::endl;

	SpeedTest(doubleDetMinor,        5, -1, 1);
	SpeedTest(ADdoubleDetMinor,      5, -1, 1);
	SpeedTest(TapeDetMinor,          5, -1, 1);
	SpeedTest(JacDetMinor,           5, -1, 1);
	SpeedTest(HesDetMinor,           5, -1, 1);

	SpeedTest(doubleDetLu,           9, -2, 1);
	SpeedTest(ADdoubleDetLu,         9, -2, 1);
	SpeedTest(TapeDetLu,             9, -2, 1);
	SpeedTest(JacDetLu,              9, -2, 1);
	SpeedTest(HesDetLu,              9, -2, 1);

	SpeedTest(doubleOde,             5, -1, 1);
	SpeedTest(ADdoubleOde,           5, -1, 1);
	SpeedTest(TapeOde,               5, -1, 1);
	SpeedTest(JacOde,                5, -1, 1);
	SpeedTest(HesOde,                5, -1, 1);

	SpeedTest(LuSolve,               5, -1, 1);
	SpeedTest(LuVecAD,               5, -1, 1);

	return 0;
}

// END PROGRAM
