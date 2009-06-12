/* $Id$ */
# ifndef CPPAD_ABS_OP_INCLUDED
# define CPPAD_ABS_OP_INCLUDED
CPPAD_BEGIN_NAMESPACE

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


/*!
\file abs_op.hpp
*/


/*!
Forward mode Taylor coefficient of the specfied order result of AbsOp operation.

The C++ source code corresponding to this operation is
\verbatim
	z = abs(x)
\endverbatim

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in taylor to z. 

\param arg
arg[0] is the variable index corresponding to the argument for this operation;
i.e. the row index in taylor corresponding to x.

\param p
order of the Taylor coefficient that we are computing.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: taylor[ arg[0] * nc_taylor + j ] 
is the j-th order Taylor coefficient corresponding to x for j = 0 , ... , p. 
\n
\b Input: taylor[ i_z * nc_taylor + j ] is the j-th order Taylor coefficient 
corresponding to z for j = 0 , ... , p-1. 
\n
\b Output: taylor[ i_z * nc_taylor + p ] is the p-th order Taylor coefficient 
corresponding to z. 

\par Checked Assertions:
\li NumInd(AbsOp) == 1
\li NumVar(AbsOp) == 1
\li arg[0] < i_z 
\li p < nc_taylor
*/
template <class Base>
inline void forward_abs_op(
	size_t i_z         ,
	const size_t *arg  ,
	size_t p           ,
	size_t nc_taylor   , 
	Base   *taylor     )
{
	static Base zero(0);

	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumInd(AbsOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumVar(AbsOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( arg[0] < i_z );
	CPPAD_ASSERT_UNKNOWN( p < nc_taylor );

	// Taylor coefficients corresponding to argument
	Base *x = taylor + arg[0] * nc_taylor;

	// Taylor coefficients corresponding to result
	Base *z = taylor + i_z * nc_taylor;

	// order that decides positive, negative or zero
	size_t k;
	k = 0;
	while( (k < p) & (x[k] == zero) )
		k++; 

	if( GreaterThanZero(x[k]) )
		z[p]  = x[p];
	else if( LessThanZero(x[k]) )
		z[p] = -x[p]; 
	else	z[p] = zero;
}

/*!
Reverse mode Partial derivatives of the specfied order for AbsOp.

The C++ source code corresponding to this operation is
\verbatim
	z = abs(x)
\endverbatim
This routine is given the partial derivatives of a function G(z, x, ... )
and it uses them to compute the partial derivatives of 
\verbatim
	H(x, ... ) = G[ z(x) , x , ... ]
\endverbatim

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in taylor to z. 

\param arg
arg[0] is the variable index corresponding to the argument for this operation;
i.e. the row index in taylor corresponding to x.

\param p
order of the partial derivative that we are computing 

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
taylor[ arg[0] * nc_taylor + j ] 
is the j-th order Taylor coefficient corresponding to x for j = 0 , ... , p-1. 
\n
taylor[ i_z * nc_taylor + j ] is the j-th order Taylor coefficient 
corresponding to z for j = 0 , ... , p-1. 


\param nc_partial
number of colums in the matrix containing all the partial derivatives.

\param partial
\b Input: partial[ arg[0] * nc_taylor + j ] 
is the j-th order partial derivative of G(z , x , ... ) with respect to z
for j = 0 , ... , p. 
\n
\b Input: partial[ i_z * nc_taylor + j ] 
is the j-th order partial derivative of G(z , x , ... ) with respect to x.
for j = 0 , ... , p. 
\n
\b Output: partial[ arg[0] * nc_taylor + j ]
is the j-th order partial derivative of H(x , ... ) with respect to x
for j = 0 , ... , p.


\par Checked Assumptions:
\li NumInd(AbsOp) == 1
\li NumVar(AbsOp) == 1
\li arg[0] < i_z 
\li p < nc_taylor
\li p < nc_partial
*/
template <class Base>
inline void reverse_abs_op(
	size_t i_z          ,
	const size_t *arg   ,
	size_t p            ,
	size_t nc_taylor    , 
	const Base  *taylor ,
	size_t nc_partial   ,
	Base   *partial     )
{	size_t j, k;	
	static Base zero(0);


	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumInd(AbsOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumVar(AbsOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( arg[0] < i_z );
	CPPAD_ASSERT_UNKNOWN( p < nc_taylor );
	CPPAD_ASSERT_UNKNOWN( p < nc_partial );

	// Taylor coefficients and partials corresponding to argument
	const Base *x  = taylor  + arg[0] * nc_taylor;
	Base *px       = partial + arg[0] * nc_partial;

	// Taylor coefficients and partials corresponding to result
	Base *pz       = partial +    i_z * nc_partial;

	// order that decides positive, negative or zero
	k = 0;
	while( (k < p) & (x[k] == zero) )
		k++; 

	if( GreaterThanZero(x[k]) )
	{	// partial of z w.r.t x is +1
		for(j = k; j <= p; j++)
			px[j] += pz[j];
	}
	else if( LessThanZero(x[k]) )
	{	// partial of z w.r.t x is -1
		for(j = k; j <= p; j++)
			px[j] -= pz[j];
	}

}

CPPAD_END_NAMESPACE
# endif
