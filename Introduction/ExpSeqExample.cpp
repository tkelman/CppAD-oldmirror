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
$begin ExpSeqExample$$
$spell
	adouble
	cpp
	Bradley
	typename
	sqrt
	Hyp
	fstream
	ofstream
	iostream
	cassert
	std
	cout
	endl
	Exp
	const
	vname
	vindex
$$

$section Operation Sequence$$
$index algorithm, operation sequence$$
$index operation, sequence$$
$index sequence, operation$$

$head Definition$$
A $italic Type$$ operation sequence is the sequence
of operations with operands of the specified type.
Given an C++ algorithm and its inputs,
there is a corresponding operation sequence for each type. 
If the type in question is clear, 
we often drop it and just refer to the operation sequence.

$head Exp Operation Sequence$$
Suppose that we use our
$xref/ExpApxExample/Implementation/implementation/$$
of $latex Exp(x, e)$$ 
with $latex x$$ is equal to .5
and $latex e$$ is equal to .1. 
We use the notation $latex v_j$$ for the value assigned
to the variable $latex v$$ in the $th j$$ execution of loop in the algorithm.
The initialization section always sets
$latex r_0$$,
$latex s_0$$,
and
$latex k_0$$ equal to one.
Hence the $italic Type$$ values 
$latex r_0$$, $latex s_0$$ and $latex k_0$$ are
$xref/Glossary/Parameter/parameters/$$; i.e.,
they do not depend on the independent variables ($latex x$$ or $italic e$$).
Below, is the operation sequence together with the corresponding
code, symbols, and values:
$center
$table
$bold Code$$
	$cnext $bold Symbol$$
	$cnext $bold Operation$$
	$cnext $bold Value$$
	$rnext
$code a = x$$
	$cnext $latex a_0 $$
	$cnext $latex = x $$ 
	$cnext = 0.5      
$rnext
$code q  = a * r$$
	$cnext $latex q_1 $$
	$cnext $latex = r_0 * a_0 $$ 
	$cnext = 0.5      
$rnext
$code r  = q / k$$
	$cnext $latex r_1 $$
	$cnext $latex = q_1 / k_0 $$ 
	$cnext = 0.5      
$rnext
$code s  = s + r$$
	$cnext $latex s_1 $$
	$cnext $latex = s_0 + r_1 $$ 
	$cnext = 1.5      
$rnext
$code k  = k + Type(1)$$
	$cnext $latex k_1 $$
	$cnext $latex = k_0 + 1 $$ 
	$cnext = 2        
$rnext
$code q  = a * r$$
	$cnext $latex q_2 $$
	$cnext $latex = r_1 * a_0 $$ 
	$cnext = 0.25     
$rnext
$code r  = q / k$$
	$cnext $latex r_2 $$
	$cnext $latex = q_2 / k_1 $$ 
	$cnext = 0.125    
$rnext
$code s  = s + r$$
	$cnext $latex s_2 $$
	$cnext $latex = s_1 + r_2 $$ 
	$cnext = 1.625    
$rnext
$code k  = k + Type(1)$$
	$cnext $latex k_2 $$
	$cnext $latex = k_1 + 1 $$ 
	$cnext = 3        
$rnext
$code q  = a * r$$
	$cnext $latex q_3 $$
	$cnext $latex = r_2 * a_0 $$ 
	$cnext = 0.0625   
$rnext
$code r  = q / k$$
	$cnext $latex r_3 $$
	$cnext $latex = q_3 / k_2 $$ 
	$cnext = 0.0208   
$rnext
$code s  = s + r$$
	$cnext $latex s_3 $$
	$cnext $latex = s_2 + r_3 $$ 
	$cnext = 1.6458   
$rnext
$code k  = k + Type(1)$$
	$cnext $latex k_3 $$
	$cnext $latex = k_2 + 1 $$ 
	$cnext = 4  
$tend
$$
The return value of the algorithm for this case is
$latex s_3$$ which is equal to 1.6458.
$pre

$$
Note that each operation above has a corresponding binary function.
If $latex x$$ were negative,
or if $latex e$$ were a much smaller or much larger value,
then the operation sequence would be different; i.e.,
the operations above only correspond to the our algorithm
for values of $latex x$$ and $latex e$$ within a certain range.

$head Operation Symbol$$
$index operation, symbol$$
$index symbol, operation$$
The symbols for an operation sequence is a
sequence of names with a unique name corresponding to each operation.
For example, the symbols for the operation sequence above are:
$latex a_0$$ and
$latex q_j$$,
$latex r_j$$,
$latex s_j$$  where $latex j = 1 , 2 , 3$$.
In an abuse of notation, we use a symbol for both the
function and the value of the function corresponding to an operation. 
Note each value that a $italic Type$$ object assumes
(in a C++ program)
has a corresponding unique symbol in the 
corresponding $italic Type$$ operation sequence.

$head Trace$$
$index trace, Exp$$
$index ExpSeqExample$$
The following routine 
($code Introduction/ExpSeqExample.cpp$$)
traces this operation sequence in a file called $code ExpSeqExample.out$$
and stores the corresponding values as global variables:
$codep */

# include <fstream>                  // used to write ExpSeqExample.out
double a[1], q[4], r[4], s[4], k[4]; // global variables


void Print(std::ofstream &file, const char *vname, size_t vindex, double v)
{	file << vname << vindex << " = " << v;
	file << std::endl;
}
void ExpSeqExample(double x, double e)
{	r[0] = s[0] = k[0] = 1.;

	// open the output file
	std::ofstream file( "ExpSeqExample.out" );

	a[0] = x;
	Print(file, "a", 0, a[0]);

	size_t j;
	for(j = 1; j <= 3; j++) 
	{	q[j] = r[j-1] * a[0];
		Print(file, "q", j, q[j]);

		r[j] = q[j] / k[j-1];
		Print(file, "r", j, r[j]);

		s[j]     = s[j-1] + r[j];
		Print(file, "s", j, s[j]);

		k[j]     = k[j-1] + 1.;
		Print(file, "k", j, k[j]);
	}
}
/* $$
The following program 
($code Introduction/ExpSeqMain.cpp$$)
executes the routine above with 
$latex x$$ is equal to .5 and $latex e$$ is equal to .1:
$code
$verbatim%Introduction/ExpSeqMain.cpp%0%// BEGIN PROGRAM%$$
$$
The following trace
($code ExpSeqExample.out$$)
is generated when this program is run:
$code
$verbatim%Introduction/ExpSeqExample.out%$$
$$

$head Exercise$$
$index exercise, operation sequence$$
$index operation, sequence exercise$$
$index sequence, operation exercise$$

$list number$$
Determine the $italic Type$$ operation sequence corresponding the following
routine
$codep
	template <typename Type>
	Type hypotenuse(Type x, Type y)
	{	Type a = x * x;
		Type b = y * y;
		Type c = a + b;
		return sqrt(c);
	}
$$
when the argument $latex x$$ is equal to 3. 
and $latex y$$ is equal to 4.
$lnext
Create a routine that stores the algorithm sequence values
as global variables and outputs them to a file called Hypotenuse.out.
$lnext
Create and run a main program that executes this routine with 
$italic x$$ equal to 3. and $italic y$$ equal to 4.
$lnext
Determine the hypotenuse operation sequence when $italic x$$ is 1
and $italic y$$ is 1.
$lend

$end
*/
