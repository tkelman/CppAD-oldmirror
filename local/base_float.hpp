/* $Id$ */
# ifndef CPPAD_BASE_FLOAT_INCLUDED
# define CPPAD_BASE_FLOAT_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin base_float.hpp$$
$spell
	bool
	Lt Le Eq Ge Gt
	Rel
	CppAD
	CondExpOp
	namespace
	inline
	enum
	const
	exp
	const
$$

$index float, Base$$
$index Base, float$$
$index float, Base$$

$section Enable use of AD<Base> where Base is float$$

$head CondExpOp$$
The type $code float$$ is a relatively simple type 
that supports
$code <$$, $code <=$$, $code ==$$, $code >=$$, and $code >$$ operators
its $code CondExpOp$$ function is defined by
$codep */
namespace CppAD {
	inline float CondExpOp( 
		enum CompareOp     cop          ,
		const float&       left         ,
		const float&       right        , 
		const float&       exp_if_true  , 
		const float&       exp_if_false )
	{	return CondExpTemplate(cop, left, right, exp_if_true, exp_if_false);
	}
}
/* $$

$head CondExpRel$$
The following macro invocation
$codep */
namespace CppAD {
	CPPAD_COND_EXP_REL(float)
}
/* $$
uses $code CondExpOp$$ above to
define $codei%CondExp%Rel%$$ for $code float$$ arguments
and $icode%Rel%$$ equal to
$code Lt$$, $code Le$$, $code Eq$$, $code Ge$$, and $code Gt$$.

$head EqualOpSeq$$
The type $code float$$ is simple (in this respect) and so we define
$codep */
namespace CppAD {
	inline bool EqualOpSeq(const float& x, const float& y)
	{	return x == y; }
}
/* $$

$head Identical$$
The type $code float$$ is simple (in this respect) and so we define
$codep */
namespace CppAD {
	inline bool IdenticalPar(const float& x)
	{	return true; }
	inline bool IdenticalZero(const float& x)
	{	return (x == 0.f); }
	inline bool IdenticalOne(const float& x)
	{	return (x == 1.f); }
	inline bool IdenticalEqualPar(const float& x, const float& y)
	{	return (x == y); }
}
/* $$

$head Integer$$
$codep */
namespace CppAD {
	inline int Integer(const float& x)
	{	return static_cast<int>(x); }
}
/* $$

$end
*/

# endif
