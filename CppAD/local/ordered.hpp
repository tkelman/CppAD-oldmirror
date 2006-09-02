# ifndef CppADOrderedIncluded
# define CppADOrderedIncluded

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
---------------------------------------------------------------------------
$begin GreaterThanZero$$ $comment CppAD Developer Documentation$$
$spell
	inline
	const
	bool
$$

$index >$$
$index GreaterThanZero$$
$index positive$$
$section Check if a Value is Greater Than Zero$$

$head Syntax$$
$syntax%inline bool GreaterThanZero(const %Type% &%x%)%$$

$head Description$$
If $italic Type$$ is $syntax%AD<%Base%>%$$ for some $italic Base$$,
$italic x$$ is greater than zero if and only if
$syntax%%x%.value%$$ is greater than zero.
If $italic Type$$ is not $syntax%AD<%Base%>%$$ for some $italic Base$$,
$italic x$$ is greater than zero if and only if
$syntax%
	%x% > 0.
%$$

$end 
---------------------------------------------------------------------------
$begin GreaterThanOrZero$$ $comment CppAD Developer Documentation$$
$spell
	inline
	const
	bool
$$

$index >=$$
$index GreaterThanOrZero$$
$index positive$$
$section Check if a Value is Greater Than Or Equal Zero$$

$head Syntax$$
$syntax%inline bool GreaterThanZero(const %Type% &%x%)%$$

$head Description$$
If $italic Type$$ is $syntax%AD<%Base%>%$$ for some $italic Base$$,
$italic x$$ is greater than or equal zero if and only if
$syntax%%x%.value%$$ is greater than zero.
If $italic Type$$ is not $syntax%AD<%Base%>%$$ for some $italic Base$$,
$italic x$$ is greater than zero if and only if
$syntax%
	%x% >= 0.
%$$

$end 
---------------------------------------------------------------------------
$begin LessThanZero$$ $comment CppAD Developer Documentation$$
$spell
	inline
	const
	bool
$$

$index >$$
$index LessThanZero$$
$index negative$$
$section Check if a Value is Less Than Zero$$

$head Syntax$$
$syntax%inline bool LessThanZero(const %Type% &%x%)%$$

$head Description$$
If $italic Type$$ is $syntax%AD<%Base%>%$$ for some $italic Base$$,
$italic x$$ is less than zero if and only if
$syntax%%x%.value%$$ is less than zero.
If $italic Type$$ is not $syntax%AD<%Base%>%$$ for some $italic Base$$,
$italic x$$ is less than zero if and only if
$syntax%
	%x% < 0.
%$$

$end 
---------------------------------------------------------------------------
$begin LessThanOrZero$$ $comment CppAD Developer Documentation$$
$spell
	inline
	const
	bool
$$

$index >=$$
$index LessThanOrZero$$
$index negative$$
$section Check if a Value is Less Than Or Equal Zero$$

$head Syntax$$
$syntax%inline bool LessThanOrZero(const %Type% &%x%)%$$

$head Description$$
If $italic Type$$ is $syntax%AD<%Base%>%$$ for some $italic Base$$,
$italic x$$ is less than or equal zero if and only if
$syntax%%x%.value%$$ is less than zero.
If $italic Type$$ is not $syntax%AD<%Base%>%$$ for some $italic Base$$,
$italic x$$ is less than or equal zero if and only if
$syntax%
	%x% <= 0.
%$$

$end 
------------------------------------------------------------------------------
*/


namespace CppAD { // BEGIN CppAD namespace

// GreaterThanZero ------------------------------------------------------------
inline bool GreaterThanZero(float x)
{	return x > 0.; }

inline bool GreaterThanZero(double x)
{	return x > 0.; }

inline bool GreaterThanZero(std::complex<float> x)
{	CppADUsageError(
		0,
		"Attempt to use std::complex<float> as an ordered type"
	);
	return false;
}

inline bool GreaterThanZero(std::complex<double> x)
{	CppADUsageError(
		0,
		"Attempt to use std::complex<double> as an ordered type"
	);
	return false;
}

template <class Base>
inline bool GreaterThanZero(const AD<Base> &x)
{	return GreaterThanZero(x.value); }


// GreaterThanOrZero ---------------------------------------------------------
inline bool GreaterThanOrZero(float x)
{	return x >= 0.; }

inline bool GreaterThanOrZero(double x)
{	return x >= 0.; }

inline bool GreaterThanOrZero(std::complex<float> x)
{	CppADUsageError(
		0,
		"Attempt to use std::complex<float> as an ordered type"
	);
	return false;
}

inline bool GreaterThanOrZero(std::complex<double> x)
{	CppADUsageError(
		0,
		"Attempt to use std::complex<double> as an ordered type"
	);
	return false;
}

template <class Base>
inline bool GreaterThanOrZero(const AD<Base> &x)
{	return GreaterThanOrZero(x.value); }


// LessThanZero  ------------------------------------------------------------

inline bool LessThanZero(float x)
{	return x < 0.; }

inline bool LessThanZero(double x)
{	return x < 0.; }

inline bool LessThanZero(std::complex<float> x)
{	CppADUsageError(
		0,
		"Attempt to use std::complex<float> as an ordered type"
	);
	return false;
}

inline bool LessThanZero(std::complex<double> x)
{	CppADUsageError(
		0,
		"Attempt to use std::complex<double> as an ordered type"
	);
	return false;
}

template <class Base>
inline bool LessThanZero(const AD<Base> &x)
{	return LessThanZero(x.value); }

// LessThanOrZero  ------------------------------------------------------------

inline bool LessThanOrZero(float x)
{	return x <= 0.; }

inline bool LessThanOrZero(double x)
{	return x <= 0.; }

inline bool LessThanOrZero(std::complex<float> x)
{	CppADUsageError(
		0,
		"Attempt to use std::complex<float> as an ordered type"
	);
	return false;
}

inline bool LessThanOrZero(std::complex<double> x)
{	CppADUsageError(
		0,
		"Attempt to use std::complex<double> as an ordered type"
	);
	return false;
}

template <class Base>
inline bool LessThanOrZero(const AD<Base> &x)
{	return LessThanOrZero(x.value); }

} // END CppAD namespace

# endif

