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
$begin SeqProperty.cpp$$
$spell
	abs
	var
$$

$section ADFun Sequence Properties: Example and Test$$

$index Domain, ADFun$$
$index Range, ADFun$$
$index Parameter, ADFun$$
$index size_var, ADFun$$

$index example, Domain$$
$index example, Range$$
$index example, Parameter$$
$index example, size_var$$

$index test, Domain$$
$index test, Range$$
$index test, Parameter$$
$index test, size_var$$

$code
$verbatim%Example/SeqProperty.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool SeqProperty(void)
{	bool ok = true;
	using CppAD::AD;

	// Use nvar to tracks number of variables in the operation sequence.
	// Start with one for the phantom variable at tape address zero.
	size_t nvar = 1;

	// domain space vector
	size_t n = 2;
	CppADvector< AD<double> > x(n);
	x[0]     = 0.;
	x[1]     = 1.;

	// declare independent variables and start tape recording
	CppAD::Independent(x); 
	nvar    += n;

	AD<double> u = x[0];  // use same variable as x[0]
	AD<double> w = x[1];  // use same variable as x[1]
	w      = w * (u + w); // requires two new variables
	nvar  += 2;

	// range space vector
	size_t m = 3;
	CppADvector< AD<double> > y(m);
	y[0]   = 1.;          // a parameter value   
	y[1]   = u;           // use same variable as x[0]
	y[2]   = w;           // use same variable as w

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// check Domain, Range, Parameter, use_VecAD
	ok &= f.Domain()     == n;
	ok &= f.Range()      == m;
	ok &= f.Parameter(0) == true;
	ok &= f.Parameter(1) == false;
	ok &= f.Parameter(2) == false;
	ok &= f.use_VecAD()  == false;

	// add one for each range component that is a parameter
	size_t i;
	for(i = 0; i < m; i++)
		if( f.Parameter(i) ) nvar++;

	// number of variables corresponding to the sequence
	ok &= f.size_var()   == nvar;

	return ok;
}

// END PROGRAM