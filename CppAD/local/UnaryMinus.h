# ifndef CppADUnaryMinusIncluded
# define CppADUnaryMinusIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */

/*
$begin UnaryMinus$$
$spell
	Var
	const
	inline
$$

$index unary, AD minus operator$$
$index AD, unary minus operator$$
$index minus, AD unary operator$$
$index minus, AD unary operator$$
$index -, AD unary operator$$

$section AD Unary Minus Operator$$

$table
$bold Syntax$$ 
$cnext 
$syntax% %y% = - %x%$$
$tend

$fend 20$$

$head Purpose$$
Computes the negative of $italic y$$ where
$code -$$ has the same interpretation as for the $italic Base$$ type.

$head x$$
The operand $italic x$$ has prototype
$syntax%
	const AD<%Base%> &%x%
%$$

$head y$$
The result $italic y$$ has type
$syntax%
	AD<%Base%> %y%
%$$
It is equal to the negative of the operand $italic x$$.

$head Assumptions$$
If the negation unary minus operator is used with an 
$syntax%AD<%Base%>%$$ object,
it must also be defined for 
the base type $italic Base$$.

In addition,
if $latex f$$ is a 
$xref/glossary/Base Function/Base function/$$,
$latex \[
	\D{[ - f(x) ]}{x} = - \D{f(x)}{x}
\] $$



$head Example$$
$children%
	Example/UnaryMinus.cpp
%$$
The file
$xref/UnaryMinus.cpp/$$
contains an example and a test of this operation.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

// Broken g++ compiler inhibits declaring unary minus a member or friend
template <class Base>
inline AD<Base> AD<Base>::operator - (void) const 
{	// should make a more efficient version by adding unary minus to
	// Operator.h (some day)
	AD<Base> result(0);

	result  -= *this;

	return result;
}


template <class Base>
inline AD<Base> operator - (const typename VecAD<Base>::reference &right) 
{	return - right.ADBase(); }

}
//  END CppAD namespace


# endif
