# ifndef CppADOdeGearIncluded
# define CppADOdeGearIncluded

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
$begin OdeGear$$
$latex \newcommand{\R}{{\bf R}}$$
$spell
	Jan
	bool
	const
	CppAD
	dep
$$

$index OdeGear$$
$index Ode, Gear$$
$index Gear, Ode$$
$index stiff, Ode$$
$index differential, equation$$
$index equation, differential$$
 
$section An Arbitrary Order Gear Method$$

$table
$bold Syntax$$ 
$cnext $code # include <CppAD/OdeGear.h>$$
$rnext $cnext
$syntax%OdeGear(%F%, %m%, %n%, %T%, %X%)%$$
$tend

$fend 25$$

$head Description$$
This routine applies
$xref/OdeGear/Gear's Method/Gear's Method/$$
to solve an explicit set of ordinary differential equations.
We are given 
$latex f : \R \times \R^n \rightarrow \R^n$$ be a smooth function.
This routine solves the following initial value problem
$latex \[
\begin{array}{rcl}
	x( t_{m-1} )  & = & x^0    \\
	x \prime (t)  & = & f[t , x(t)] 
\end{array}
\] $$
for the value of $latex x( t_m )$$.
If your set of  ordinary differential equations are not stiff
an explicit method may be better (perhaps $xref/Runge45/$$.)

$head Include$$
The file $code CppAD/OdeGear.h$$ is included by $code CppAD/CppAD.h$$
but it can also be included separately with out the rest of 
the $code CppAD$$ routines.

$head Fun$$
The class $italic Fun$$ 
and the object $italic F$$ satisfy the prototype
$syntax%
	%Fun% &%F%
%$$
This must support the following set of calls
$syntax%
	%F%.Ode(%t%, %x%, %f%)
	%F%.Ode_dep(%t%, %x%, %f_x%)
%$$

$subhead t$$
The argument $italic t$$ has prototype
$syntax%
	const %Scalar% &%t%
%$$
(see description of $xref/OdeGear/Scalar/Scalar/$$ below). 

$subhead x$$
The argument $italic x$$ has prototype
$syntax%
	const %Vector% &%x%
%$$
and has size $italic n$$
(see description of $xref/OdeGear/Vector/Vector/$$ below). 

$subhead f$$
The argument $italic f$$ to $syntax%%F%.Ode%$$ has prototype
$syntax%
	%Vector% &%f%
%$$
On input and output, $italic f$$ is a vector of size $italic n$$
and the input values of the elements of $italic f$$ do not matter.
On output,
$italic f$$ is set equal to $latex f(t, x)$$
(see $italic f(t, x)$$ in $xref/OdeGear/Description/Description/$$). 

$subhead f_x$$
The argument $italic f_x$$ has prototype
$syntax%
	%Vector% &%f_x%
%$$
On input and output, $italic f_x$$ is a vector of size $latex n * n$$
and the input values of the elements of $italic f_x$$ do not matter.
On output, 
$latex \[
	f_x [i * n + j] = \partial_{x(j)} f_i ( t , x )
\] $$ 

$subhead Warning$$
The arguments $italic f$$, and $italic f_x$$
must have a call by reference in their prototypes; i.e.,
do not forget the $code &$$ in the prototype for 
$italic f$$ and $italic f_x$$.

$head m$$
The argument $italic m$$ has prototype
$syntax%
	size_t %m%
%$$
It specifies the order (highest power of $latex t$$) 
used to represent the function $latex x(t)$$ in the multi-step method. 
Upon return from $code OdeGear$$,
the $th i$$ component of the polynomial is defined by
$latex \[
	p_i ( t_j ) = X[ j * n + i ]
\] $$
for $latex j = 0 , \ldots , m$$ (where $latex 0 \leq i < n$$).
The value of $latex m$$ must be greater than or equal one.

$head n$$
The argument $italic n$$ has prototype
$syntax%
	size_t %n%
%$$
It specifies the range space dimesion of the 
vector valued function $latex x(t)$$.

$head T$$
The argument $italic T$$ has prototype
$syntax%
	const %Vector% &%T%
%$$
and size equal to $latex m+1$$.
For $latex j = 0 , \ldots m$$, $latex T[j]$$ is the time 
corresponding to time corresponding 
to a previous point in the multi-step method.
The value $latex T[m]$$ is the time 
of the next point in the multi-step method.
Above and below we often use the shorthand $latex t_j$$ for $latex T[j]$$.

$head X$$
The argument $italic X$$ has the prototype
$syntax%
	%Vector% &%X%
%$$
and the size of $italic X$$ is equal to
$latex (m+1) * n$$.
On input to $code OdeGear$$,
for $latex j = 0 , \ldots , m-1$$, and
$latex i = 0 , \ldots , n-1$$ 
$latex \[
	X[ j * n + i ] = x_i ( t_j )
\] $$
Upon return from $code OdeGear$$,
for $latex i = 0 , \ldots , n-1$$ 
$latex \[
	X[ m * n + i ] \approx x_i ( t_m ) 
\] $$

$head Scalar$$
The type $italic Scalar$$ must satisfy the conditions
for a $xref/NumericType/$$ type.
The routine $xref/CheckNumericType/$$ will generate an error message
if this is not the case.
In addition, the following operations must be defined for 
$italic Scalar$$ objects $italic a$$ and $italic b$$:

$table
$bold Operation$$ $cnext $bold Description$$  $rnext
$syntax%%a% < %b%$$ $cnext
	less than operator (returns a $code bool$$ object)
$tend

$head Vector$$
The type $italic Vector$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type Scalar/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head Example$$
$comment%
	Example/OdeGear.cpp
%$$
The file
$xref/OdeGear.cpp/$$
contains an example and test a test of using this routine.
It returns true if it succeeds and false otherwise.

$head Source Code$$
The source code for this routine is in the file
$code CppAD/OdeGear.h$$.

$head Theory$$
For this discussion we use the shorthand $latex x_j$$ 
for the value $latex x ( t_j ) \in \R^n$$ which is not to be confused
with $latex x_i (t) \in \R$$ in the notation above.
The interpolating polynomial $latex p(t)$$ is given by
$latex \[
p(t) = 
\sum_{j=0}^m 
x_j
\frac{ 
	\prod_{i \neq j} ( t - t_i )
}{
	\prod_{i \neq j} ( t_j - t_i ) 
}
\] $$
The derivative $latex p \prime (t)$$ is given by
$latex \[
p \prime (t) = 
\sum_{j=0}^m 
x_j
\frac{ 
	\sum_{i \neq j} \prod_{k \neq i,j} ( t - t_k )
}{ 
	\prod_{k \neq j} ( t_j - t_k ) 
}
\] $$
Evaluating the derivative at the point $latex t_m$$ we have
$latex \[
\begin{array}{rcl}
p \prime ( t_m ) & = & 
x_m
\frac{ 
	\sum_{i \neq m} \prod_{k \neq i,m} ( t_m - t_k )
}{ 
	\prod_{k \neq m} ( t_m - t_k ) 
}
+
\sum_{j=0}^{m-1} 
x_j
\frac{ 
	\sum_{i \neq j} \prod_{k \neq i,j} ( t_m - t_k )
}{ 
	\prod_{k \neq j} ( t_j - t_k ) 
}
\\
& = &
x_m
\sum_{i \neq m} 
\frac{ 1 }{ t_m - t_i }
+
\sum_{j=0}^{m-1} 
x_j
\frac{ 
	\prod_{k \neq m,j} ( t_m - t_k )
}{ 
	\prod_{k \neq j} ( t_j - t_k ) 
}
\\
& = &
x_m
\sum_{k \neq m} ( t_m - t_k )^{-1}
+
\sum_{j=0}^{m-1} 
x_j
( t_j - t_m )^{-1}
\prod_{k \neq m,j} ( t_m - t_k ) / ( t_j - t_k )
\end{array}
\] $$
We define the vector $latex \alpha \in \R^{m+1}$$ by
$latex \[
\alpha_j = \left\{ \begin{array}{ll}
\sum_{k \neq m} ( t_m - t_k )^{-1}
	& {\rm if} \; j = m
\\
( t_j - t_m )^{-1}
\prod_{k \neq m,j} ( t_m - t_k ) / ( t_j - t_k )
	& {\rm otherwise}
\end{array} \right.
\] $$
It follows that
$latex \[
	p \prime ( t_m ) = \alpha_0 x_0 + \cdots + \alpha_m x_m
\] $$
Gear's method determines $latex x_m$$ by solving the following 
nonlinear equation
$latex \[
	f( t_m , x_m ) = \alpha_0 x_0 + \cdots + \alpha_m x_m
\] $$
Newton's method for solving this equation determines iterates, 
which we denote by $latex x_m^k$$, by solving the following affine 
approximation of the equation above
$latex \[
\begin{array}{rcl}
f( t_m , x_m^{k-1} ) + \partial_x f( t_m , x_m^{k-1} ) ( x_m^k - x_m^{k-1} )
& = &
\alpha_0 x_0^k + \alpha_1 x_1 + \cdots + \alpha_m x_m
\\
\left[ \alpha_m I - \partial_x f( t_m , x_m^{k-1} ) \right]  x_m
& = &
\left[ 
f( t_m , x_m^{k-1} ) - \partial_x f( t_m , x_m^{k-1} ) x_m^{k-1} 
- \alpha_0 x_0 - \cdots - \alpha_{m-1} x_{m-1}
\right]
\end{array}
\] $$

$head Gear's Method$$
C. W. Gear, 
``Simultaneous Numerical Solution of Differential-Algebraic Equations,'' 
IEEE Transactions on Circuit Theory, 
vol. 18, no. 1, pp. 89-95, Jan. 1971.


$end
--------------------------------------------------------------------------
*/

# include <cstddef>
# include <CppAD/CppADError.h>
# include <CppAD/CheckSimpleVector.h>
# include <CppAD/CheckNumericType.h>
# include <CppAD/CppAD_vector.h>
# include <CppAD/LuFactor.h>
# include <CppAD/LuInvert.h>

namespace CppAD { // BEGIN CppAD namespace

template <typename Vector, typename Fun>
void OdeGear(
	Fun          &F  , 
	size_t        m  ,
	size_t        n  ,
	const Vector &T  , 
	Vector       &X  ) 
{
	typedef typename Vector::value_type Scalar;

	// check numeric type specifications
	CheckNumericType<Scalar>();

	// check simple vector class specifications
	CheckSimpleVector<Scalar, Vector>();

	CppADUsageError(
		m >= 1,
		"OdeGear: m is less than one"
	);
	CppADUsageError(
		n > 0,
		"OdeGear: n is equal to zero"
	);
	CppADUsageError(
		T.size() == (m+1),
		"OdeGear: size of T is not equal to (m+1)"
	);
	CppADUsageError(
		X.size() == (m+1) * n,
		"OdeGear: size of X is not equal to (m+1) * n"
	);

	// some constants
	Scalar zero(0);
	Scalar one(1);

	// temporary indices
	size_t i, j, k;


	// vectors required by method
	Vector alpha(m + 1);
	Vector f(n);
	Vector f_x(n * n);
	Vector x_m(n);
	Vector b(n);
	Vector A(n * n);

	// compute alpha[m]
	alpha[m] = zero;
	for(k = 0; k < m; k++)
		alpha[m] += one / (T[m] - T[k]);

	// compute other components of alpha
	for(j = 0; j < m; j++)
	{	// compute alpha[j]
		alpha[j] = one / (T[j] - T[m]);
		for(k = 0; k < m; k++)
		{	if( k != j )
			{	alpha[j] *= (T[m] - T[k]);
				alpha[j] /= (T[j] - T[k]);
			}
		}
	}

	// define q(t) as polynomial of degree m-1 such that
	// 	q( t_j ) = x( t_j ) for j = 0 , ... , m-1
	// initialize x_m = q( t_m )
	for(i = 0; i < n; i++)
		x_m[i] = zero;
	for(j = 0; j < m; j++)
	{	// compute term in q( t_m ) that uses x( t_j )
		Scalar factor = one;
		for(k = 0; k < m; k++)
		{	if( k != j )
				factor *= ( T[m] - T[k] ) / ( T[j] - T[k] );
		}
		for(i = 0; i < n; i++)
			x_m[i] += factor * X[ j * n + i ]; 
	}

	// evaluate f( T[m] , x_m ) and it's partial w.r.t x
	F.Ode_dep(T[m], x_m, f_x);

	// compute the matrix A = ( alpha[m] * I - f_x )
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
			A[i * n + j]  = - f_x[i * n + j];
		A[i * n + i] += alpha[m];
	}

	// LU factor (and overwrite) the matrix A
	int sign;
	CppAD::vector<size_t> ip(n) , jp(n);
	sign = LuFactor(ip, jp, A);
	CppADUsageError(
		sign != 0,
		"OdeGear: step size is to large"
	);

	// Iterations of Newton's method
	for(k = 0; k < 3; k++)
	{	std::cout << "x_m = " << x_m << std::endl;

		// only evaluate f( T[m] , x_m ) keep f_x during iteration
		F.Ode(T[m], x_m, f);

		// b = f + f_x x_m - alpha[0] x_0 - ... - alpha[m-1] x_{m-1}
		for(i = 0; i < n; i++)
		{	b[i]         = f[i];
			for(j = 0; j < n; j++)
				b[i]         -= f_x[i * n + j] * x_m[j];
			for(k = 0; k < m; k++)
				b[i] -= alpha[k] * X[ k * n + i ];
		}
		LuInvert(ip, jp, A, b);
		x_m = b;
	}
	std::cout << "x_m = " << x_m << std::endl;

	// return estimate for x( t[k] )
	for(i = 0; i < n; i++)
		X[m * n + i] = x_m[i];
}

} // End CppAD namespace 

# endif
