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
$begin ExpRevExample$$
$spell
	Apx
	const
	fname
	findex
	vname
	vindex
	iq
	ir
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
	ia
	df
$$

$section Reverse Mode$$
$index theory, reverse$$
$index reverse, theory$$

$head Reverse Example$$
In reverse mode we use an
$xref/ExpApxSeq//operation sequence/$$
in reverse order to compute the entire derivative of
one dependent variable with respect to all of the independent variables.
For this example we use our
$xref/ExpApx/Implementation/implementation/$$
of $latex ExpApx(x, e)$$
with the independent variables being $italic x$$ and $italic e$$.
The operation sequence symbols
$latex s_0$$, $latex r_0$$, $latex k_0$$, $latex k_1$$,
$latex k_2$$, and $latex k_3$$ are
$xref/Glossary/Parameter/parameters/$$.
Hence they not included in our function definitions below.
In addition, the operation sequence depends on $italic e$$
but the individual operations do not.
Thus, using the operation sequence, 
all of the partial derivatives with respect to $italic e$$ are zero.
Hence it also is left out of our function definitions below.

$head f_0 $$
In reverse mode we choose one of the dependent variable and
compute its derivative with respect to all the independent variables.
For our example, we chose the value returned by $code Exp$$
which is equal to the symbol $latex s_3 $$ in the operation sequence.
We begin with the function $latex f_0 $$ where $latex s_3 $$ 
is both an argument and the value of the function; i.e.,
$latex \[
\begin{array}{rcl}
f_0 ( a_0 , q_1 , r_1 , s_1 , q_2 , r_2 , s_2 , q_3 , r_3 , s_3 ) & = & s_3 \\
\partial_{s3} 
f_0 ( a_0 , q_1 , r_1 , s_1 , q_2 , r_2 , s_2 , q_3 , r_3 , s_3 ) & = & 1   \\
\end{array}
\] $$
and all the other partial derivatives of $latex f_0 $$ are zero.
Note that we have used $latex s3 $$ is place of $latex s_3 $$
as the subscript in $latex \partial_{s3} $$.
This avoids double subscripting which can be hard to read.

$head f_1 $$
We define the function 
$latex f_1 ( a_0 , q_1 , r_1 , s_1 , q_2 , r_2 , s_2 , q_3 , r_3  ) $$
as equal to $latex f_0 $$
except that $latex s_3 $$ is eliminated using 
its functional representation in the operation sequence i.e.
$latex \[
\begin{array}{rcl}
f_1 &  =  &
f_0 [ a_0 , q_1 , r_1 , s_1 , q_2 , r_2 , s_2 , q_3 , r_3 , s_3 ( s_2, r_3 ) ]
\\
s_3 & =   & s_2 + r_3  
\end{array}
\] $$
It follows that 
$latex \[
\begin{array}{rcll}
\partial_{s2} f_1 
& = & \partial_{s2} f_0 + 
	\partial_{s2} s_3 * \partial_{s3} f_0 
& = 1
\\
\partial_{r3} f_1 
& = & \partial_{r3} f_0 + 
	\partial_{r3} s_3 * \partial_{s3} f_0 
& = 1
\end{array}
\] $$
and all the other partial derivatives of $latex f_1 $$ are zero.

$head f_2 $$
We define the function 
$latex f_2 ( a_0 , q_1 , r_1 , s_1 , q_2 , r_2 , s_2 , q_3 ) $$
as equal to $latex f_1 $$
except that $latex r_3 $$ is eliminated using 
its functional representation in the operation sequence i.e.
$latex \[
\begin{array}{rcl}
f_2 &  =  &
f_1 [ a_0 , q_1 , r_1 , s_1 , q_2 , r_2 , s_2 , q_3 , r_3 ( q_3 ) ]
\\
r_3 & =   & q_3 / k_2  
\end{array}
\] $$
Note that $latex k_2 = 3 $$ is a 
$xref/ExpForExample/Parameter/parameter/$$ 
so we have not included it an argument to the function $latex r_3 $$.
It follows that 
$latex \[
\begin{array}{rcll}
\partial_{q3} f_2 
& = & \partial_{q3} f_1 + 
	\partial_{q3} r_3 * \partial_{r3} f_1 
& = 0.3333
\\
\partial_{s2} f_2
& = & \partial_{s2} f_1 
& = 1
\end{array}
\] $$
and all the other partial derivatives of $latex f_2 $$ are zero.

$head f_3 $$
We define the function 
$latex f_3 ( a_0 , q_1 , r_1 , s_1 , q_2 , r_2 , s_2 ) $$
as equal to $latex f_2 $$
except that $latex q_3 $$ is eliminated using 
its functional representation in the operation sequence i.e.
$latex \[
\begin{array}{rcl}
f_3 &  =  &
f_2 [ a_0 , q_1 , r_1 , s_1 , q_2 , r_2 , s_2 ,  q_3 (r_2, a_0 )  ]
\\
q_3  & = & r_2 * a_0
\end{array}
\] $$
Given the independent variable values and the operation sequence,
$latex r_2 =  0.125 $$ and $latex a_0 = 0.5 $$.
It follows that 
$latex \[
\begin{array}{rcll}
\partial_{r2} f_3 
& = & \partial_{r2} f_2 + 
	\partial_{r2} q_3 * \partial_{q3} f_2 
& =  0.1667
\\
\partial_{a0} f_3 
& = & \partial_{a0} f_2 + 
	\partial_{a0} q_3 * \partial_{q3} f_2 
& =  0.0417
\\
\partial_{s2} f_3
& = & \partial_{s2} f_2 
& = 1
\end{array}
\] $$
and all the other partial derivatives of $latex f_3 $$ are zero.

$head f_4 $$
We define the function 
$latex f_4 ( a_0 , q_1 , r_1 , s_1 , q_2 , r_2 ) $$
as equal to $latex f_3 $$
except that $latex s_2 $$ is eliminated using 
its functional representation in the operation sequence i.e.
$latex \[
\begin{array}{rcl}
f_4 &  =  &
f_3 [ a_0 , q_1 , r_1 , s_1 , q_2 , r_2 , s_2 (s_1, r_2 ) ]
\\
s_2  & = &  s_1 + r_2 
\end{array}
\] $$
It follows that
$latex \[
\begin{array}{rcll}
\partial_{s1} f_4 
& = & \partial_{s1} f_3 + 
	\partial_{s1} s_2 * \partial_{s2} f_3 
& =  1
\\
\partial_{r2} f_4 
& = & \partial_{r2} f_3 + 
	\partial_{r2} s_2 * \partial_{s2} f_3 
& =  1.6667
\\
\partial_{a0} f_4 
& = & \partial_{a0} f_3
& =  0.0417
\end{array}
\] $$
and all the other partial derivatives of $latex f_4 $$ are zero.


$head f_5 $$
We define the function 
$latex f_5 ( a_0 , q_1 , r_1 , s_1 , q_2 ) $$
as equal to $latex f_4 $$
except that $latex r_2 $$ is eliminated using 
its functional representation in the operation sequence i.e.
$latex \[
\begin{array}{rcl}
f_5 &  =  &
f_4 [ a_0 , q_1 , r_1 , s_1 , q_2 , r_2 ( q_2 )  ]
\\
r_2 & =   & q_2 / k_1  
\end{array}
\] $$
Note that $latex k_1 = 2 $$ is a 
$xref/ExpForExample/Parameter/parameter/$$ 
so we have not included as an argument to the function $latex r_2 $$.
It follows that 
$latex \[
\begin{array}{rcll}
\partial_{q2} f_5 
& = & \partial_{q2} f_4 + 
	\partial_{q2} r_2 * \partial_{r2} f_4 
& =  0.5833
\\
\partial_{a0} f_5 & & & = 0.0417
\end{array}
\] $$
and all the other partial derivatives of $latex f_5 $$ are zero.

$head f_6 $$
We define the function 
$latex f_6 ( a_0 , q_1 , r_1 , s_1 ) $$
as equal to $latex f_5 $$
except that $latex q_2 $$ is eliminated using 
its functional representation in the operation sequence i.e.
$latex \[
\begin{array}{rcl}
f_6 &  =  &
f_5 [ a_0 , q_1 , r_1 , s_1 , q_2 (r_1, a_0 )  ]
\\
q_2 & =   & r_1 * a_0 
\end{array}
\] $$
Given the independent variable values and the operation sequence,
$latex r_1 =  0.5 $$ and $latex a_0 = 0.5 $$.
It follows that 
$latex \[
\begin{array}{rcll}
\partial_{s1} f_6 & & & = 1 
\\
\partial_{r1} f_6 
& = & \partial_{r1} f_5 + 
	\partial_{r1} q_2 * \partial_{q2} f_5
& =  0.2917
\\
\partial_{q1} f_6 & = & \partial_{q1} f_5 & = 0
\\
\partial_{a0} f_6 
& = & \partial_{a0} f_5 + 
	\partial_{a0} q_2 * \partial_{q2} f_5 
& =  0.3333
\end{array}
\] $$


$head f_7 $$
We define the function 
$latex f_7 ( a_0 , q_1 , r_1 ) $$
as equal to $latex f_6 $$
except that $latex s_1 $$ is eliminated using 
its functional representation in the operation sequence i.e.
$latex \[
\begin{array}{rcl}
f_7 &  =  &
f_6 [ a_0 , q_1 , r_1 , s_1 ( r_1 ) ]
\\
s_1 & =   & s_0 + r_1 
\end{array}
\] $$
Note that $latex s_0 $$ is a 
$xref/ExpForExample/Parameter/parameter/$$ 
so we have not included as an argument to the function $latex s_1 $$.
It follows that 
$latex \[
\begin{array}{rcll}
\partial_{r1} f_7 
& = & \partial_{r1} f_6 + 
	\partial_{r1} s_1 * \partial_{s1} f_6
& =  1.2917
\\
\partial_{q1} f_7 & = & \partial_{q1} f_6 & = 0
\\
\partial_{a0} f_7 
& = & \partial_{a0} f_6 
& =  0.3333
\end{array}
\] $$

$head f_8 $$
We define the function 
$latex f_8 ( a_0 , q_1 ) $$
as equal to $latex f_7 $$
except that $latex r_1 $$ is eliminated using 
its functional representation in the operation sequence i.e.
$latex \[
\begin{array}{rcl}
f_8 &  =  &
f_7 [ a_0 , q_1 , r_1 ( q_1 )]
\\
r_1 & =  & q_1 / k_0 
\end{array}
\] $$
Note that $latex k_0 = 1 $$ is a 
$xref/ExpForExample/Parameter/parameter/$$ 
so we have not included as an argument to the function $latex r_1 $$.
It follows that 
$latex \[
\begin{array}{rcll}
\partial_{q1} f_8 
& = & \partial_{q1} f_7 + 
	\partial_{q1} r_1 * \partial_{r1} f_7
& =  1.2917
\\
\partial_{a0} f_7 
& = & \partial_{a0} f_6 
& =  0.3333
\end{array}
\] $$

$head f_9 $$
We define the function 
$latex f_9 ( a_0 ) $$
as equal to $latex f_8 $$
except that $latex q_1 $$ is eliminated using 
its functional representation in the operation sequence i.e.
$latex \[
\begin{array}{rcl}
f_9 &  =  &
f_8 [ a_0 , q_1 ( a_0 ) ]
\\
q_1 & = & r_0 * a_0
\end{array}
\] $$
Note that $latex r_0 = 1 $$ is a 
$xref/ExpForExample/Parameter/parameter/$$ 
so we have not included as an argument to the function $latex q_1 $$.
It follows that 
$latex \[
\begin{array}{rcll}
\partial_{a0} f_9 
& = & \partial_{a0} f_8 + 
	\partial_{a0} q_1 * \partial_{q1} f_8
& =  1.625
\end{array}
\] $$

Note that for the symbol $latex a_0 $$ is equal to $latex x$$ and  
$latex \partial_{a0} f_9 $$ 
is equal to
the partial with respect to
$latex x$$ of $latex ExpApx(x, .1)$$ at the point $latex x = 0.5$$.
We also note that both reverse and forward mode give the 
same result for this partial derivative.


$head Trace$$
$index trace, reverse$$
$index reverse, trace$$
The program below 
($code Introduction/ExpRevExample.cpp$$)
traces the reverse mode derivative calculations
above to standard output.
It uses the global variables that are computed and stored by
$xref/ExpApxSeq/$$.
It only prints the non-zero partial derivative of $latex f_0$$; i.e.,
its derivative with respect to $latex s_3$$.
For $latex j > 0$$,
it only prints the partial derivatives of
$latex f_j$$ that are different from those of $latex f_{j-1}$$.
Note that the output of this routine includes the partial derivative of
$latex s_3$$ with respect to the parameters
$latex s_0$$, and $latex r_0$$.
This was not included in the analysis above in order
to simplify the presentation.

$codep */

# include <iostream>

// global variables computed by ExpApxSeq
extern double a[1], q[4], r[4], s[4], k[4];
extern void ExpApxSeq(double x, double e);

void Print(const char *fname, size_t findex, const char *vname, size_t vindex, 
	double f_v )
{	std::cout << fname << findex << "_" << vname << vindex << " = " << f_v;
	std::cout << std::endl;
}
int main(void)
{	// ordering of arguments is: 
	// a0, q0, q1, q2, q3, r0, r1, r2, r3, s0, s1, s2, s3 
	// corresponding index offsets for each of the parameters
	size_t ia = 0, iq = 1, ir = 5, is = 9;

	// compute the global variables 
	double x = .5, e = .1;
	ExpApxSeq(x, e);

	// initial all partial derivatives as zero
	double df[13];
	size_t j;
	for(j = 0; j < 13; j++)
		df[j] = 0.;

	// f0
	df[is+3] = 1.;
	Print("f", 0, "s", 3, df[is + 3]);

	for(j = 3; j >= 1; j--) 
	{	// remove s[j] = s[j-1] + r[j] from the function
		df[is + j-1] += df[is + j];
		df[ir + j]   += df[is + j];
		Print("f", 10-3*j, "s", j-1, df[is + j-1]);
		Print("f", 10-3*j, "r", j,   df[ir + j]);

		// remove r[j] = q[j] / k[j-1] from the function
		df[iq + j]   += df[ir + j] / k[j-1];
		Print("f", 11-3*j, "q", j,   df[iq + j]);

		// remove q[j] = r[j-1] * a[0] from the function
		df[ir + j-1] += df[iq + j] * a[0];
		df[ia + 0]   += df[iq + j] * r[j-1];
		Print("f", 12-3*j, "r", j-1, df[ir + j-1]);
		Print("f", 12-3*j, "a", 0,   df[ia + 0]);

	}
	return 0;
}
/* $$

The following output
($code Introduction/ExpRevExample.out$$)
is generated when this program is run:
$code
$verbatim%Introduction/ExpRevExample.out%$$
$$

$end
*/
