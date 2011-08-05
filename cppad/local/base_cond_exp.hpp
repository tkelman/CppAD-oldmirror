/* $Id$ */
# ifndef CPPAD_BASE_COND_EXP_INCLUDED
# define CPPAD_BASE_COND_EXP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/* 
$begin base_cond_exp$$
$spell
	hpp
	enum
	namespace
	Op
	Lt
	Le
	Eq
	Ge
	Gt
	Ne
	cond
	exp
	const
	adolc
	CppAD
	inline
$$

$section base_cond_exp$$
$index CondExp, base require$$
$index base, CondExp require$$
$index require, base CondExp$$

$head Purpose$$
These definitions are required by the user's code to support the 
$icode Base$$ type for $cref/CondExp/$$ operations:

$head CompareOp$$
The following $code enum$$ type is used in the specifications below:
$codep */
namespace CppAD {

	// The conditional expression operator enum type
	enum CompareOp 
	{	CompareLt, // less than
		CompareLe, // less than or equal
		CompareEq, // equal
		CompareGe, // greater than or equal
		CompareGt, // greater than
		CompareNe  // not equal
	};
}
/* $$

$head CondExpTemplate$$
The type $icode Base$$ must support the syntax
$codei%
	%result% = CondExpOp(%cop%, %left%, %right%, %exp_if_true%, %exp_if_false%)
%$$
which computes implements the corresponding $cref/CondExp/$$ 
function when the result has prototype
$codei%
	%Base% %result%
%$$
The argument $icode cop$$ has prototype
$codei%
	enum CppAD::CompareOp %cop%
%$$ 
The other arguments have the prototype
$codei%
	const %Base%&  %left% 
	const %Base%&  %right% 
	const %Base%&  %exp_if_true%
	const %Base%&  %exp_if_false% 
%$$

$subhead Ordered Type$$
If $icode Base$$ is a relatively simple type
that supports
$code <$$, $code <=$$, $code ==$$, $code >=$$, and $code >$$ operators
its $code CondExpOp$$ function can be defined by
$codei%
namespace CppAD {
	inline %Base% CondExpOp(
	enum CppAD::CompareOp  cop            ,
	const %Base%           &left          ,
	const %Base%           &right         ,
	const %Base%           &exp_if_true   ,
	const %Base%           &exp_if_false  )
	{	return CondExpTemplate(
			cop, left, right, trueCase, falseCase);
	}
}
%$$
For example, see 
$cref/double CondExpOp/base_float.hpp/CondExpOp/$$.
For an example of and implementation of $code CondExpOp$$ with
a more involved $icode Base$$ type see
$cref/adolc CondExpOp/base_adolc.hpp/CondExpOp/$$.
 

$subhead Not Ordered$$
If the type $icode Base$$ does not support ordering,
the $code CondExpOp$$ function does not make sense.
In this case one might (but need not) define $code CondExpOp$$ as follows:
$codei%
namespace CppAD {
	inline %Base% CondExpOp(
	enum CompareOp cop           ,
	const %Base%   &left         ,
	const %Base%   &right        ,
	const %Base%   &exp_if_true  ,
	const %Base%   &exp_if_false )
	{	// attempt to use CondExp with a %Base% argument
		assert(0);
		return %Base%(0);
	}
}
%$$
For example see
$cref/complex CondExpOp/base_complex.hpp/CondExpOp/$$.
 

$end
*/

CPPAD_BEGIN_NAMESPACE

/*!
\file base_cond_exp.hpp
CondExp operations that aid in meeting Base type requirements.

\tparam CompareType
is the type of the left and right operands to the comparision operator.

\tparam ResultType
is the type of the result, which is the same as \c CompareType except
during forward and reverse mode sparese calculations.

\param cop
specifices which comparision to use; i.e.,
$code <$$,
$code <=$$,
$code ==$$,
$code >=$$,
$code >$$, or
$code !=$$.

\param left
is the left operand to the comparision operator.

\param right
is the right operand to the comparision operator.

\param exp_if_true
is the return value is the comparision results in true.

\param exp_if_true
is the return value is the comparision results in false.

\return
see \c exp_if_true and \c exp_if_false above.
*/
template <class CompareType, class ResultType>
ResultType CondExpTemplate( 
	enum  CompareOp            cop          ,
	const CompareType&         left         ,
	const CompareType&         right        , 
	const ResultType&          exp_if_true  , 
	const ResultType&          exp_if_false )
{	ResultType returnValue;
	switch( cop )
	{
		case CompareLt:
		if( left < right )
			returnValue = exp_if_true;
		else	returnValue = exp_if_false;
		break;

		case CompareLe:
		if( left <= right )
			returnValue = exp_if_true;
		else	returnValue = exp_if_false;
		break;

		case CompareEq:
		if( left == right )
			returnValue = exp_if_true;
		else	returnValue = exp_if_false;
		break;

		case CompareGe:
		if( left >= right )
			returnValue = exp_if_true;
		else	returnValue = exp_if_false;
		break;

		case CompareGt:
		if( left > right )
			returnValue = exp_if_true;
		else	returnValue = exp_if_false;
		break;

		default:
		CPPAD_ASSERT_UNKNOWN(0);
		returnValue = exp_if_true;
	}
	return returnValue;
}

CPPAD_END_NAMESPACE
# endif
