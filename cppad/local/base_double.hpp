/* $Id$ */
# ifndef CPPAD_BASE_DOUBLE_INCLUDED
# define CPPAD_BASE_DOUBLE_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin base_double.hpp$$
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

$index double, Base$$
$index Base, double$$
$index double, Base$$

$section Enable use of AD<Base> where Base is double$$

$head CondExpOp$$
The type $code double$$ is a relatively simple type 
that supports
$code <$$, $code <=$$, $code ==$$, $code >=$$, and $code >$$ operators
its $code CondExpOp$$ function is defined by
$codep */
namespace CppAD {
	inline double CondExpOp( 
		enum CompareOp     cop          ,
		const double&       left         ,
		const double&       right        , 
		const double&       exp_if_true  , 
		const double&       exp_if_false )
	{	return CondExpTemplate(cop, left, right, exp_if_true, exp_if_false);
	}
}
/* $$

$head CondExpRel$$
The following macro invocation
$codep */
namespace CppAD {
	CPPAD_COND_EXP_REL(double)
}
/* $$
uses $code CondExpOp$$ above to
define $codei%CondExp%Rel%$$ for $code double$$ arguments
and $icode%Rel%$$ equal to
$code Lt$$, $code Le$$, $code Eq$$, $code Ge$$, and $code Gt$$.

$head EqualOpSeq$$
The type $code double$$ is simple (in this respect) and so we define
$codep */
namespace CppAD {
	inline bool EqualOpSeq(const double& x, const double& y)
	{	return x == y; }
}
/* $$

$head Identical$$
The type $code double$$ is simple (in this respect) and so we define
$codep */
namespace CppAD {
	inline bool IdenticalPar(const double& x)
	{	return true; }
	inline bool IdenticalZero(const double& x)
	{	return (x == 0.); }
	inline bool IdenticalOne(const double& x)
	{	return (x == 1.); }
	inline bool IdenticalEqualPar(const double& x, const double& y)
	{	return (x == y); }
}
/* $$

$head Integer$$
$codep */
namespace CppAD {
	inline int Integer(const double& x)
	{	return static_cast<int>(x); }
}
/* $$

$end
*/

# endif
