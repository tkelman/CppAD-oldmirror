// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

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
// END SHORT COPYRIGHT

/*
Old CompareChange example and test, now just used for validation testing
*/

# include <CppAD/CppAD.h>

bool CompareChange(void)
{	bool ok = true;

	using namespace CppAD;

	// ------------------------------- < ----------------------------

	// create independent variables
	CppADvector< AD<double> > X(2);
	X[0] = 3.;
	X[1] = 4.;
	Independent(X);

	// create dependent variables
	CppADvector< AD<double> > Y(6);

	// CondExp would never require retaping 
	if( X[0] < X[1] )      // True variable < variable
		Y[0] = X[0];
	else	Y[0] = X[1];
	if( X[1] < X[0] )      // False variable < variable
		Y[1] = X[0];
	else	Y[1] = X[1];
	if( 3.5  < X[1] )      // True parameter < variable
		Y[2] = X[0];
	else	Y[2] = X[1];
	if( 3.5  < X[0] )      // False parameter < variable
		Y[3] = X[0];
	else	Y[3] = X[1];
	if( X[0] < 4.   )      // True variable < parameter
		Y[4] = X[0];
	else	Y[4] = X[1];
	if( X[1] < 4.   )      // False variable < parameter
		Y[5] = X[0];
	else	Y[5] = X[1];

	// f : X -> Y
	ADFun<double> *f;
	f = new ADFun<double>(X, Y);

	// new argument value
	CppADvector<double> x( X.size() );
	x[0] = 4.;
	x[1] = 3.;

	// evaluate the function at new argument
	CppADvector<double> y( Y.size() );
	y = f->Forward(0, x);

	// check results
	ok &= (y[0] == x[0]);           // this is what the was taped
	ok &= (y[1] == x[1]);
	ok &= (y[2] == x[0]);
	ok &= (y[3] == x[1]);
	ok &= (y[4] == x[0]);
	ok &= (y[5] == x[1]);
	ok &= (f->CompareChange() == 6); // all comparisions have changed

	// done with this function
	delete f;

	// ------------------------------- > ----------------------------
	// create independent variables
	Independent(X);

	if( X[0] > X[1] )      // False variable > variable
		Y[0] = X[0];
	else	Y[0] = X[1];
	if( X[1] > X[0] )      // True variable > variable
		Y[1] = X[0];
	else	Y[1] = X[1];
	if( 3.5  > X[1] )      // False parameter > variable
		Y[2] = X[0];
	else	Y[2] = X[1];
	if( 3.5  > X[0] )      // True parameter > variable
		Y[3] = X[0];
	else	Y[3] = X[1];
	if( X[0] > 3.   )      // False variable > parameter
		Y[4] = X[0];
	else	Y[4] = X[1];
	if( X[1] > 3.   )      // True variable > parameter
		Y[5] = X[0];
	else	Y[5] = X[1];

	// f : X -> Y
	f = new ADFun<double> (X, Y);

	// evaluate the function at new argument
	y = f->Forward(0, x);

	// check results
	ok &= (y[0] == x[1]);           // this is what the was taped
	ok &= (y[1] == x[0]);
	ok &= (y[2] == x[1]);
	ok &= (y[3] == x[0]);
	ok &= (y[4] == x[1]);
	ok &= (y[5] == x[0]);
	ok &= (f->CompareChange() == 6); // all comparisions have changed

	// done with this function
	delete f;

	// ------------------------------- <= ----------------------------
	// create independent variables
	Independent(X);

	if( X[0] <= X[1] )      // True variable <= variable
		Y[0] = X[0];
	else	Y[0] = X[1];
	if( X[1] <= X[0] )      // False variable <= variable
		Y[1] = X[0];
	else	Y[1] = X[1];
	if( 4.  <= X[1] )       // True parameter <= variable
		Y[2] = X[0];
	else	Y[2] = X[1];
	if( 4.  <= X[0] )       // False parameter <= variable
		Y[3] = X[0];
	else	Y[3] = X[1];
	if( X[0] <= 3.5   )     // True variable <= parameter
		Y[4] = X[0];
	else	Y[4] = X[1];
	if( X[1] <= 3.5  )      // False variable <= parameter
		Y[5] = X[0];
	else	Y[5] = X[1];

	// f : X -> Y
	f = new ADFun<double> (X, Y);

	// evaluate the function at new argument
	y = f->Forward(0, x);

	// check results
	ok &= (y[0] == x[0]);           // this is what the was taped
	ok &= (y[1] == x[1]);
	ok &= (y[2] == x[0]);
	ok &= (y[3] == x[1]);
	ok &= (y[4] == x[0]);
	ok &= (y[5] == x[1]);
	ok &= (f->CompareChange() == 6); // all comparisions have changed

	// done with this function
	delete f;


	// ------------------------------- >= ----------------------------
	// create independent variables
	Independent(X);

	if( X[0] >= X[1] )      // False variable >= variable
		Y[0] = X[0];
	else	Y[0] = X[1];
	if( X[1] >= X[0] )      // True variable >= variable
		Y[1] = X[0];
	else	Y[1] = X[1];
	if( 3.5  >= X[1] )      // False parameter >= variable
		Y[2] = X[0];
	else	Y[2] = X[1];
	if( 3.5  >= X[0] )      // True parameter >= variable
		Y[3] = X[0];
	else	Y[3] = X[1];
	if( X[0] >= 4.   )      // False variable >= parameter
		Y[4] = X[0];
	else	Y[4] = X[1];
	if( X[1] >= 4.   )      // True variable >= parameter
		Y[5] = X[0];
	else	Y[5] = X[1];

	// f : X -> Y
	f = new ADFun<double> (X, Y);

	// evaluate the function at new argument
	y = f->Forward(0, x);

	// check results
	ok &= (y[0] == x[1]);           // this is what the was taped
	ok &= (y[1] == x[0]);
	ok &= (y[2] == x[1]);
	ok &= (y[3] == x[0]);
	ok &= (y[4] == x[1]);
	ok &= (y[5] == x[0]);
	ok &= (f->CompareChange() == 6); // all comparisions have changed

	// done with this function
	delete f;

	// ------------------------------- == ----------------------------
	// create independent variables
	Independent(X);

	if( X[0] == X[1] )      // False variable == variable
		Y[0] = X[0];
	else	Y[0] = X[1];
	if( X[0] == X[0] )      // True variable == variable
		Y[1] = X[0];
	else	Y[1] = X[1];
	if( 3.  == X[1] )       // False parameter == variable
		Y[2] = X[0];
	else	Y[2] = X[1];
	if( 3.  == X[0] )       // True parameter == variable
		Y[3] = X[0];
	else	Y[3] = X[1];
	if( X[0] == 4.   )      // False variable == parameter
		Y[4] = X[0];
	else	Y[4] = X[1];
	if( X[1] == 4.   )      // True variable == parameter
		Y[5] = X[0];
	else	Y[5] = X[1];

	// f : X -> Y
	f = new ADFun<double> (X, Y);

	// evaluate the function at new argument
	y = f->Forward(0, x);

	// check results
	ok &= (y[0] == x[1]);           // this is what the was taped
	ok &= (y[1] == x[0]);
	ok &= (y[2] == x[1]);
	ok &= (y[3] == x[0]);
	ok &= (y[4] == x[1]);
	ok &= (y[5] == x[0]);
	// the first two comparisions do not change
	ok &= (f->CompareChange() == 4); 

	// done with this function
	delete f;

	// ------------------------------- != ----------------------------
	// create independent variables
	Independent(X);

	if( X[0] != X[1] )      // True variable != variable
		Y[0] = X[0];
	else	Y[0] = X[1];
	if( X[0] != X[0] )      // False variable != variable
		Y[1] = X[0];
	else	Y[1] = X[1];
	if( 3.  != X[1] )       // True parameter != variable
		Y[2] = X[0];
	else	Y[2] = X[1];
	if( 3.  != X[0] )       // False parameter != variable
		Y[3] = X[0];
	else	Y[3] = X[1];
	if( X[0] != 4.   )      // True variable != parameter
		Y[4] = X[0];
	else	Y[4] = X[1];
	if( X[1] != 4.   )      // False variable != parameter
		Y[5] = X[0];
	else	Y[5] = X[1];

	// f : X -> Y
	f = new ADFun<double> (X, Y);

	// evaluate the function at new argument
	y = f->Forward(0, x);

	// check results
	ok &= (y[0] == x[0]);           // this is what the was taped
	ok &= (y[1] == x[1]);
	ok &= (y[2] == x[0]);
	ok &= (y[3] == x[1]);
	ok &= (y[4] == x[0]);
	ok &= (y[5] == x[1]);
	// the first two comparisions do not change
	ok &= (f->CompareChange() == 4); 

	// done with this function
	delete f;

	return ok;
}
