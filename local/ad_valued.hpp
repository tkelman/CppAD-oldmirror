# ifndef CPPAD_AD_VALUED_INCLUDED
# define CPPAD_AD_VALUED_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ADValued$$
$spell
$$

$index operation, AD valued$$
$index function, AD valued$$

$section AD Valued Operations and Functions$$

$childtable%
	CppAD/local/arithmetic.hpp%
	CppAD/local/std_math_unary.hpp%
	CppAD/local/math_other.hpp%
	CppAD/local/cond_exp.hpp%
	CppAD/local/discrete.hpp
%$$

$end
*/

// include MathOther.h after CondExp.h because some MathOther.h routines use 
// CondExp.h and CondExp.h is not sufficently declared in Declare.h

# include <CppAD/local/arithmetic.hpp>
# include <CppAD/local/std_math_unary.hpp>
# include <CppAD/local/cond_exp.hpp>
# include <CppAD/local/math_other.hpp>
# include <CppAD/local/discrete.hpp>

# endif
