/* $Id$ */
# ifndef CPPAD_ACOS_OP_INCLUDED
# define CPPAD_ACOS_OP_INCLUDED

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
\defgroup acos_op_hpp acos_op.hpp
\{
\file acos_op.hpp
Forward and reverse mode calculations for z = acos(x).
*/


/*!
Compute forward mode Taylor coefficient for result of op = AcosOp.

The C++ source code corresponding to this operation is
\verbatim
	z = acos(x)
\endverbatim
The auxillary result is
\verbatim
	y = sqrt(1 - x * x)
\endverbatim
The value of y, and its derivatives, are computed along with the value
and derivatives of z.

\copydetails forward_unary2_op
*/
template <class Base>
inline void forward_acos_op(
	size_t q           ,
	size_t p           ,
	size_t i_z         ,
	size_t i_x         ,
	size_t nc_taylor   , 
	Base*  taylor      )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumArg(AcosOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumRes(AcosOp) == 2 );
	CPPAD_ASSERT_UNKNOWN( i_x + 1 < i_z );
	CPPAD_ASSERT_UNKNOWN( p < nc_taylor );
	CPPAD_ASSERT_UNKNOWN( q <= p );

	// Taylor coefficients corresponding to argument and result
	Base* x = taylor + i_x * nc_taylor;
	Base* z = taylor + i_z * nc_taylor;
	Base* b = z      -       nc_taylor;  // called y in documentation

	size_t k;
	Base uj;
	if( q == 0 )
	{	z[0] = acos( x[0] );
		uj   = Base(1) - x[0] * x[0];
		b[0] = sqrt( uj );
		q++;
	}
	for(size_t j = q; j <= p; j++)
	{	uj = 0.;
		for(k = 0; k <= j; k++)
			uj -= x[k] * x[j-k];
		b[j] = Base(0);
		z[j] = Base(0);
		for(k = 1; k < j; k++)
		{	b[j] -= Base(k) * b[k] * b[j-k];
			z[j] -= Base(k) * z[k] * b[j-k];
		}
		b[j] /= Base(j);
		z[j] /= Base(j);
		//
		b[j] += uj / Base(2);
		z[j] -= x[j];
		//
		b[j] /= b[0];
		z[j] /= b[0];
	}
}

/*!
Compute zero order forward mode Taylor coefficient for result of op = AcosOp.

The C++ source code corresponding to this operation is
\verbatim
	z = acos(x)
\endverbatim
The auxillary result is
\verbatim
	y = sqrt( 1 - x * x )
\endverbatim
The value of y is computed along with the value of z.

\copydetails forward_unary2_op_0
*/
template <class Base>
inline void forward_acos_op_0(
	size_t i_z         ,
	size_t i_x         ,
	size_t nc_taylor   , 
	Base*  taylor      )
{
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumArg(AcosOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumRes(AcosOp) == 2 );
	CPPAD_ASSERT_UNKNOWN( i_x + 1 < i_z );
	CPPAD_ASSERT_UNKNOWN( 0 < nc_taylor );

	// Taylor coefficients corresponding to argument and result
	Base* x = taylor + i_x * nc_taylor;
	Base* z = taylor + i_z * nc_taylor;
	Base* b = z      -       nc_taylor; // called y in documentation

	z[0] = acos( x[0] );
	b[0] = sqrt( Base(1) - x[0] * x[0] );
}
/*!
Compute reverse mode partial derivatives for result of op = AcosOp.

The C++ source code corresponding to this operation is
\verbatim
	z = acos(x)
\endverbatim
The auxillary result is
\verbatim
	y = sqrt( 1 - x * x )
\endverbatim
The value of y is computed along with the value of z.

\copydetails reverse_unary2_op
*/

template <class Base>
inline void reverse_acos_op(
	size_t      d            ,
	size_t      i_z          ,
	size_t      i_x          ,
	size_t      nc_taylor    , 
	const Base* taylor       ,
	size_t      nc_partial   ,
	Base*       partial      )
{
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumArg(AcosOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumRes(AcosOp) == 2 );
	CPPAD_ASSERT_UNKNOWN( i_x + 1 < i_z );
	CPPAD_ASSERT_UNKNOWN( d < nc_taylor );
	CPPAD_ASSERT_UNKNOWN( d < nc_partial );

	// Taylor coefficients and partials corresponding to argument
	const Base* x  = taylor  + i_x * nc_taylor;
	Base* px       = partial + i_x * nc_partial;

	// Taylor coefficients and partials corresponding to first result
	const Base* z  = taylor  + i_z * nc_taylor;
	Base* pz       = partial + i_z * nc_partial;

	// Taylor coefficients and partials corresponding to auxillary result
	const Base* b  = z  - nc_taylor; // called y in documentation
	Base* pb       = pz - nc_partial;

	// number of indices to access
	size_t j = d;
	size_t k;
	while(j)
	{
		// scale partials w.r.t b[j] by 1 / b[0]
		pb[j] /= b[0];

		// scale partials w.r.t z[j] by 1 / b[0]
		pz[j] /= b[0];

		// update partials w.r.t b^0 
		pb[0] -= pz[j] * z[j] + pb[j] * b[j]; 

		// update partial w.r.t. x^0
		px[0] -= pb[j] * x[j];

		// update partial w.r.t. x^j
		px[j] -= pz[j] + pb[j] * x[0];

		// further scale partial w.r.t. z[j] by 1 / j
		pz[j] /= Base(j);

		for(k = 1; k < j; k++)
		{	// update partials w.r.t b^(j-k)
			pb[j-k] -= Base(k) * pz[j] * z[k] + pb[j] * b[k];

			// update partials w.r.t. x^k 
			px[k]   -= pb[j] * x[j-k];

			// update partials w.r.t. z^k
			pz[k]   -= pz[j] * Base(k) * b[j-k];
		}
		--j;
	}

	// j == 0 case
	px[0] -= ( pz[0] + pb[0] * x[0]) / b[0];
}

/*! \} */
} // END_CPPAD_NAMESPACE
# endif
