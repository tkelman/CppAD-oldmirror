// $Id$
# ifndef CPPAD_BASE_REQUIRE_INCLUDED
# define CPPAD_BASE_REQUIRE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin base_require$$
$spell
	isnan
	Lt
	Le
	Eq
	Ge
	Gt
	cppad.hpp
	namespace
	acos
	asin
	atan
	cos
	sqrt
	optimizations
	bool
	const
	CppAD
	enum
	Lt
	Le
	Eq
	Ge 
	Gt
	inline
	Op
	std
	CondExp
	erf
$$

$index Base, require$$
$index require, Base type$$
$index type, Base require$$

$section AD<Base> Requirements for Base Type$$

$head Syntax$$
$code include <cppad/base_require.hpp>$$

$head Warning$$
This is a preliminary version of these specifications
and it is subject to change in future versions of CppAD.

$head Purpose$$
This section lists the requirements for the type
$icode Base$$ so that the type $codei%AD<%Base%>%$$ can be used.

$subhead Standard Base Types$$
In the case where $icode Base$$ is 
$code float$$, 
$code double$$,
$code std::complex<float>$$, 
$code std::complex<double>$$,
or $codei%AD<%Other%>%$$,
these requirements are provided by including he file
$code cppad/cppad.hpp$$.

$head Include$$
If you are linking a non-standard base type to CppAD,
you must first include the file $code cppad/base_require.hpp/$$,
then provide the specifications below, 
and then include the file $code cppad/cppad.hpp$$.

$head Numeric Type$$
The type $icode Base$$ must support all the operations for a 
$cref/NumericType/$$.

$head isnan$$
If your base type defines the $code isnan$$ function,
you may have to override its definition in the CppAD namespace
(to avoid a function ambiguity).
For example, see the complex version of $cref/isnan/base_complex.hpp/isnan/$$.

$head Integer$$
$index Integer, base require$$
$index base, Integer require$$
$index require, base Integer$$
The type $icode Base$$ must support the syntax
$codei%
	%i% = Integer(%x%)
%$$
which converts $icode x$$ to an $code int$$.
The argument $icode x$$ has prototype
$codei%
	const %Base%& %x%
%$$
and the return value $icode i$$ has prototype
$codei%
	int %i%
%$$
For example, see
$cref/float EqualOpSeq/base_float.hpp/EqualOpSeq/$$,
$cref/double EqualOpSeq/base_double.hpp/EqualOpSeq/$$, and
$cref/complex EqualOpSeq/base_complex.hpp/EqualOpSeq/$$.

$subhead Suggestion$$
The $icode Base$$ version of the $code Integer$$ function
might be defined by
$codei%
namespace CppAD {
	inline int Integer(const %Base% &x)
	{	return static_cast<int>(x); }
}
%$$

$head pow$$
$index pow, Base require$$
The type $icode Base$$ must support the syntax
$codei%
	%z% = pow(%x%, %y%)
%$$
which computes $latex z = x^y$$.
The arguments $icode x$$ and $icode y$$ have prototypes
$codei%
	const %Base%& %x%
	const %Base%& %y%
%$$
The return value $icode z$$ has prototype
$codei%
	%Base% %z%
%$$

$head Standard Math Unary$$
$index math, Base require$$
The type $icode Base$$ must support the following 
$cref/standard math unary functions/std_math_ad/$$:
$table
$bold Syntax$$ $cnext $bold Result$$ 
$rnext
$icode%y% = acos(%x%)%$$ $cnext inverse cosine $rnext
$icode%y% = asin(%x%)%$$ $cnext inverse sine   $rnext
$icode%y% = atan(%x%)%$$ $cnext inverse tangent   $rnext
$icode%y% = cos(%x%)%$$  $cnext cosine            $rnext
$icode%y% = cosh(%x%)%$$ $cnext hyperbolic cosine $rnext
$icode%y% = exp(%x%)%$$ $cnext exponential        $rnext
$icode%y% = log(%x%)%$$ $cnext natural logarithm          $rnext
$icode%y% = sin(%x%)%$$ $cnext sine               $rnext
$icode%y% = sinh(%x%)%$$ $cnext hyperbolic sine   $rnext
$icode%y% = sqrt(%x%)%$$ $cnext square root       $rnext
$icode%y% = tan(%x%)%$$  $cnext tangent           
$tend

The argument $icode x$$ has prototype
$codei%
	const %Base%& %x%
%$$
and the result $icode y$$ has prototype
$codei%
	%Base% %y%
%$$

$childtable%
	cppad/local/base_cond_exp.hpp%
	omh/base_identical.omh%
	omh/base_ordered.omh%
	cppad/local/base_float.hpp%
	cppad/local/base_double.hpp%
	cppad/local/base_complex.hpp%
	example/base_adolc.hpp
%$$

$end
*/

// definitions that must come before base implementations
# include <cppad/error_handler.hpp>
# include <cppad/local/define.hpp>
# include <cppad/local/cppad_assert.hpp>

// grouping documentation by feature
# include <cppad/local/base_cond_exp.hpp>

// base cases that come with CppAD
# include <cppad/local/base_float.hpp>
# include <cppad/local/base_double.hpp>
# include <cppad/local/base_complex.hpp>

# endif
