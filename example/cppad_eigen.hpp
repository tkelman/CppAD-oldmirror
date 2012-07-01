/* $Id$ */
# ifndef CPPAD_CPPAD_EIGEN_INCLUDED
# define CPPAD_CPPAD_EIGEN_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_eigen.hpp$$
$spell
	atan
	Num
	acos
	asin
	CppAD
	std::numeric
	enum
	Mul
	Eigen
	cppad.hpp
	namespace
	struct
	typedef
	const
	imag
	sqrt
	exp
	cos
$$
$section Enable Use of Eigen Linear Algebra Package with CppAD$$

$head Syntax$$
$codei%# include <cppad/example/cppad_eigen.hpp>%$$
$children%
	example/eigen_det.cpp
%$$

$head Purpose$$
Enables the use of the 
$href%http://eigen.tuxfamily.org%eigen%$$
linear algebra package with the type $icode%AD<%Base%>%$$.

$head Example$$
THe file $cref eigen_det.cpp$$ contains an example and test of this
include file.
It returns true if it succeeds and false otherwise.

$head Include Files$$
This file $code cppad_eigen.hpp$$ requires the CppAD types to be defined.
In addition, it needs some Eigen definitions to work properly.
It assumes that the corresponding $icode Base$$ type is real; e.g.,
is not $code std::complex<double>$$.
$codep */
# include <cppad/cppad.hpp>
# include <Eigen/Core>
/* $$

$head Traits$$
$codep */
namespace Eigen {
	template <class Base> struct NumTraits< CppAD::AD<Base> >
	{	// type that corresponds to the real part of an AD<Base> value
		typedef CppAD::AD<Base>   Real;
		// type for AD<Base> operations that result in non-integer values
		typedef CppAD::AD<Base>   NonInteger;
		// type for nested value inside an AD<Base> expression tree
		typedef CppAD::AD<Base>   Nested;

		enum {
			// does not support complex Base types
			IsComplex             = 0 ,
			// does not support integer Base types
			IsInteger             = 0 ,
			// only support signed Base types
			IsSigned              = 1 ,
			// must initialize an AD<Base> object
			RequireInitialization = 1 ,
			// computational cost of the corresponding operations
			ReadCost              = 1 ,
			AddCost               = 2 ,
			MulCost               = 2
		};

		// machine epsilon with type of real part of x
		// (use assumption that Base is not complex)
		static CppAD::AD<Base> epsilon(void)
		{	return CppAD::numeric_limits<Base>::epsilon(); }

		// relaxed version of machine epsilon for comparison of different
		// operations that should result in the same value
		static CppAD::AD<Base> dummy_epsilon(void)
		{	return 100. * CppAD::numeric_limits<Base>::epsilon(); }

		// minimum normalized positive value
		static CppAD::AD<Base> lowest(void)
		{	return CppAD::numeric_limits< CppAD::AD<Base> >::min(); }

		// maximum finite value
		static CppAD::AD<Base> highest(void)
		{	return CppAD::numeric_limits< CppAD::AD<Base> >::max(); }

	};
}
namespace CppAD {
		// functions that return references
		template <class Base> const AD<Base>& conj(const AD<Base>& x)
		{	return x; }
		template <class Base> const AD<Base>& real(const AD<Base>& x)
		{	return x; }

		// functions that return values
		template <class Base> AD<Base> imag(const AD<Base>& x)
		{	return CppAD::AD<Base>(0.); }
		template <class Base> AD<Base> abs2(const AD<Base>& x)
		{	return x * x; }
}
/* $$
$end
*/
# endif
