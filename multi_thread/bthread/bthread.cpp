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
$begin bthread.cpp$$
$spell
	inv
	mega
	cpp
	num
	bthread
	bthreads
$$
$index bthread, example$$
$index bthread, speed$$
$index bthread, test$$
$index example, bthread$$
$index speed, bthread$$
$index test, bthread$$


$section Run Boost Threads Examples and Speed Tests$$

$head Syntax$$
$codei%./bthread a11c
%$$
$codei%./bthread simple_ad
%$$
$codei%./bthread sum_i_inv %max_threads% %mega_sum%
%$$ 
$codei%./bthread multi_newton %max_threads% %num_zero% %num_sub% %num_sum% %use_ad%$$ 

$head Running Tests$$
You can build this program and run the default version of its test
parameters by executing the following commands:
$codep
	cd bthread
	make test
$$

$head Purpose$$
Runs one of the following examples:
$table
$rref bthread.cpp$$
$rref simple_ad.cpp$$
$tend
or one of the following speed tests:
$table
$rref sum_i_inv_time.cpp$$
$rref multi_newton_time.cpp$$ 
$tend

$head max_threads$$
If the argument $icode max_threads$$ is a non-negative integer specifying
the maximum number of bthreads to use for the test.
The specified test is run with the following number of threads:
$codei%
	%num_threads% = 0 , %...% , %max_threads%
%$$
The value of zero corresponds to not using bthreads
(bthreads is used for all other values).

$comment -----------------------------------------------------------------$$

$head sum_i_inv$$
The following command line arguments only apply to $code sum_i_inv$$:

$subhead mega_sum$$
The command line argument $icode mega_sum$$ 
is an integer greater than or equal one and has the same meaning as in
$cref/sum_i_inv_time.cpp/sum_i_inv_time.cpp/mega_sum/$$.

$comment -----------------------------------------------------------------$$

$head multi_newton$$
The following command line arguments only apply to $code multi_newton$$:

$subhead num_zero$$
The command line argument $icode num_zero$$ 
is an integer greater than or equal two and has the same meaning as in
$cref/multi_newton_time.cpp/multi_newton_time.cpp/num_zero/$$.

$subhead num_sub$$
The command line argument $icode num_sub$$ 
is an integer greater than or equal one and has the same meaning as in
$cref/multi_newton_time.cpp/multi_newton_time.cpp/num_sub/$$.

$subhead num_sum$$
The command line argument $icode num_sum$$ 
is an integer greater than or equal one and has the same meaning as in
$cref/multi_newton_time.cpp/multi_newton_time.cpp/num_sum/$$.

$subhead use_ad$$
The command line argument $icode use_ad$$ is either 
$code true$$ or $code false$$ and has the same meaning as in
$cref/multi_newton_time.cpp/multi_newton_time.cpp/use_ad/$$.

$head Subroutines$$
$childtable%
	multi_thread/bthread/a11c.cpp%
	multi_thread/bthread/sum_i_inv.cpp%
	multi_thread/bthread/multi_newton.cpp%
	multi_thread/bthread/bthread_team.cpp
%$$

$head Source$$
$code
$verbatim%multi_thread/bthread/bthread.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cmath>
# include <cstring>
# include "../thread_team.hpp"
# include "../simple_ad.hpp"
# include "../sum_i_inv_time.hpp"
# include "../multi_newton_time.hpp"

extern bool a11c(void);

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
{	using CppAD::thread_alloc;
	size_t num_fail = 0;
	bool ok         = true;
	using std::cout;

	// commnd line usage message
	const char *usage = 
	"usage: ./bthread a11c\n"
	"       ./bthread simple_ad\n"
	"       ./bthread sum_i_inv    max_threads mega_sum\n"
	"       ./bthread multi_newton max_threads num_zero num_sub num_sum use_ad";

	// command line argument values (assign values to avoid compiler warnings)
	size_t num_zero=0, num_sub=0, num_sum=0;
	bool use_ad=true;

	ok = false;
	const char* test_name = "";
	if( argc > 1 )
		test_name = *++argv;
	bool run_a11c         = std::strcmp(test_name, "a11c")         == 0;
	bool run_simple_ad    = std::strcmp(test_name, "simple_ad")    == 0;
	bool run_sum_i_inv    = std::strcmp(test_name, "sum_i_inv")    == 0;
	bool run_multi_newton = std::strcmp(test_name, "multi_newton") == 0;
	if( run_a11c || run_simple_ad )
		ok = (argc == 2);
	else if( run_sum_i_inv )
		ok = (argc == 4);  
	else if( run_multi_newton )
		ok = (argc == 7);
	if( ! ok )
	{	std::cerr << "test_name = " << test_name << std::endl;	
		std::cerr << "argc      = " << argc      << std::endl;	
		std::cerr << usage << std::endl;
		exit(1);
	}
	if( run_a11c || run_simple_ad )
	{	if( run_a11c )
			ok        = a11c();
		else ok        = simple_ad();
		if( ok )
		{	cout << "OK:    " << test_name << std::endl;
			exit(0);
		}
		else
		{	 cout << "Error: " << test_name << std::endl;
			exit(1);
		}
	}

	// max_threads 
	size_t max_threads = arg2size_t( *++argv, 0, 
		"run: max_threads is less than zero"
	);

	size_t mega_sum = 0; // assignment to avoid compiler warning
	if( run_sum_i_inv )
	{	// mega_sum
		mega_sum = arg2size_t( *++argv, 1, 
			"run: mega_sum is less than one"
		);
	}
	else
	{	ok &= run_multi_newton;

		// num_zero
		num_zero = arg2size_t( *++argv, 2,
			"run: num_zero is less than two"
		);

		// num_sub
		num_sub = arg2size_t( *++argv, 1,
			"run: num_sub is less than one"
		);
       
		// num_sum 
		num_sum = arg2size_t( *++argv, 1,
			"run: num_sum is less than one"
		);

		// use_ad
		++argv;
		if( std::strcmp(*argv, "true") == 0 )
			use_ad = true;
		else if( std::strcmp(*argv, "false") == 0 )
			use_ad = false;
		else
		{	std::cerr << "run: use_ad = '" << *argv;
			std::cerr << "' is not true or false" << std::endl;
			exit(1);
		}
	}

	// run the test for each number of threads
	CppAD::vector<size_t> rate_all(max_threads + 1);
	size_t num_threads, inuse_this_thread = 0;
	for(num_threads = 0; num_threads <= max_threads; num_threads++)
	{	// set the number of threads
		if( num_threads > 0 )
			ok &= start_team(num_threads);

		// ammount of memory initialy inuse by thread zero
		ok &= 0 == thread_alloc::thread_num();
		inuse_this_thread = thread_alloc::inuse(0);

		// run the requested test
		if( run_sum_i_inv ) ok &= 
			sum_i_inv_time(rate_all[num_threads], num_threads, mega_sum);
		else
		{	ok &= run_multi_newton;
			ok &= multi_newton_time(
				rate_all[num_threads] ,
				num_threads           ,
				num_zero                ,
				num_sub                 ,
				num_sum                 ,
				use_ad
			);
		}

		// set back to one thread and fee all avaialable memory
		if( num_threads > 0 )
			ok &= stop_team();
		size_t thread;
		for(thread = 0; thread < num_threads; thread++)
		{	thread_alloc::free_available(thread);
			if( thread == 0 )
				ok &= thread_alloc::inuse(thread) == inuse_this_thread;
			else	ok &= thread_alloc::inuse(thread) == 0;
		}
		if( ok )
			cout << "OK:    " << test_name << ": ";
		else
		{	cout << "Error: " << test_name << ": ";
			num_fail++;
		}
		cout << "num_threads = " << num_threads;
		cout << ", rate = " << rate_all[num_threads] << std::endl;
	}
	cout << "rate_all = " << rate_all << std::endl;
	if( num_fail == 0 )
	{	cout << "All " << max_threads + 1 << " " << test_name;
		cout  << " tests passed." << std::endl;
	}
	else
	{	cout << num_fail << " " << test_name;
		cout << " tests failed." << std::endl;
	}
 
	return num_fail;
}

// END PROGRAM
