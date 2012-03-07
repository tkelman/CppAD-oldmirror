/* $Id$ */
# ifndef CPPAD_SIN_OP_INCLUDED
# define CPPAD_SIN_OP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


CPPAD_BEGIN_NAMESPACE
/*!
\file sin_op.hpp
Forward and reverse mode calculations for z = sin(x).
*/


/*!
Compute forward mode Taylor coefficient for result of op = SinOp.

The C++ source code corresponding to this operation is
\verbatim
	z = sin(x)
\endverbatim
The value of z, and its derivatives, are computed to the specfied order.
The auxillary variable for this operation is
\verbatim
	y = cos(x)
\endverbatim
The value of y, and its derivatives, are computed to one lower order
than then the order specified for z.

\copydetails forward_unary2_op
*/
template <class Base>
inline void forward_sin_op(
	size_t j           ,
	size_t i_z         ,
	size_t i_x         ,
	size_t nc_taylor   , 
	Base*  taylor      )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumArg(SinOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumRes(SinOp) == 2 );
	CPPAD_ASSERT_UNKNOWN( i_x + 1 < i_z );
	CPPAD_ASSERT_UNKNOWN( j < nc_taylor );

	// Taylor coefficients corresponding to argument and result
	Base* x = taylor + i_x * nc_taylor;
	Base* s = taylor + i_z * nc_taylor;
	Base* c = s      -       nc_taylor;

	// zero order case
	size_t k;
	if( j == 0 )
	{	s[0] = sin( x[0] );
		return;
	}

	// compute auxillary result to order j-1
	size_t j1 = j-1;
	if( j1 == 0 )
		c[0] = cos( x[0] );
	else
	{	c[j1] = Base(0);
		for(k = 1; k <= j1; k++)
			c[j1] -= Base(k) * x[k] * s[j1-k];
		c[j1] /= Base(j1);
	}

	// compute primary result to order j
	s[j] = Base(0);
	for(k = 1; k <= j; k++)
		s[j] += Base(k) * x[k] * c[j-k];
	s[j] /= Base(j);
}


/*!
Compute zero order forward mode Taylor coefficient for result of op = SinOp.

The C++ source code corresponding to this operation is
\verbatim
	z = sin(x)
\endverbatim
The auxillary result is
\verbatim
	y = cos(x)
\endverbatim
The value of y is not computed (because it is only needed to one lower order).

\copydetails forward_unary2_op_0
*/
template <class Base>
inline void forward_sin_op_0(
	size_t i_z         ,
	size_t i_x         ,
	size_t nc_taylor   , 
	Base*  taylor      )
{
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumArg(SinOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumRes(SinOp) == 2 );
	CPPAD_ASSERT_UNKNOWN( i_x + 1 < i_z );
	CPPAD_ASSERT_UNKNOWN( 0 < nc_taylor );

	// Taylor coefficients corresponding to argument and result
	Base* x = taylor + i_x * nc_taylor;
	Base* s = taylor + i_z * nc_taylor;  // called z in documentation

	s[0] = sin( x[0] );
}

/*!
Compute reverse mode partial derivatives for result of op = SinOp.

The C++ source code corresponding to this operation is
\verbatim
	z = sin(x)
\endverbatim
The auxillary result is
\verbatim
	y = cos(x)
\endverbatim
The value of y is computed along with the value of z.

\copydetails reverse_unary2_op
*/

template <class Base>
inline void reverse_sin_op(
	size_t      d            ,
	size_t      i_z          ,
	size_t      i_x          ,
	size_t      nc_taylor    , 
	const Base* taylor       ,
	size_t      nc_partial   ,
	Base*       partial      )
{
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumArg(SinOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumRes(SinOp) == 2 );
	CPPAD_ASSERT_UNKNOWN( i_x + 1 < i_z );
	CPPAD_ASSERT_UNKNOWN( d < nc_taylor );
	CPPAD_ASSERT_UNKNOWN( d < nc_partial );

	// Taylor coefficients and partials corresponding to argument
	const Base* x  = taylor  + i_x * nc_taylor;
	Base* px       = partial + i_x * nc_partial;

	// Taylor coefficients and partials corresponding to first result
	const Base* s  = taylor  + i_z * nc_taylor; // called z in doc
	Base* ps       = partial + i_z * nc_partial;

	// Taylor coefficients and partials corresponding to auxillary result
	const Base* c  = s  - nc_taylor; // called y in documentation
	Base* pc       = ps - nc_partial;

	size_t j, j1, k;
# ifndef NDEBUG
	// check that auxillary varaible is not used except by this operation
	for(j = 0; j <= d; j++)
		CPPAD_ASSERT_UNKNOWN( pc[j] == Base(0) );
# endif

	// run forward_sin_op in reverse
	j = d;
	while(j)
	{	// reverse calculation of s[j] in forward_sin_op
		ps[j]   /= Base(j);
		for(k = 1; k <= j; k++)
		{	px[k]   += ps[j] * Base(k) * c[j-k];
			pc[j-k] += ps[j] * Base(k) * x[k];
		}

		// reverse calculation of c[j1] in forward_sin_op
		j1 = j - 1;
		if( j1 == 0 )
			px[0] -= pc[0] * s[0];
		else
		{	pc[j1]   /= Base(j1);
			for(k = 1; k <= j1; k++)
			{
				px[k]    -= pc[j1] * Base(k) * s[j1-k];
				ps[j1-k] -= pc[j1] * Base(k) * x[k];
			}
		}
		--j;
	}
	if( d == 0 )
	{	// special case where cos( x[0] ) has not yet been calculated
		px[0] += ps[0] * cos( x[0] );
	}
	else
	{	px[0] += ps[0] * c[0];
	}
}

CPPAD_END_NAMESPACE
# endif
