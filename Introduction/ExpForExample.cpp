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
$begin ExpForExample$$
$spell
	Apx
	const
	vname
	vindex
	cpp
	iostream
	cout
	Bradley
	Hyp
	Exp
	fstream
	cassert
	ofstream
	std
	endl
$$

$section Forward Mode$$
$index forward, theory$$
$index theory, forward$$

$head Parameter$$
The independent variables correspond to a subset of 
$xref/ExpApxSeq/Symbols/symbols/$$ in an operation sequence.
If the value of a symbol does not depend on the value of the independent
variables, the symbol is called a parameter.
Otherwise the symbol is called a variable.
A $italic Type$$ object is a 
parameter (variable) if and only if the symbol
corresponding to the current value of the object
is a parameter (variable).


$head Forward Example$$
$index forward, example$$
$index example, forward$$
$index Exp, forward$$
In forward mode we use an 
$xref/ExpApxSeq//operation sequence/$$
in its original order to compute the partial derivative
of all the dependent variables with respect to one independent variable. 
For this example, we use our 
$xref/ExpApx/Implementation/implementation/$$ 
of $latex ExpApx(x, e)$$
with the independent variables being $latex x$$ and $latex e$$.
The operation sequence depends on $italic e$$,
but given the operation sequence,
the individual operations do not depend on $italic e$$.
Thus, the partial derivative with respect to $italic e$$,
of any of the variables in the operation sequence,
is zero.
We also note that the partial derivatives of 
$latex r_0$$, $latex s_0$$, $latex k_0$$, $latex k_1$$,
$latex k_2$$, and $latex k_3$$
with respect to $latex x$$ are also zero
(all of these symbols are parameters).
The forward mode calculations for the variables
(of the partial derivative with respect to $italic x$$)
are:
$center
$table
$bold Symbol$$
	$cnext $bold Value $$
	$cnext $pre  $$ $comment space between colums$$ 
	$cnext $bold Partial$$
	$cnext $bold Value$$
$rnext
$latex a_0 = x $$
	$cnext = 0.5
	$cnext
	$cnext $latex \partial_x x  $$
	$cnext = 1
$rnext
$latex q_1  = r_0 * a_0  $$
	$cnext = 0.5
	$cnext
	$cnext $latex \partial_x r_0  * a_0 + r_0 * \partial_x a_0   $$
	$cnext = 1
$rnext
$latex r_1  = q_1 / k_0  $$
	$cnext = 0.5
	$cnext
	$cnext $latex \partial_x q_1 / k_0 $$
	$cnext = 1
$rnext
$latex s_1  = s_0 + r_1  $$
	$cnext = 1.5
	$cnext
	$cnext $latex \partial_x s_0 + \partial_x r_1 $$
	$cnext = 1
$rnext
$latex q_2 =  r_1 * a_0 $$
	$cnext = 0.25
	$cnext
	$cnext $latex \partial_x r_1  * a_0 + r_1 * \partial_x a_0 $$
	$cnext = 1
$rnext
$latex r_2 = q_2 / k_1 $$
	$cnext = 0.125
	$cnext
	$cnext $latex \partial_x q_2 / k_1 $$
	$cnext = 0.5
$rnext
$latex s_2   = s_1 + r_2 $$
	$cnext = 1.625
	$cnext
	$cnext $latex  \partial_x s_1 + \partial_x r_2 $$
	$cnext = 1.5
$rnext
$latex q_3  = r_2 * a_0 $$
	$cnext = 0.0625
	$cnext
	$cnext $latex \partial_x r_2  * a_0 + r_2 * \partial_x a_0 $$
	$cnext = 0.375
$rnext
$latex r_3  = q_3 / k_2 $$
	$cnext = 0.0208
	$cnext
	$cnext $latex \partial_x q_3 / k_2 $$
	$cnext = 0.125
$rnext
$latex s_3  = s_2 + r_3 $$
	$cnext = 1.6458
	$cnext
	$cnext $latex \partial_x s_2 + \partial_x r_3 $$
	$cnext = 1.625
$tend
$$
Thus forward mode returns
1.625 as the derivative of the return value of the algorithm
for this case.
It is a subtile point (related to continuity of functions)
that there is a neighborhood
of $latex x = 0.5$$ in which the algorithm sequence does not change.
Thus forward mode returns the exact partial with respect to
$latex x$$ of $latex ExpApx(x, .1)$$ at the point $latex x = 0.5$$.

$head Trace$$
$index trace, forward$$
$index forward, trace$$
The program below 
($code Introduction/ExpForExample.cpp$$)
traces the forward mode derivative calculations
above to standard output.
It uses the global variables that are computed and stored by
$xref/ExpApxSeq/$$.
$codep */

# include <iostream>

// global variables computed by ExpApxSeq
extern double a[1], q[4], r[4], s[4], k[4];
extern void ExpApxSeq(double x, double e);


void Print(const char *vname, size_t vindex, double v_x )
{	std::cout << vname << vindex << "_x = " << v_x;
	std::cout << std::endl;
}
int main(void)
{	double a_x[1], q_x[4], r_x[4], s_x[4];

	// compute the global variables 
	double x = .5, e = .1;
	ExpApxSeq(x, e);

	// begin forward mode
	r_x[0] = s_x[0] = 0.;
	a_x[0] = 1.;
	Print("a", 0, a_x[0]);
	size_t j;
	for(j = 1; j <= 2; j++) 
	{	q_x[j] = r_x[j-1] * a[0] + r[j-1] * a_x[0];  // q = r * a
		Print("q", j, q_x[j]);

		r_x[j] = q_x[j] / k[j-1];                    // r = q / k
		Print("r", j, r_x[j]);

		s_x[j]     = s_x[j-1] + r_x[j];              // s = s + r
		Print("s", j, s_x[j]);
	}
	return 0;
}
/* $$

The following output
($code Introduction/ExpForExample.out$$)
is generated when this program is run:
$code
$verbatim%Introduction/ExpForExample.out%$$
$$

$end
*/
