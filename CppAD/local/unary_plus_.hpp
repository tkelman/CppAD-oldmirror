# ifndef CppADUnaryPlusIncluded
# define CppADUnaryPlusIncluded

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */

/*
$begin UnaryPlus$$
$spell
	Vec
	const
	inline
$$

$index unary, AD plus operator$$
$index AD, unary plus operator$$
$index plus, AD unary operator$$
$index +, AD unary operator$$

$section AD Unary Plus Operator$$

$head Syntax$$

$syntax%%y% = + %x%$$


$head Purpose$$
Performs the unary plus operation
(the result $italic y$$ is equal to the operand $italic x$$).


$head x$$
The operand $italic x$$ has one of the following prototypes
$syntax%
	const AD<%Base%>               &%x%
	const VecAD<%Base%>::reference &%x%
%$$

$head y$$
The result $italic y$$ has type
$syntax%
	AD<%Base%> %y%
%$$
It is equal to the operand $italic x$$.

$head Operation Sequence$$
This is an AD of $italic Base$$
$xref/glossary/Operation/Atomic/atomic operation/1/$$
and hence is part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.

$head Derivative$$
If $latex f$$ is a 
$xref/glossary/Base Function/Base function/$$,
$latex \[
	\D{[ + f(x) ]}{x} = \D{f(x)}{x}
\] $$



$head Example$$
$children%
	Example/UnaryPlus.cpp
%$$
The file
$xref/UnaryPlus.cpp/$$
contains an example and test of this operation.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
inline AD<Base> AD<Base>::operator + (void) const 
{	AD<Base> result(*this);

	return result;
}


template <class Base>
inline AD<Base> operator + (const VecAD_reference<Base> &right) 
{	return right.ADBase(); }

}
//  END CppAD namespace


# endif
