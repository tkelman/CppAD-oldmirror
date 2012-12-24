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
$codei%ipopt::solve(
%retape%, %nf%, %xi%, %xl%, %xu%, %gl%, %gu%, %fg_eval%, %options%, %result%
)%$$

$head Purpose$$
The function $code ipopt::solve$$ solves nonlinear programming
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

$head Vector$$
The type $icode Vector$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$ 
$code double$$.

$head ADvector$$
The type $icode ADvector$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$ 
$code AD<double>$$.

$head retape$$
The argument $icode retape$$ has prototype
$codei%
	bool %retape%
%$$
If it is true,
the $cref/operation sequence/glossary/Operation/Sequence/$$ 
used by $cref/fg_eval/ipopt_solve/fg_eval/$$ 
to compute the elements of $icode fg$$ 
is re-taped for each value of $icode x$$.
Otherwise, it is assumed that this operation sequence
does not depend on $icode x$$
(which should be much faster).

$head nf$$
The argument $icode nf$$ has prototype
$codei%
	size_t %nf%
%$$
It specifies the dimension of the range space for 
$latex f : \B{R}^{nx} \rightarrow \B{R}^{nf}$$.
Let $latex nd(i)$$ be the number of components of $latex x$$
that the function $latex f_i (x)$$ depends on.
One should choose the decomposition of the object into a sum
so as to minimize the maximum, with respect to $latex i$$, of $latex nd(i)$$.

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
	%fg_eval%(%fg%, %x%)
%$$
The arguments to $icode fg_eval$$ have the following meaning:

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
$icode%fg%[%i%] =%$$ $latex f_i (x)$$ and   
for $latex i = 0, \ldots , ng-1$$,
$icode%fg%[%nf% + %i%] =%$$ $latex g_i (x)$$.

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
	ipopt::solution& %result%
%$$
After the optimization process is completed, $icode result$$ contains
the following information:

$subhead status$$
The $icode status$$ field of $icode result$$ has prototype
$codei%
	ipopt::solution_status %result%.status
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
*/
