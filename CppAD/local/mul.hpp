# ifndef CppADMulIncluded
# define CppADMulIncluded

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
-------------------------------------------------------------------------------
$begin Mul$$
$spell
	Vec
	const
	Add
$$

$index binary, AD multiply operator$$
$index AD, binary multiply operator$$
$index operator, AD binary multiply$$
$index multiply, AD binary operator$$
$index *, AD binary operator$$

$section AD Binary Multiplication Operator$$

$head Syntax$$

$syntax%%z% = %x% * %y%$$


$head Purpose$$
Computes the product of $italic x$$ times $italic y$$ 
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
	\D{[ f(x) * g(x) ]}{x} = g(x) * \D{f(x)}{x} + f(x) * \D{g(x)}{x}
\] $$

$head Example$$
$children%
	Example/Mul.cpp
%$$
The file
$xref/Mul.cpp/$$
contains an example and test of these operations.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base> AD<Base>::operator *(const AD<Base> &right) const
{
	AD<Base> result;
	CppADUnknownError( Parameter(result) );

	result.value  = value * right.value;

	if( Variable(*this) )
	{	if( Variable(right) )
		{	// result = variable * variable
			Tape()->RecordOp(
				MulvvOp, 
				result, 
				taddr, 
				right.taddr
			);
		}
		else if( IdenticalZero(right.value) )
		{	// result = variable * 0
		}
		else if( IdenticalOne(right.value) )
		{	// result = variable * 1
			result.MakeVariable(	
				taddr
			);
		}
		else
		{	// result = variable * parameter
			Tape()->RecordOp(
				MulvpOp, 
				result, 
				taddr, 
				right.value
			);
		}
	}
	else if( Variable(right) )
	{	if( IdenticalZero(value)  )
		{	// result = 0 * variable
		}
		else if( IdenticalOne(value) )
		{	// result = 1 * variable
			result.MakeVariable(
				right.taddr
			);
		}
		else
		{	// result = parameter * variable
			Tape()->RecordOp(
				MulpvOp, 
				result, 
				value, 
				right.taddr
			);
		}
	}

	return result;
}

// convert other cases to the case above
CppADFoldBinaryOperator(AD<Base>, *)


} // END CppAD namespace

# endif 
