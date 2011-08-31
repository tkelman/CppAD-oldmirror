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
$begin openmp_run.cpp$$
$spell
	num
	openmp
$$
$index OpenMP, run speed$$
$index run, OpenMP speed$$
$index speed, OpenMP run$$


$section Run Speed Test of OpenMP Newton Example$$

$head Syntax$$
$codei%run% newton_example %max_threads% %n_zero% %n_sub% %n_sum% %use_ad%$$ 

$head Purpose$$
Runs a timing tests of the $cref openmp_newton_example.cpp$$ routine.

$head max_threads$$
If the argument $icode max_threads$$ is a non-negative integer specifying
the maximum number of OpenMP threads to use for the test.
The routine $code newton_example$$ will be run for each value of
$cref/num_threads/openmp_newton_example.cpp/num_threads/$$ from
zero to $icode max_threads$$.
The value of zero corresponds to not using OpenMP
(OpenMP is used for all other values).

$head n_zero$$
The command line argument $icode n_zero$$ 
is a integer greater than or equal two and has the same meaning as
$cref/n_zero/openmp_newton_example.cpp/n_zero/$$.

$head n_sub$$
The command line argument $icode n_sub$$ 
is an integer greater than or equal one and has the same meaning as
$cref/n_sub/openmp_newton_example.cpp/n_sub/$$.


$head n_sum$$
The command line argument $icode n_sum$$ 
is an integer greater than or equal one and has the same meaning as
$cref/n_sum/openmp_newton_example.cpp/n_sum/$$.

$head use_ad$$
The command line argument $icode use_ad$$ is either 
$code true$$ or $code false$$ and has the same meaning as
$cref/use_ad/openmp_newton_example.cpp/use_ad/$$.

$head Subroutines$$
$children%
	openmp/newton_example.cpp
%$$
$table
$rref openmp_newton_example.cpp$$
$tend

$head Source$$
$code
$verbatim%openmp/run.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cmath>
# include <cstring>
# include "newton_example.hpp"

namespace {
	size_t arg2size_t(
		const char* arg       , 
		int limit             , 
		const char* error_msg )
	{	int i = std::atoi(arg);
		if( i >= limit )
			return size_t(i);
		std::cerr << "value = " << i << std::endl;
		std::cerr << error_msg << std::endl;
		exit(1);
	}
}

int main(int argc, char *argv[])
{	size_t num_fail = 0;
	using std::cout;

	const char *usage = 
	"usage: ./run max_threads n_zero n_sub n_sum use_ad";
	if( argc != 6 )
	{	std::cerr << "argc = " << argc << std::endl;	
		std::cerr << usage << std::endl;
		exit(1);
	}
	argv++;

	// max_threads 
	size_t max_threads = arg2size_t( *argv++, 0, 
		"run: max_threads is less than zero"
	);

	// n_zero
	size_t n_zero = arg2size_t( *argv++, 2,
		"run: n_zero is less than two"
	);

	// n_sub
	size_t n_sub = arg2size_t( *argv++, 1,
		"run: n_sub is less than one"
	);
       
	// n_sum 
	size_t n_sum = arg2size_t( *argv++, 1,
		"run: n_sum is less than one"
	);

	bool use_ad;
	if( strcmp(*argv, "true") == 0 )
		use_ad = true;
	else if( strcmp(*argv, "false") == 0 )
		use_ad = false;
	else
	{	std::cerr << "run: use_ad is not true or false" << std::endl;
		exit(1);
	}

	// run the test for each number of threads
	CppAD::vector<size_t> rate_all(max_threads + 1);
	size_t num_threads;
	for(num_threads = 0; num_threads <= max_threads; num_threads++)
	{	bool ok = newton_example(
			rate_all[num_threads] ,
			num_threads           ,
			n_zero                ,
			n_sub                 ,
			n_sum                 ,
			use_ad
		);
		if( ok )
			cout << "OK:    num_threads = " << num_threads;
		else
		{	cout << "Error: num_threads = " << num_threads;
			num_fail++;
		}
		cout << ", rate = " << rate_all[num_threads] << std::endl;
	}
	cout << "rate_all = " << rate_all << std::endl;
	if( num_fail == 0 )
		cout << "All " << max_threads + 1 << " tests passed." << std::endl;
	else	cout << num_fail << " tests failed." << std::endl;
 
	return num_fail;
}

// END PROGRAM
