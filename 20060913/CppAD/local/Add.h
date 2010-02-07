# ifndef CppADAddIncluded
# define CppADAddIncluded

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
-------------------------------------------------------------------------------
$begin Add$$
$spell
	Vec
	Var
	const
	Add
$$

$index binary, AD add operator$$
$index AD, binary add operator$$
$index operator, AD binary add$$
$index add, AD binary operator$$
$index +, AD binary operator$$

$section AD Binary Addition Operator$$

$head Syntax$$

$syntax%%z% = %x% + %y%$$


$head Purpose$$
Computes the sum of $italic x$$ plus $italic y$$ 
where one of the two operands has type
$syntax%AD<%Base%>%$$ or
$syntax%VecAD<%Base%>::reference%$$.

$head x$$
The operand $italic x$$ has one of the following prototypes
$syntax%
	const int %%                   &%x%
	const %Base%                   &%x%
	const AD<%Base%>               &%x%
	const VecAD<%Base%>::reference &%x%
%$$

$head y$$
The operand $italic y$$ has one of the following prototypes
$syntax%
	const int %%                   &%y%
	const %Base%                   &%y%
	const AD<%Base%>               &%y%
	const VecAD<%Base%>::reference &%y%
%$$

$head z$$
The result $italic z$$ has type
$syntax%
	const AD<%Base%> %z%
%$$

$head Operation Sequence$$
This is an AD of $italic Base$$
$xref/glossary/Operation/Atomic/atomic operation/1/$$
and hence is part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.

$head Derivative$$
If $latex f$$ and $latex g$$ are 
$xref/glossary/Base Function/Base functions/$$
$latex \[
	\D{[ f(x) + g(x) ]}{x} = \D{f(x)}{x} + \D{g(x)}{x}
\] $$

$head Example$$
$children%
	Example/Add.cpp
%$$
The file
$xref/Add.cpp/$$
contains an example and test of these operations.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base> AD<Base>::operator +(const AD<Base> &right) const
{
	AD<Base> result;
	CppADUnknownError( Parameter(result) );

	result.value  = value + right.value;

	if( Variable(*this) )
	{	if( Variable(right) )
		{	// result = variable + variable
			Tape()->RecordOp(AddvvOp, 
				result, taddr, right.taddr
			);
		}
		else if( IdenticalZero(right.value) )
		{	// result = variable + 0
			result.MakeVariable(taddr);
		}
		else
		{	// result = variable + parameter
			Tape()->RecordOp(AddvpOp, 
				result, taddr, right.value
			);
		}
	}
	else if( Variable(right) )
	{	if( IdenticalZero(value) )
		{	// result = 0 + variable
			result.MakeVariable(right.taddr);
		}
		else
		{	// result = parameter + variable
			Tape()->RecordOp(AddpvOp, 
				result, value, right.taddr
			);
		}
	}
	return result;
}

// convert other cases into the case above
CppADFoldBinaryOperator(AD<Base>, +)

} // END CppAD namespace

# endif 