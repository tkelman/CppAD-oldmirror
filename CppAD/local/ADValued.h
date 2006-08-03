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
	CppAD/local/Arithmetic.h%
	CppAD/local/StdMathUnary.h%
	CppAD/local/MathOther.h%
	CppAD/local/CondExp.h%
	CppAD/local/Discrete.h
%$$

$end
*/

// include MathOther.h after CondExp.h because some MathOther.h routines use 
// CondExp.h and CondExp.h is not sufficently declared in Declare.h

# include <CppAD/local/Arithmetic.h>
# include <CppAD/local/StdMathUnary.h>
# include <CppAD/local/CondExp.h>
# include <CppAD/local/MathOther.h>
# include <CppAD/local/Discrete.h>

# endif
