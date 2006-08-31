# ifndef CppADADValuedIncluded
# define CppADADValuedIncluded

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
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
