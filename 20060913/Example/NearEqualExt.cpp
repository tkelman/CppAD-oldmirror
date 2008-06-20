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
$begin NearEqualExt.cpp$$
$spell
	cpp
	abs
o	Microsoft
$$

$section Compare AD with Base Objects: Example and Test$$
$index NearEqualExt, example$$
$index example, NearEqualExt$$
$index test, NearEqualExt$$

$code
$verbatim%Example/NearEqualExt.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <complex>

bool NearEqualExt(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// double 
	double x    = 1.00000;
	double y    = 1.00001;
	double a    =  .00005;
	double r    =  .00005;
	double zero = 0.; 

	// AD<double> 
	AD<double> ax(x);
	AD<double> ay(y);

	ok &= NearEqual(ax, ay, zero, a);
	ok &= NearEqual(ax, y,  r, zero);
	ok &= NearEqual(x, ay,  r,    a);

	// std::complex<double> 
	AD<double> cx(x);
	AD<double> cy(y);

	// AD< std::complex<double> > 
	AD<double> acx(x);
	AD<double> acy(y);

	ok &= NearEqual(acx, acy, zero, a);
	ok &= NearEqual(acx,  cy, r, zero);
	ok &= NearEqual(acx,   y, r,    a);
	ok &= NearEqual( cx, acy, r,    a);
	ok &= NearEqual(  x, acy, r,    a);

	return ok;
}

// END PROGRAM
