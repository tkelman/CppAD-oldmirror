# ifndef CPPAD_IPOPT_SOLVE_INCLUDED
# define CPPAD_IPOPT_SOLVE_INCLUDED
/* $Id:$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin ipopt_solve$$
$latex
	\newcommand{\B}[1]{ {\bf #1} }
	\newcommand{\R}[1]{ {\rm #1} }
	\newcommand{\W}[1]{ {\; #1 \;} }
$$
$spell
	Bvector
	bool
	retape
	infeasibility
	const
	cpp
	cppad
	doesn't
	ADvector
	eval
	fg
	gl
	gu
	hpp
	inf
	ipopt
	maxiter
	naninf
	nf
	ng
	nx
	obj
	optimizer
	std
	xi
	xl
	xu
	zl
	zu
$$

$section Use Ipopt to Solve a Nonlinear Programming Problem$$

$head Syntax$$
$codei%# include "ipopt_solve.hpp"
%$$
$codei%ipopt_solve(
	%retape%, %xi%, %xl%, %xu%, %gl%, %gu%, %fg_eval%, %options%, %result%
)%$$

$head Purpose$$
The function $code ipopt_solve$$ solves nonlinear programming
problems of the form
$latex \[
\begin{array}{rll}
{\rm minimize}      & \sum_{i=0}^{nf} f_i (x) 
\\
{\rm subject \; to} & gl \leq g(x) \leq gu
\\
                    & xl  \leq x   \leq xu
\end{array}
\] $$
This is done using 
$href%
	http://www.coin-or.org/projects/Ipopt.xml%
	Ipopt
%$$
optimizer and CppAD for the derivative and sparsity calculations. 

$head Bvector$$
The type $icode Bvector$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$ 
$code bool$$.

$head Dvector$$
The type $icode DVector$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$ 
$code double$$.

$head nf$$
The notation $icode nf$$ denotes the dimension of the range space for 
$latex f : \B{R}^{nx} \rightarrow \B{R}^{nf}$$.
Let $latex nd(i)$$ be the number of components of $latex x$$
that the function $latex f_i (x)$$ depends on.
One should choose the decomposition of the object into a sum
so as to minimize the maximum, with respect to $latex i$$, of $latex nd(i)$$.

$head retape$$
The argument $icode retape$$ has prototype
$codei%
	const %Bvector%& %retape%
%$$
and its size is $icode%nf% + %ng%$$.
For $icode%i% = 0 , %...% , %nf%+%ng%-1%$$, if $icode%retape%[%i%]%$$ is true,
the $cref/operation sequence/glossary/Operation/Sequence/$$ 
used by $cref/fg_eval/ipopt_solve/fg_eval/$$ 
to compute $icode%fg%[%i%]%$$ 
is re-taped for each value of $icode x$$.
Otherwise, it is assumed that this operation sequence
does not depend on $icode x$$
(which should be much faster).

$head xi$$
The argument $icode xi$$ has prototype
$codei%
	const %Vector%& %xi%
%$$
and its size is equal to $icode nx$$.
It specifies the initial point where Ipopt starts the optimization process.

$head xl$$
The argument $icode xl$$ has prototype
$codei%
	const %Vector%& %xl%
%$$
and its size is equal to $icode nx$$.
It specifies the lower limits for the argument in the optimization problem.

$head xu$$
The argument $icode xu$$ has prototype
$codei%
	const %Vector%& %xu%
%$$
and its size is equal to $icode nx$$.
It specifies the upper limits for the argument in the optimization problem.

$head gl$$
The argument $icode gl$$ has prototype
$codei%
	const %Vector%& %gl%
%$$
and its size is equal to $icode ng$$.
It specifies the lower limits for the constraints in the optimization problem.

$head gu$$
The argument $icode gu$$ has prototype
$codei%
	const %Vector%& %gu%
%$$
and its size is equal to $icode ng$$.
It specifies the upper limits for the constraints in the optimization problem.

$head fg_eval$$
The argument $icode fg_eval$$ has prototype
$codei%
	%FG_eval% %fg_eval%
%$$
where the class $icode FG_eval$$ is unspecified except for the fact that
it supports the syntax
$codei%
	%FG_eval%::ADvector
	%fg_eval%(%fg%, %x%)
%$$
The type $icode ADvector$$
and the arguments to $icode fg$$, $icode x$$ have the following meaning:

$subhead ADvector$$
The type $icode%FG_eval%::ADVector%$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$ 
$code AD<double>$$.

$subhead x$$
The $icode fg_eval$$ argument $icode x$$ has prototype
$codei%
	const %ADvector%& %x%
%$$
where $icode%nx% = %x%.size()%$$.

$subhead fg$$
The $icode fg_eval$$ argument $icode fg$$ has prototype
$codei%
	%ADvector%& %fg%
%$$
where $icode%nf% + %ng% = %fg%.size()%$$.
The input value of the elements of $icode fg$$ does not matter.
Upon return from $icode fg_eval$$,
for $latex i = 0 , \ldots , nf-1$$,
$codei%
	%fg%[%i%] =%$$ $latex f_i (x)$$ $codei%
%$$
and   for $latex i = 0, \ldots , ng-1$$,
$codei%
	%fg%[%nf% + %i%] =%$$ $latex g_i (x)$$

$head options$$
The argument $icode options$$ has prototype
$codei%
	const std::string& %options%
%$$
It is the name of the ipopt options file used to set
the its options. 

$head result$$
The argument $icode result$$ has prototype
$codei%
	ipopt_solve_result<%Dvector%>& %result%
%$$
After the optimization process is completed, $icode result$$ contains
the following information:

$subhead status$$
The $icode status$$ field of $icode result$$ has prototype
$codei%
	ipopt_solve_result<%Dvector%>::result_status %result%.status
%$$
It is the final Ipopt status for the optimizer. 
Here is a list of the possible values for the status:

$table
$icode status$$ $cnext Meaning
$rnext
not_defined $cnext
The optimizer did not return a final status for this problem.
$rnext
unknown $cnext
The status returned by the optimizer is not defined in the Ipopt
documentation for $code finalize_solution$$.
$rnext
success $cnext
Algorithm terminated successfully at a point satisfying the convergence 
tolerances (see Ipopt options).
$rnext
maxiter_exceeded $cnext
The maximum number of iterations was exceeded (see Ipopt options).
$rnext
stop_at_tiny_step $cnext
Algorithm terminated because progress was very slow.
$rnext
stop_at_acceptable_point $cnext
Algorithm stopped at a point that was converged, 
not to the 'desired' tolerances, but to 'acceptable' tolerances 
(see Ipopt options).
$rnext
local_infeasibility $cnext
Algorithm converged to a non-feasible point
(problem may have no solution).
$rnext
user_requested_stop $cnext
This return value should not happen.
$rnext
diverging_iterates $cnext
It the iterates are diverging.
$rnext
restoration_failure $cnext
Restoration phase failed, algorithm doesn't know how to proceed.
$rnext
error_in_step_computation $cnext
An unrecoverable error occurred while Ipopt tried to 
compute the search direction.
$rnext
invalid_number_detected $cnext
Algorithm received an invalid number (such as $code nan$$ or $code inf$$) 
from the users function $icode%fg_info%.eval%$$ or from the CppAD evaluations
of its derivatives
(see the Ipopt option $code check_derivatives_for_naninf$$).
$rnext
internal_error $cnext
An unknown Ipopt internal error occurred.
Contact the Ipopt authors through the mailing list.
$tend

$subhead x$$
The $code x$$ field of $icode result$$ has prototype
$codei%
	%Vector% %result%.x
%$$
and its size is equal to $icode nx$$.
It is the final $latex x$$ value for the optimizer.

$subhead zl$$
The $code zl$$ field of $icode result$$ has prototype
$codei%
	%Vector% %result%.zl
%$$
and its size is equal to $icode nx$$.
It is the final Lagrange multipliers for the 
lower bounds on $latex x$$.

$subhead zu$$
The $code zu$$ field of $icode result$$ has prototype
$codei%
	%Vector% %result%.zu
%$$
and its size is equal to $icode nx$$.
It is the final Lagrange multipliers for the 
upper bounds on $latex x$$.

$subhead g$$
The $code g$$ field of $icode result$$ has prototype
$codei%
	%Vector% %result%.g
%$$
and its size is equal to $icode ng$$.
It is the final value for the constraint function $latex g(x)$$.

$subhead lambda$$
The $code lambda$$ field of $icode result$$ has prototype
$codei%
	%Vector%> %result%.lambda
%$$
and its size is equal to $icode ng$$.
It is the final value for the 
Lagrange multipliers corresponding to the constraint function.

$subhead obj_value$$
The $code obj_value$$ field of $icode result$$ has prototype
$codei%
	double %result%.obj_value
%$$
It is the final value of the objective function $latex f(x)$$.
	
$end
-------------------------------------------------------------------------------
*/
# include <cppad_ipopt_nlp.hpp>

CPPAD_BEGIN_NAMESPACE
/*!
Class that contains information about ipopt_solve problem solution

\tparam Vector
a simple vector with elements of type double
*/
template <class Vector>
class ipopt_solve_result 
{	typedef typename cppad_ipopt::cppad_ipopt_solution::solution_status 
		result_status;
public:
	/// possible values for solution status
	result_status status;
	/// the approximation solution
	Vector x;
	/// Lagrange multipliers corresponding to lower bounds on x
	Vector z_l;
	/// Lagrange multipliers corresponding to upper bounds on x
	Vector z_u;
	/// value of g(x)
	Vector g;
	/// Lagrange multipliers correspondiing constraints on g(x)
	Vector lambda;
	/// value of f(x)
	double obj_value;
	/// constructor initializes solution status as not yet defined
	cppad_ipopt_solution(void)
	{	status = not_defined; }
};

/*!
FG_info class used by cppad_ipopt_nlp (used for temporary conversion).
*/
template <class ADvector, class FG_eval>
class ipopt_solve_fg_info : public cppad_ipopt::cppad_ipopt_fg_info
{
private:
	size_t   nf_;
	size_t   ng_;
	FG_eval& fg_eval_;
public:
	// derived class constructor
	FG_info(size_t nf, size_t ng, FG_eval& fg_eval)
	: nf_(nf), ng_(ng), fg_eval_(fg_eval)
	{ }
	// Evaluation of f(x) and g(x) using AD
	ADvector eval_r(size_t k, const ADVector& x)
	{	size_t i;
		ADvector fg(nf_ + ng_);
		fg_eval_(x);
		ADvector r(1 + ng_);
		r[0] = fg[0];
		for(i = 1; i < nf; i++)
			r[0] += fg[i];
		for(i = 0; i < ng; i++)
			r[1 + i] = fg[nf + i];
		return r;
	}
	bool retape(size_t k)
	{	return true; }
	};
}

/*!
Use Ipopt to Solve a Nonlinear Programming Problem

\tparam Bvector
simple vector class with elements of type bool.

\tparam Dvector
simple vector class with elements of type double.

\tparam FG_eval
function object used to evaluate f(x) and g(x); see fg_eval below.
It must also support
\code
	FG_eval::ADvector
\endcode
to dentify the type used for the arguments to fg_eval.

\param retape
identify which components of fg need to be retaped.

\param xi
initial argument value to start optimization procedure at.

\param xl
lower limit for argument during optimization

\param xu
upper limit for argument during optimization

\param gl
lower limit for g(x) during optimization.

\param gu
upper limit for g(x) during optimization.

\param fg_eval
function that evaluates the objective and constraints using the syntax
\code
	fg_eval(fg, x)
\endcode

\param options
file that contains the Ipopt options.

\param result
structure that holds the results of the optimization.
*/
template <class Bvector, class Dvector, class FG_eval>
void ipopt_solve(
	const Bvector&                retape  , 
	const Dvector&                xi      , 
	const Dvector&                xl      ,
	const Dvector&                xu      , 
	const Dvector&                gl      , 
	const Dvector&                gu      , 
	FG_eval&                      fg_eval , 
	const std::string&            options ,
	ipopt_solve_result<Dvector>&  result  )
{	typedef typename FG_eval::ADvector ADvector;
	size_t i;
	bool ok;

	CPPAD_ASSERT_KNOWN(
		xi.size() == xl.size() && xi.size() == xu.size() ,
		"ipopt_solve: size of xi, xl, and xu are not all equal."
	);
	CPPAD_ASSERT_KNOWN(
		gl.size() == gu.size() ,
		"ipopt_solve: size of gl and gu are not equal."
	);
	CPPAD_ASSERT_KNOWN(
		retape.size() > gl.size() ,
		"ipopt_solve: size of retape is not greater than size of gl."
	);
	size_t nx = xi.size();
	size_t ng = gl.size();
	size_t nf = retape.size() - ng;

	// convert to types expected by cppad_ipopt_nlp
	cppad_ipopt::NumberVector x_i(nx), x_l(nx), x_u(nx), g_l(ng), g_u(ng);
	for(i = 0; i < nx; i++)
	{	x_i[i] = xi[i];
		x_l[i] = xl[i];
		x_u[i] = xu[i];
	}
	for(i = 0; i < ng; i++)
	{	g_l[i] = gl[i];
		g_u[i] = gu[i];
	}

	// ipopt callback function
	ipopt_solve_fg_info<ADvector, FG_eval> fg_info(nf, ng, fg_eval);

	// Create an interface from Ipopt to this specific problem.
	// Note the assumption here that ADvector is same as cppd_ipopt::ADvector
	cppad_ipopt::cppad_ipopt_solution solution;
	Ipopt::SmartPtr<Ipopt::TNLP> cppad_nlp = new_ipopt_nlp(
		nx, ng, x_i, x_l, x_u, g_l, g_u, &fg_info, &solution
	);

	// Create an IpoptApplication
	using Ipopt::IpoptApplication;
	Ipopt::SmartPtr<IpoptApplication> app = new IpoptApplication();

	// set the options file
	app->Options()->SetStringValue("option_file_name", options.c_str());

	// Initialize the IpoptApplication and process the options
	Ipopt::ApplicationReturnStatus status = app->Initialize();
	ok    &= status == Ipopt::Solve_Succeeded;
	if( ! ok )
	{	result.status = result_status::unknown;
		return;
	}

	// Run the IpoptApplication
	app->OptimizeTNLP(cppad_nlp);

	// pass back the result
	result.status    = solution.status;
	result.obj_value = solution.obj_value;
	for(i = 0; i < nx; i++)
	{	result.x[i]  = solution.x[i];
		result.zl[i] = solution.z_l[i];
		result.zu[i] = solution.z_u[i];
	}
	for(i = 0; i < ng; i++)
	{	result.g[i]      = solution.g[i];
		result.lambda[i] = solution.lambda[i];
	}
	return;
}

CPPAD_END_NAMESPACE
# endif
