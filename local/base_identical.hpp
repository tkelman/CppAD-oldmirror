/* $Id$ */
# ifndef CPPAD_BASE_IDENTICAL_INCLUDED
# define CPPAD_BASE_IDENTICAL_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin base_identical$$
$spell
	adolc
	Op
	const
	CppAD
	adolc
	namespace
	bool
	inline
$$

$section Base Type Requirements for Identically Equal Comparisons$$

$head EqualOpSeq$$
$index EqualOpSeq, base require$$
$index base, EqualOpSeq require$$
$index require, base EqualOpSeq$$
If function $cref/EqualOpSeq/$$ is used with 
arguments of type $codei%AD<%Base%>%$$,
the type $icode Base$$ must support the syntax
$codei%
	%b% = EqualOpSeq(%u%, %v%)
%$$
This should return true if and only if 
$icode u$$ is identically equal to $icode v$$
and it makes no different which one is used.
The arguments $icode u$$ and $icode v$$ have prototype
$codei%
	const %Base%& %u%
	const %Base%& %v%
%$$
The return value $icode b$$ has prototype
$codei%
	bool %b%
%$$

$subhead The Simple Case$$
If $icode Base$$ is a relatively simple type,
the $code EqualOpSeq$$ function can be defined by
$codei%
namespace CppAD {
	inline %Base% EqualOpSeq(const %Base%& u, const %Base%& v)
	{	return u == v; }
}
%$$
For example, see
$cref/float EqualOpSeq/base_float.hpp/EqualOpSeq/$$,
$cref/double EqualOpSeq/base_double.hpp/EqualOpSeq/$$, and
$cref/complex EqualOpSeq/base_complex.hpp/EqualOpSeq/$$.

$subhead More Complicated Cases$$
The variables
$icode u$$ and $icode v$$ are not identically equal in the following case
(which CppAD automatically defines $code EqualOpSeq$$ for):
The type $icode Base$$ is $codei%AD<double>%$$,
$icode x[0] = x[1] = 1.$$,
then $cref/independent/$$ is used to make $icode x$$ the independent
variable vector,
and then $icode u = x[0]$$, $icode v = x[1]$$, 
Note that during a future $cref/Forward/$$ calculation,
$icode u$$ and $icode v$$ could correspond to different values.
For example, see
$cref/adolc EqualOpSeq/base_adolc.hpp/EqualOpSeq/$$.

$head Identical$$
$index identical, base require$$
$index base, identical require$$
$index require, base identical$$

$subhead IdenticalPar$$
A $icode Base$$ is a $cref/parameter/glossary/Parameter/$$ 
when used in an $codei%AD<%Base%>%$$ operation sequence.
It is however still possible for a parameter to change its value.
For example,
the $icode Base$$ value $icode u$$ is not identically a parameter
equal in the following case
(which CppAD automatically defines $code IdenticalPar$$ for):
The type $icode Base$$ is $codei%AD<double>%$$,
$icode x[0] = 1.$$,
then $cref/independent/$$ is used to make $icode x$$ the independent
variable vector,
and then $icode u = x[0]$$,
Note that during a future $cref/Forward/$$ calculation,
$icode u$$ could correspond to different values.

$subhead Prototypes$$
The argument $icode u$$ has prototype
$codei%
	const %Base% %u%
%$$
If it is present, the argument $icode v$$ has prototype
$codei%
	const %Base% %v%
%$$
The result $icode b$$ has prototype
$codei%
	bool %b%
%$$

$subhead Identical Functions$$
The type $icode Base$$ must support the following functions:
$table
$bold Syntax$$ $cnext $bold Result$$
$rnext
$icode%b% = IdenticalPar(%u%)%$$ $pre  $$
	$cnext the $icode Base$$ value will always be the same 
$rnext
$icode%b% = IdenticalZero(%u%)%$$ $pre  $$
	$cnext $icode u$$ equals zero and $codei%IdenticalPar(%u%)%$$
$rnext
$icode%b% = IdenticalOne(%u%)%$$ $pre  $$
	$cnext $icode u$$ equals one and $codei%IdenticalPar(%u%)%$$
$rnext
$icode%b% = IdenticalEqualPar(%u%, %v%)%$$ $pre  $$
	$cnext $icode u$$ equals $icode v$$,
 	$codei%IdenticalPar(%u%)%$$ and
 	$codei%IdenticalPar(%v%)%$$
$tend

$subhead Examples$$
See 
$cref/float identical/base_float.hpp/Identical/$$, 
$cref/double identical/base_double.hpp/Identical/$$, 
$cref/complex identical/base_complex.hpp/Identical/$$, and
$cref/adolc identical/base_adolc.hpp/Identical/$$.

$end
*/

# endif
