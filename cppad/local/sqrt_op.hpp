/* $Id$ */
# ifndef CPPAD_SQRT_OP_INCLUDED
# define CPPAD_SQRT_OP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\defgroup sqrt_op_hpp sqrt_op.hpp
\{
\file sqrt_op.hpp
Forward and reverse mode calculations for z = sqrt(x).
*/


/*!
Compute forward mode Taylor coefficient for result of op = SqrtOp.

The C++ source code corresponding to this operation is
\verbatim
	z = sqrt(x)
\endverbatim

\copydetails forward_unary1_op
*/
template <class Base>
inline void forward_sqrt_op(
	size_t q           ,
	size_t p           ,
	size_t i_z         ,
	size_t i_x         ,
	size_t nc_taylor   , 
	Base*  taylor      )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumArg(SqrtOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumRes(SqrtOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( i_x < i_z );
	CPPAD_ASSERT_UNKNOWN( p < nc_taylor );
	CPPAD_ASSERT_UNKNOWN( q <= p );

	// Taylor coefficients corresponding to argument and result
	Base* x = taylor + i_x * nc_taylor;
	Base* z = taylor + i_z * nc_taylor;

	size_t k;
	if( q == 0 )
	{	z[0] = sqrt( x[0] );
		q++;
	}
	for(size_t j = q; j <= p; j++)
	{
		CPPAD_ASSERT_KNOWN(
			x[0] != Base(0),
			"Forward: attempt to take derivatve of square root of zero"
		)
		z[j] = Base(0);
		for(k = 1; k < j; k++)
			z[j] -= Base(k) * z[k] * z[j-k];
		z[j] /= Base(j);
		z[j] += x[j] / Base(2);
		z[j] /= z[0];
	}
}

/*!
Compute zero order forward mode Taylor coefficient for result of op = SqrtOp.

The C++ source code corresponding to this operation is
\verbatim
	z = sqrt(x)
\endverbatim

\copydetails forward_unary1_op_0
*/
template <class Base>
inline void forward_sqrt_op_0(
	size_t i_z         ,
	size_t i_x         ,
	size_t nc_taylor   , 
	Base*  taylor      )
{
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumArg(SqrtOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumRes(SqrtOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( i_x < i_z );
	CPPAD_ASSERT_UNKNOWN( 0 < nc_taylor );

	// Taylor coefficients corresponding to argument and result
	Base* x = taylor + i_x * nc_taylor;
	Base* z = taylor + i_z * nc_taylor;

	z[0] = sqrt( x[0] );
}
/*!
Compute reverse mode partial derivatives for result of op = SqrtOp.

The C++ source code corresponding to this operation is
\verbatim
	z = sqrt(x)
\endverbatim

\copydetails reverse_unary1_op
*/

template <class Base>
inline void reverse_sqrt_op(
	size_t      d            ,
	size_t      i_z          ,
	size_t      i_x          ,
	size_t      nc_taylor    , 
	const Base* taylor       ,
	size_t      nc_partial   ,
	Base*       partial      )
{
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumArg(SqrtOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumRes(SqrtOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( i_x < i_z );
	CPPAD_ASSERT_UNKNOWN( d < nc_taylor );
	CPPAD_ASSERT_UNKNOWN( d < nc_partial );

	// Taylor coefficients and partials corresponding to argument
	Base* px       = partial + i_x * nc_partial;

	// Taylor coefficients and partials corresponding to result
	const Base* z  = taylor  + i_z * nc_taylor;
	Base* pz       = partial + i_z * nc_partial;

	CPPAD_ASSERT_KNOWN(
		z[0] != Base(0),
		"Reverse: attempt to take derivatve of square root of zero"
	)

	// number of indices to access
	size_t j = d;
	size_t k;
	while(j)
	{

		// scale partial w.r.t. z[j]
		pz[j]   /= z[0];

		pz[0]   -= pz[j] * z[j];
		px[j]   += pz[j] / Base(2);
		for(k = 1; k < j; k++)
			pz[k]   -= pz[j] * z[j-k];
		--j;
	}
	px[0] += pz[0] / (Base(2) * z[0]);
}

/*! \} */
} // END_CPPAD_NAMESPACE
# endif
