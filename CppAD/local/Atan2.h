# ifndef CppADAtan2Included
# define CppADAtan2Included

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
$begin atan2$$
$spell
	CppAD
	namespace
	std
	atan
	const
$$

$index tan, AD inverse$$
$index inverse, AD tan$$
$index atan2, AD$$

$section The AD Two Argument Inverse Tangent Function$$

$table
$bold Syntax$$ $cnext 
$syntax%%theta% = atan2(%y%, %x%)%$$
$tend

$fend 20$$

$head Purpose$$
Determines an angle $latex \theta \in [ - \pi , + \pi ]$$
such that 
$latex \[
\begin{array}{rcl}
	\sin ( \theta )  & = & y / \sqrt{ x^2 + y^2 }  \\
	\cos ( \theta )  & = & x  / \sqrt{ x^2 + y^2 }
\end{array}
\] $$

$head y$$
The argument $italic y$$ has prototype
$syntax%
	const AD<%Base%> &%y%
%$$ 

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const AD<%Base%> &%x%
%$$ 

$head theta$$
The result $italic theta$$ has prototype
$syntax%
	AD<%Base%> %theta%
%$$

$head Base$$
The type $italic Base$$ must support the use of conditional expressions
$xref/CondExp/$$.
To be specific,
$italic Base$$ can be $code float$$, $code double$$,
or any type in the 
$xref/glossary/AD Type Sequence/AD type sequences/$$ 
above $code float$$ or $code double$$.

$head Standard Types$$
A definition for the $code atan2$$ function is automatically included
in the CppAD namespace for the case where both $italic x$$
and $italic y$$ have the following standard types
$code float$$ and $code double$$.

$head Example$$
$children%
	Example/Atan2.cpp
%$$
The file
$xref/Atan2.cpp/$$
contains an example and test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN CppAD namespace

inline float atan2(float x, float y)
{	return std::atan2(x, y); }

inline double atan2(double x, double y)
{	return std::atan2(x, y); }

// The code below is used as an example by the CondExp documentation.
// BEGIN CondExp
template <class Base>
AD<Base> atan2 (const AD<Base> &y, const AD<Base> &x)
{	AD<Base> alpha;
	AD<Base> beta;
	AD<Base> theta;

	AD<Base> zero = 0;
	AD<Base> pi2  = 2. * atan(1.);
	AD<Base> pi   = 2. * pi2;

	AD<Base> ax = abs(x);
	AD<Base> ay = abs(y);

	// if( ax > ay )
	// 	theta = atan(ay / ax);
	// else	theta = pi2 - atan(ax / ay);
	alpha = atan(ay / ax);
	beta  = pi2 - atan(ax / ay);
	theta = CondExpGt(ax, ay, alpha, beta);         // use of CondExp

	// if( x <= 0 )
	// 	theta = pi - theta;
	theta = CondExpLe(x, zero, pi - theta, theta);  // use of CondExp
	
	// if( y <= 0 )
	// 	theta = - theta;
	theta = CondExpLe(y, zero, -theta, theta);      // use of CondExp

	return theta;
}
// END CondExp 

template <class Base>
inline AD<Base> atan2 (const typename VecAD<Base>::reference &y, 
	const AD<Base> &x)
{	return atan2( y.ADBase() , x ); }

template <class Base>
inline AD<Base> atan2 (const AD<Base> &y, 
	const typename VecAD<Base>::reference &x)
{	return atan2( y , x.ADBase() ); }

template <class Base>
inline AD<Base> atan2 (const typename VecAD<Base>::reference &y, 
	const typename VecAD<Base>::reference &x)
{	return atan2( y.ADBase() , x.ADBase() ); }

} // END CppAD namespace

# endif 
