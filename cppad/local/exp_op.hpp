/* $Id$ */
# ifndef CPPAD_EXP_OP_INCLUDED
# define CPPAD_EXP_OP_INCLUDED
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
\file exp_op.hpp
Forward and reverse mode calculations for z = exp(y).
*/


/*!
Forward mode Taylor coefficient for result of op = ExpOp.

The C++ source code corresponding to this operation is
\verbatim
	z = exp(y)
\endverbatim

\copydetails forward_unary1_op
*/
template <class Base>
inline void forward_exp_op(
	size_t p           ,
	size_t i_z         ,
	size_t i_y         ,
	size_t nc_taylor   , 
	Base*  taylor      )
{	
	size_t k;
	static Base zero(0);

	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumInd(ExpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumVar(ExpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( i_y < i_z );
	CPPAD_ASSERT_UNKNOWN( p < nc_taylor );

	// Taylor coefficients corresponding to argument
	Base *y = taylor + i_y * nc_taylor;

	// Taylor coefficients corresponding to result
	Base *z = taylor + i_z * nc_taylor;


	if( p == 0 )
		z[0] = exp( y[0] );
	else
	{
		z[p] = zero;
		for(k = 1; k <= p; k++)
			z[p] += Base(k) * y[k] * z[p-k];
		z[p] /= Base(p);
	}
}

/*!
Zero order forward mode Taylor coefficient for result of op = ExpOp.

The C++ source code corresponding to this operation is
\verbatim
	z = exp(y)
\endverbatim

\copydetails forward_unary1_op_0
*/
template <class Base>
inline void forward_exp_op_0(
	size_t i_z         ,
	size_t i_y         ,
	size_t nc_taylor   , 
	Base*  taylor      )
{

	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumInd(ExpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumVar(ExpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( i_y < i_z );
	CPPAD_ASSERT_UNKNOWN( 0 < nc_taylor );

	// Taylor coefficients corresponding to argument
	Base *y = taylor + i_y * nc_taylor;

	// Taylor coefficients corresponding to result
	Base *z = taylor + i_z * nc_taylor;

	z[0] = exp( y[0] );
}
/*!
Reverse mode partial derivatives for result of op = ExpOp.

The C++ source code corresponding to this operation is
\verbatim
	z = exp(y)
\endverbatim

\copydetails reverse_unary1_op
*/

template <class Base>
inline void reverse_exp_op(
	size_t      p            ,
	size_t      i_z          ,
	size_t      i_y          ,
	size_t      nc_taylor    , 
	const Base* taylor       ,
	size_t      nc_partial   ,
	Base*       partial      )
{	size_t j, k;	

	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumInd(ExpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumVar(ExpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( i_y < i_z );
	CPPAD_ASSERT_UNKNOWN( p < nc_taylor );
	CPPAD_ASSERT_UNKNOWN( p < nc_partial );

	// Taylor coefficients and partials corresponding to argument
	const Base *y  = taylor  + i_y * nc_taylor;
	Base *py       = partial + i_y * nc_partial;

	// Taylor coefficients and partials corresponding to result
	const Base *z  = taylor  + i_z * nc_taylor;
	Base *pz       = partial + i_z * nc_partial;

	// number of indices to access
	j = p;
	while(j)
	{	// scale partial w.r.t z[j]
		pz[j] /= Base(j);

		for(k = 1; k <= j; k++)
		{	py[k]   += pz[j] * Base(k) * z[j-k]; 	
			pz[j-k] += pz[j] * Base(k) * y[k];
		}
		--j;
	}
	py[0] += pz[0] * z[0];
}

CPPAD_END_NAMESPACE
# endif
