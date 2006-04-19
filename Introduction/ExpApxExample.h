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
$begin ExpApxExample$$
$spell
	const
	exp
	iostream
	typename
	bool
	std
	fabs
	adouble
	cout
	endl
	Cpp
$$

$section An Approximation for the Exponential Function$$

$index Exp$$
$index exponential, approximate$$
$index example, Exp$$

$table
$bold Syntax$$    $cnext
$syntax%# include "Exp.h"%$$
$rnext $cnext
$syntax%%y% = exp(%x%, %e%)%$$
$tend

$fend 20$$

$head Definition$$
$index Exp$$
The exponential function satisfies the relation
$latex \[
	\exp (x) = 1 + x^1 / 1 ! + x^2 / 2 ! + \cdots 
\] $$
Suppose that we want to approximate the exponential by truncating the
series at index $latex k ( \varepsilon )  $$ defined as the smallest
positive integer such that $latex \varepsilon \geq x^k / k !$$. 
We can approximate this function using the following function
$latex \[
\begin{array}{rcl}
Exp(x , \varepsilon ) & = & \left\{
\begin{array}{ll}
1. / Exp(-x , \varepsilon ) 
	& {\rm if} \; x < 0 
\\
1 + x^1 / 1 ! + \cdots + x^{k( \varepsilon)} / k( \varepsilon )
	& {\rm otherwise}
\end{array}
\right.
\end{array}
\] $$

$head Specifications$$
$index Exp, specification$$
The C++ 
$xref/TemplateFun//template function/$$ $code Exp$$
computes the value of 
$latex Exp(x, \varepsilon)$$ (defined above).
This function has the following prototype:
$syntax%
	template <typename %Type%>
	%Type% Exp(const %Type% &%x%, const %Type% &%e%)
%$$

$subhead x$$
The argument $italic x$$ specifies the point
at which to approximate the value 
of the exponential function $latex \exp(x)$$.

$subhead e$$
The argument $italic e$$ specifies the accuracy with which
to approximate the exponential function value; i.e.,
it is the value of $latex \varepsilon$$ in the definition of
$latex Exp(x, \varepsilon)$$.

$subhead y$$
The return value $italic y$$ will be set to the value of 
$latex Exp(x, \varepsilon)$$.

$subhead Type$$
It is assumed that the following operations are defined where
$italic u$$ and $italic v$$ are $italic Type$$ objects and $italic i$$
is an $code int$$: 

$table
$bold Operation$$  $cnext $bold Result Type$$ $cnext $bold Description$$
$rnext
$syntax%%Type%(%i%)%$$ 
	$cnext $italic Type$$
	$cnext object with value equal to $italic i$$ 
$rnext
$syntax%%u% > %v%$$
	$cnext $code bool$$
	$cnext true,
	if $italic u$$ greater than $italic v$$, an false otherwise
$rnext
$syntax%%u% = %v%$$
	$cnext 
	$cnext new value of $italic u$$ is value of $italic v$$
$rnext
$syntax%%u% * %v%$$
	$cnext $italic Type$$
	$cnext results in the value of $latex u * v$$
$rnext
$syntax%%u% / %v%$$
	$cnext $italic Type$$
	$cnext result has the value of $latex u / v$$
$rnext
$syntax%%u% + %v%$$
	$cnext $italic Type$$
	$cnext result has the value of $latex u + v$$
$rnext
$syntax%-%u%$$
	$cnext $italic Type$$
	$cnext result has the value of $latex - u$$
$tend

We have used the template parameter $italic Type$$ for
any type that satisfies the conditions above.
This way the implementation above can be used with $italic Type$$ equal to
$code adouble$$ or $code CppAD::AD<double>$$ which will record 
the $xref/ExpSeqExample//operation sequence/$$.
(The type $code adouble$$ uses the ADOLC package and 
$code CppAD::AD<double>$$ uses CppAD package.)
It could also be used with $italic Type$$
equal to $code double$$ 
in order to obtain faster execution 
(not recording the sequence of operations)
when no derivatives are needed.

$head Implementation$$
The following source code 
(in $code Introduction/Approximation.h$$)
implements the specifications above:
$codep */

template <typename Type>
Type Exp(const Type &x, const Type &e)
{	Type a, q, r, s, k;
	r = s = k = Type(1);
	a = x;
	if( Type(0) > x ) a = -x;
	while(r > e) 
	{	q  = a * r;
		r  = q / k;
		s  = s + r;
		k  = k + Type(1);
	}
	if( Type(0) > x ) s = Type(1) / s;
	return s;
}
/* $$

$head Example$$
The following program 
($code Introduction/ExpAppMain.cpp$$)
computes and prints the return value of
$code Exp$$ for the case where $italic Type$$ is double,
$italic x$$ is .5
and $italic e$$ is .1:
$code
$verbatim%Introduction/ExpAppMain.cpp%0%// BEGIN PROGRAM%$$
$$
The following output
($code Introduction/ExpAppMain.out$$)
is generated when this program is run:
$code
$verbatim%Introduction/ExpAppMain.out%$$
$$

$head Exercise$$
$index exercise, template function$$
$index function, template exercise$$
$index template, function exercise$$

$list number$$
Write a routine with the following prototype
$syntax%
	template <typename %Type%>
	%Type% ratio(const %Type% &%x%, const %Type% &%y%)
%$$
that calculates values for the function
$latex f : \R^2 \rightarrow \R$$ defined by
$latex \[
	ratio(x, y) = x / y
\] $$
$lnext
Create and run a main program that computes and prints the return value of 
$code ratio$$ for the case where $italic x$$ is 11, $italic y$$ is 2, and
$italic Type$$ is $code int$$.
(Hint: In C++, integer division truncates the fractional part.)
$lend


$end
*/
