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


# include <CppAD/CppAD.h>


bool RevSparseHes(void)
{	bool ok = true;
	using namespace CppAD;

	// dimension of the domain space
	size_t n = 8; 

	// dimension of the range space
	size_t m = 1;

	// temporary indices
	size_t i, j;

	// initialize check values to false
	CppADvector<bool> Check(n * n);
	for(j = 0; j < n * n; j++)
		Check[j] = false;

	// independent variable vector 
	CppADvector< AD<double> > X(n);
	for(j = 0; j < n; j++)
		X[j] = AD<double>(j);
	Independent(X);

	// accumulate sum here
	AD<double> sum;

	sum  = X[0] - X[1];

	sum += X[2] * X[3];
	Check[2 * n + 3] = Check[3 * n + 2] = true;

	sum += X[4] / X[5];
	Check[4 * n + 5] = Check[5 * n + 4] = Check[5 * n + 5] = true;

	sum += CondExpLt(X[1], X[2], sin(X[6]), cos(X[7]) );
	if( X[1] < X[2] )
		Check[6 * n + 6] = true;
	else	Check[7 * n + 7] = true;
	

	// compute product of elements in X
	CppADvector< AD<double> > Y(m);
	Y[0] = sum;

	// create function object F : X -> Y
	ADFun<double> F(X, Y);

	// sparsity pattern for the identity function U(x) = x
	CppADvector<bool> Px(n * n);
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
			Px[ i * n + j ] = false;
		Px[ i * n + i ] = true;
	}

	// compute sparsity pattern for Jacobian of F(U(x))
	F.ForSparseJac(n, Px);

	// compute sparsity pattern for Hessian of F_0 ( U(x) ) 
	CppADvector<bool> Py(m);
	for(i = 0; i < m; i++)
		Py[i] = false;
	Py[0] = true;
	CppADvector<bool> Pxx(n * n);
	Pxx = F.RevSparseHes(n, Py);

	for(j = 0; j < n * n; j++)
		ok &= (Pxx[j] == Check[j]);

	return ok;
}
