# ifndef CppADExpApxIncluded
# define CppADExpApxIncluded

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */
/*
$begin ExpApx.hpp$$
$spell
	ExpApx
	const
	Exp
$$

$section An Example Exponential Approximation Source Code$$
$codep */
template <class Type>
Type ExpApx(const Type &x, const Type &e)
{	Type a, q, r, s, k;           // declare local variables
	r = s = k = Type(1);          // initialize as equal to one
	a = x;                        // a = |x|
	if( Type(0) > x ) a = -x;
	// while the previous term included in the summation is greater than e
	while(r > e)
	{	q  = a * r;           // q = |x|^k / (k-1)!
		r  = q / k;           // r = |x|^k / k!
		s  = s + r;           // s = 1 + |x| / 1! + ... + |x|^k / k!
		k  = k + Type(1);
	}
	// In the case where x is negative, use exp(x) = 1 / exp(-|x|)
	if( Type(0) > x ) s = Type(1) / s;
	return s;
}
/* $$
$end
*/

# endif
