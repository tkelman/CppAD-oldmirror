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
$begin multi_newton_time.cpp$$
$spell
	num
	Cpp
	bool
	alloc
	openmp
$$
$index multi_thread, Newton AD speed$$
$index thread, multi_newton AD speed$$
$index AD, speed multi_thread Newton$$
$index newton, multi_thread AD speed$$

$section Timing Test of Multi-Threaded Newton Method$$

$head Syntax$$
$icode%ok% = multi_newton_time(%rate_out%, %num_threads%, 
	%num_zero%, %num_sub%, %num_sum%, %use_ad%
)%$$ 

$head Purpose$$
Runs correctness and timing test for a multi-threaded Newton method.
This test uses Newton's method to determine all the zeros of the sine
function on an interval.
CppAD, or hand coded derivatives,
can be used to calculate the derivatives used by Newton's method.
The calculation can be done in parallel on the different sub-intervals.
In addition, the calculation can be done without multi-threading.

$head ok$$
This return value has prototype
$codei%
	bool %ok%
%$$
If it is true,
$code multi_newton_time$$ passed the correctness test.
Otherwise it is false.

$head rate_out$$
This argument has prototype
$codei%
	size_t& %rate_out%
%$$
The input value of the argument does not matter.
Upon return it is the number of times per second that
the multi-threaded Newton method can compute all the zeros.

$head num_threads$$
This argument has prototype
$codei%
	size_t %num_threads%
%$$
It specifies the number of threads that 
are available for this test.
If it is zero, the test is run without multi-threading and 
$codei%
	1 == CppAD::thread_alloc::num_threads()
%$$
when $code multi_newton_time$$ is called.
If it is non-zero, the test is run with multi-threading and
$codei%
	%num_threads% == CppAD::thread_alloc::num_threads()
%$$
when $code multi_newton_time$$ is called.

$head num_zero$$
This argument has prototype
$codei%
	size_t %num_zero%
%$$
and it must be greater than one.
It specifies the actual number of zeros in the test function
$latex \sin(x)$$. 
To be specific, $code multi_newton_time$$ will attempt to determine
all of the values of $latex x$$ for which $latex \sin(x) = 0 $$ and
$latex x$$ is in the interval
$codei%
	[ 0 , (%num_zero% - 1) * %pi% ]
%$$.

$head num_sub$$
This argument has prototype
$codei%
	size_t %num_sub%
%$$
It specifies the number of sub-intervals to divide the total interval into.
It must be greater than zero and
should probably be greater than two times $icode num_zero$$.

$head num_sum$$
This argument has prototype
$codei%
	size_t %num_sum%
%$$
and must be greater than zero.
The actual function used by the Newton method is
$latex \[
	f(x) = \frac{1}{n} \sum_{i=1}^{n} \sin (x)
\] $$
where $latex n$$ is equal to $icode num_sum$$.
Larger values of $icode num_sum$$ simulate a case where the
evaluation of the function $latex f(x)$$ takes more time.

$head use_ad$$
This argument has prototype
$codei%
	bool %user_ad%
%$$
If $icode use_ad$$ is $code true$$,
then derivatives will be computed using CppAD.
Note that this derivative computation includes 
re-taping the function for each
value of $latex x$$ (even though re-taping is not necessary).
$pre

$$
If $icode use_ad$$ is $code false$$, 
derivatives will be computed using a hand coded routine.

$head multi_newton$$
The subroutine $code multi_newton$$ is multi-threading system dependent.
A different version of this routine is implemented for
$cref/openmp/openmp_multi_newton.cpp/$$.

$head Source$$
$code
$verbatim%multi_thread/multi_newton_time.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
# include <cmath>
# include <cstring>
# include <omp.h>

// required interface implemented by <system>/sum_i_inv.cpp where <system> is 
// openmp, pthread, or bthread.
extern bool multi_newton(
	CppAD::vector<double> &xout                , 
	void fun(double x, double& f, double& df)  , 
	size_t num_sub                             , 
	double xlow                                , 
	double xup                                 , 
	double epsilon                             , 
	size_t max_itr                             ,
	size_t num_threads
);


namespace { // empty namespace 

	// values correspond to arguments in previous call to multi_newton_time
	size_t num_threads_;// value passed to multi_newton_time 
	size_t num_zero_;   // number of zeros of f(x) in the total interval
	size_t num_sub_;    // number of sub-intervals to split calculation into
	size_t num_sum_;    // larger values make f(x) take longer to calculate

	// either fun_ad or fun_no depending on value of use_ad
	void (*fun_)(double x, double& f, double& df) = 0;

	// A version of the sine function that can be made as slow as we like
	template <class Float>
	Float f_eval(Float x)
	{	Float sum = 0.;
		size_t i;
		for(i = 0; i < num_sum_; i++)
			sum += sin(x);

		return sum / Float(num_sum_);
	}

	// Direct calculation of derivative with same number of floating point
	// operations as for f_eval.
	double df_direct(double x)
	{	double sum = 0.;
		size_t i;
		for(i = 0; i < num_sum_; i++)
			sum += cos(x);

		return sum / double(num_sum_);
	}

	// AD calculation of detivative
	void fun_ad(double x, double& f, double& df)
	{	// use CppAD::vector because it uses fast multi-threaded memory alloc
		using CppAD::vector;
		using CppAD::AD;	
		vector< AD<double> > X(1), Y(1);
		X[0] = x;
		CppAD::Independent(X);
		Y[0] = f_eval(X[0]);
		CppAD::ADFun<double> F(X, Y);
		vector<double> dx(1), dy(1);
		dx[0] = 1.;
		dy    = F.Forward(1, dx);
		f     = Value( Y[0] );
		df    = dy[0];
		return;
	} 

	// evaulate the function and its derivative
	void fun_no(double x, double& f, double& df) 
	{	f  = f_eval(x);
		df = df_direct(x);
		return;
	}


	// Run computation of all the zeros once
	void test_once(CppAD::vector<double> &xout, size_t no_size)
	{	if(  num_zero_ == 0 )
		{	std::cerr << "multi_newton_time: num_zero == 0" << std::endl;
			exit(1);
		}
		double pi      = 4. * std::atan(1.); 
		double xlow    = 0.;
		double xup     = (num_zero_ - 1) * pi;
		double eps     = 100. * CppAD::epsilon<double>();
		size_t max_itr = 20;
	
		bool ok = multi_newton(
			xout        ,
			fun_        ,
			num_sub_      ,
			xlow        ,
			xup         ,
			eps         ,
			max_itr     ,
			num_threads_ 
		);
		if( ! ok )
		{	std::cerr << "multi_newton: error" << std::endl;
			exit(1);
		}
		return;
	}

	// Repeat computation of all the zeros a specied number of times
	void test_repeat(size_t size, size_t repeat)
	{	size_t i;
		CppAD::vector<double> xout;
		for(i = 0; i < repeat; i++)
			test_once(xout, size);
		return;
	}
} // end empty namespace

bool multi_newton_time(
	size_t& rate_out      ,
	size_t  num_threads   ,
	size_t  num_zero      ,
	size_t  num_sub       , 
	size_t  num_sum       ,
	bool    use_ad
) 
{	bool ok = true;
	using CppAD::vector;
	using CppAD::thread_alloc;

	// Set local namespace environment variables
	num_threads_  = num_threads;
	num_zero_     = num_zero;
	num_sub_      = num_sub;
	num_sum_      = num_sum;
	if( use_ad )
		fun_ = fun_ad;
	else	fun_ = fun_no;

	// expect number of threads to already be set up
	if( num_threads > 0 )
		ok &= num_threads == CppAD::thread_alloc::num_threads();
	else	ok &= 1           == CppAD::thread_alloc::num_threads();

	// minimum time for test (repeat until this much time)
	double time_min = 1.;

	// size of the one test case (not used)
	vector<size_t> no_size_vec(1);

	// run the test case
	vector<size_t> rate_vec = CppAD::speed_test(
		test_repeat, no_size_vec, time_min
	);

	// return the rate (times per second) at which test_once runs
	rate_out = rate_vec[0];

	// Call test_once for a correctness check
	vector<double> xout;
	size_t no_size = 0;
	test_once(xout, no_size);
	double eps = 100. * CppAD::epsilon<double>();
	double pi  = 4. * std::atan(1.);
	ok        &= (xout.size() == num_zero);
	size_t i   = 0;
	for(i = 0; i < num_zero; i++)
		ok &= std::fabs( xout[i] - pi * i) <= 2 * eps;

	// return correctness check result
	return  ok;
}
// END PROGRAM
