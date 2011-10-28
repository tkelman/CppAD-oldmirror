/* $Id$ */
# ifndef CPPAD_NEWTON_METHOD_INCLUDED
# define CPPAD_NEWTON_METHOD_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin openmp_multi_newton.hpp$$
$spell
	num
	openmp
	df
	xout
	xlow
	xup
	itr
	CppAD
	const
	bool
$$

$index OpenMP, multi_thread Newton$$
$index multi_thread, OpenMP Newton method$$
$index example, OpenMP multi_thread Newton$$
$index newton, multi_thread OpenMP$$

$section An OpenMP Multi-Threaded Newton's Method$$

$head Syntax$$
$codei%multi_newton(%xout%, 
	%fun%, %num_sub%, %xlow%, %xup%, %epsilon%, %max_itr%, %use_openmp%
)%$$


$head Purpose$$
Determine the argument values $latex x \in [a, b]$$ (where $latex a < b$$)
such that $latex f(x) = 0$$.

$head Method$$
For $latex i = 0 , \ldots , n$$,  
we define the $th i$$ grid point $latex g_i$$ by
$latex \[
	g_i = a \frac{n - i}{n} +  b \frac{i}{n}
\] $$
For $latex i = 0 , \ldots , n-1$$,  
we define the $th i$$ sub-interval of $latex [a, b]$$ by
$latex \[
	I_i = [ g_i , g_{i+1} ]
\] $$
Newton's method is applied starting
at the center of each of the intervals $latex I_i$$ for
$latex i = 0 , \ldots , n-1$$
and at most one zero is found for each interval.


$head xout$$
The argument $icode xout$$ has the prototype
$codei%
	CppAD::vector<double> &%xout%
%$$
The input size and value of the elements of $icode xout$$ do not matter.
Upon return from $code openmp_multi_newton$$,
the size of $icode xout$$ is less than or equal $latex n$$ and
$latex \[
	| f( xout[i] ) | \leq epsilon
\] $$ 
for each valid index $icode i$$.
Two $latex x$$ solutions are considered equal (and joined as one) if
the absolute difference between the solutions is less than
$latex (b - a) / n$$.

$head fun$$
The argument $icode fun$$ has prototype
$codei%
	%Fun% &%fun%
%$$
This argument must evaluate the function $latex f(x)$$,
and its derivative $latex f^{(1)} (x)$$, 
using the syntax
$codei%
	%fun%(%x%, %f%, %df%)
%$$
where the arguments have the prototypes
$codei%
	double    %x% 
	double&   %f%
	double&   %df%
%$$.
The input values of $icode f$$ and $icode df$$ do not matter.
Upon return they are $latex f(x)$$ and $latex f^{(1)} (x)$$ respectively.

$head num_sub$$
The argument $icode num_sub$$ has prototype
$codei%
	size_t %num_sub%
%$$
It specifies the number of sub-intervals; i.e., $latex n$$ 
in the $cref/method/openmp_multi_newton.hpp/Method/$$ above.

$head xlow$$
The argument $icode xlow$$ has prototype
$codei%
	double %xlow%
%$$
It specifies the lower limit for the entire search; i.e., $latex a$$
in the $cref/method/openmp_multi_newton.hpp/Method/$$ above.

$head xup$$
The argument $icode xup$$ has prototype
$codei%
	double %xup%
%$$
It specifies the upper limit for the entire search; i.e., $latex b$$
in the $cref/method/openmp_multi_newton.hpp/Method/$$ above.

$head epsilon$$
The argument $icode epsilon$$ has prototype
$codei%
	double %epsilon%
%$$
It specifies the convergence criteria for Newton's method in terms
of how small the function value must be.

$head max_itr$$
The argument $icode max_itr$$ has prototype
$codei%
	size_t %max_itr%
%$$
It specifies the maximum number of iterations of Newton's method to try
before giving up on convergence.

$head use_openmp$$
The argument $icode use_openmp$$ has prototype
$codei%
	bool %use_openmp%
%$$
If it is true, OpenMP is used for the calculations,
(and the number of threads is not changed).
Otherwise, the calculation is done without multi-threading.


$head Source$$
$code
$verbatim%multi_thread/openmp/multi_newton.hpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$
$end
---------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
# include <cassert>
# include <omp.h>

namespace { // BEGIN CppAD namespace

template <class Fun>
void one_newton(Fun& fun, double &fcur, double &xcur,
	double xlow, double xin, double xup, double epsilon, size_t max_itr)
{	using CppAD::AD;
	using CppAD::vector;
	using CppAD::abs;

	size_t itr;
	xcur = xin;
	double dfcur = 0.;
	for(itr = 0; itr < max_itr; itr++)
	{	fun(xcur, fcur, dfcur);
		// check end of iterations
		if( abs(fcur) <= epsilon )
			return;
		if( (xcur == xlow) & (fcur * dfcur > 0.) )
			return; 
		if( (xcur == xup)  & (fcur * dfcur < 0.) )
			return; 
		if( dfcur == 0. )
			return;
		// next Newton iterate
		xcur = xcur - fcur / dfcur;
		// keep in bounds
		xcur = std::max(xcur, xlow);
		xcur = std::min(xcur, xup);
	}
	return;
}

template <class Fun>
void multi_newton(
	CppAD::vector<double> &xout , 
	Fun &fun                    , 
	size_t num_sub                , 
	double xlow                 , 
	double xup                  , 
	double epsilon              , 
	size_t max_itr              ,
	bool   use_openmp           )
{	using CppAD::AD;
	using CppAD::vector;
	using CppAD::abs;

	// check argument values
	assert( xlow < xup );
	assert( num_sub > 0 );

	// OpenMP uses integers in place of size_t
	int i, n = int(num_sub);

	// set up grid
	vector<double> grid(num_sub + 1);
	vector<double> fcur(n), xcur(n), xmid(n);
	double dx = (xup - xlow) / double(n);
	for(i = 0; i < n; i++)
	{	grid[i] = xlow + i * dx;
		xmid[i] = xlow + (i + .5) * dx;
	}
	grid[n] = xup;

	if( use_openmp )
	{
# pragma omp parallel for 
		for(i = 0; i < n; i++) 
		{	one_newton(
				fun       , 
				fcur[i]   ,
				xcur[i]   ,
				grid[i]   , 
				xmid[i]   , 
				grid[i+1] , 
				epsilon   , 
				max_itr
			);
		}
// end omp parallel for 
 	}
 	else
	{
		for(i = 0; i < n; i++) 
		{	one_newton(
				fun       , 
				fcur[i]   ,
				xcur[i]   ,
				grid[i]   , 
				xmid[i]   , 
				grid[i+1] , 
				epsilon   , 
				max_itr
			);
		}
	}
	// remove duplicates and points that are not solutions
	double xlast  = xlow;
	size_t ilast  = 0;
	size_t num_zero = 0;
	for(i = 0; i < n; i++)
	{	if( abs( fcur[i] ) <= epsilon )
		{	if( num_zero == 0 )
			{	xcur[num_zero++] = xlast = xcur[i];
				ilast = i;
			}
			else if( fabs( xcur[i] - xlast ) > dx ) 
			{	xcur[num_zero++] = xlast = xcur[i];
				ilast = i;
			}
			else if( fabs( fcur[i] ) < fabs( fcur[ilast] ) )
			{	xcur[num_zero - 1] = xlast = xcur[i]; 
				ilast = i;
			}
		}
	}

	// resize output vector and set its values
	xout.resize(num_zero);
	for(i = 0; size_t(i) < num_zero; i++)
		xout[i] = xcur[i];

	return;
}

} // END CppAD namespace

// END PROGRAM

# endif
