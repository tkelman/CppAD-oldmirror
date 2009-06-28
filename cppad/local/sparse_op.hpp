/* $Id$ */
# ifndef CPPAD_SPARSE_OP_INCLUDED
# define CPPAD_SPARSE_OP_INCLUDED
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
\file sparse_op.hpp
Forward and reverse mode Jacobian sparsity patterns.
*/


/*!
Forward mode Jacobian sparsity pattern for all unary operators. 

The C++ source code corresponding to a unary operation has the form
\verbatim
	z = fun(y)
\endverbatim
where fun is a C++ unary function, or it has the form
\verbatim
	z = y op p
\endverbatim
where op is a C++ binary unary operator and p is a parameter.

\tparam Pack
is the type used to pack the sparsity pattern bit values; i.e.,
there is more that one bit per Pack value.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in sparsity corresponding to z. 

\param i_y
variable index corresponding to the argument for this operator;
i.e. the row index in sparsity corresponding to y.

\param nc_sparsity
number of packed values corresponding to each variable; i.e.,
the number of columns in the sparsity pattern matrix.

\param sparsity
\b Input: \a sparsity [ \a i_y * \a nc_sparsity + j ]
for j = 0 , ... , \a nc_sparsity - 1 
is the sparsity bit pattern for y.
This identifies which of the independent variables the variable y
depends on. 
\n
\b Output: \a sparsity [ \a i_z * \a nc_sparsity + j ] 
for j = 0 , ... , \a nc_sparsity - 1 
is the sparsity bit pattern for z.

\par Checked Assertions:
\li \a i_y < \a i_z 
*/

template <class Pack>
inline void forward_sparse_jacobian_unary_op(
	size_t     i_z           ,
	size_t     i_y           ,
	size_t     nc_sparsity   ,
	Pack*      sparsity      )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( i_y < i_z );

	Pack* z  = sparsity + i_z * nc_sparsity;
	Pack* y  = sparsity + i_y * nc_sparsity;
	size_t j = nc_sparsity;
	while(j--)
		z[j] = y[j];
	return;
}	

/*!
Reverse mode Jacobian sparsity pattern for all unary operators. 

The C++ source code corresponding to a unary operation has the form
\verbatim
	z = fun(y)
\endverbatim
where fun is a C++ unary function, or it has the form
\verbatim
	z = y op p
\endverbatim
where op is a C++ bianry operator and p is a parameter.

This routine is given the sparsity patterns
for a function G(z, y, ... )
and it uses them to compute the sparsity patterns for 
\verbatim
	H(y, x, ... ) = G[ z(y) , y , x ... ]
\endverbatim

\tparam Pack
is the type used to pack the sparsity pattern bit values; i.e.,
there is more that one bit per Pack value.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in sparsity corresponding to z. 

\param i_y
variable index corresponding to the argument for this operator;
i.e. the row index in sparsity corresponding to y.

\param nc_sparsity
number of packed values corresponding to each variable; i.e.,
the number of columns in the sparsity pattern matrix.

\param sparsity
\b Input: \a sparsity [ \a i_z * \a nc_sparsity + j ]
for j = 0 , ... , \a nc_sparsity - 1 
is the sparsity bit pattern for G with respect to the variable z. 
\n
\b Input: \a sparsity [ \a i_y * \a nc_sparsity + j ]
for j = 0 , ... , \a nc_sparsity - 1 
is the sparsity bit pattern for G with respect to the variable y. 
\n
\b Output: \a sparsity [ \a i_y * \a nc_sparsity + j ] 
for j = 0 , ... , \a nc_sparsity - 1 
is the sparsity bit pattern for H with respect to the variable y.

\par Checked Assertions:
\li \a i_y < \a i_z 
*/

template <class Pack>
inline void reverse_sparse_jacobian_unary_op(
	size_t     i_z           ,
	size_t     i_y           ,
	size_t     nc_sparsity   ,
	Pack*      sparsity      )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( i_y < i_z );

	Pack* z  = sparsity + i_z * nc_sparsity;
	Pack* y  = sparsity + i_y * nc_sparsity;
	size_t j = nc_sparsity;
	while(j--)
		y[j] |= z[j];
	return;
}	

/*!
Reverse mode Hessian sparsity pattern for linear unary operators.

The C++ source code corresponding to this operation is
\verbatim
        z = fun(y)
\endverbatim
where fun is a linear functions; e.g. abs, or
\verbatim
	z = y op p
\endverbatim
where op is a C++ binary operator and p is a parameter.

\copydetails reverse_sparse_hessian_unary_op
*/
template <class Pack>
inline void reverse_sparse_hessian_linear_unary_op(
	size_t      i_z           ,
	size_t      i_y           ,
	Pack        jac_z         ,
	size_t      nc_sparsity   ,
	const Pack* jac_sparsity  ,
	Pack*       hes_sparsity  )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( i_y < i_z );

	Pack* hes_z  = hes_sparsity + i_z * nc_sparsity;
	Pack* hes_y  = hes_sparsity + i_y * nc_sparsity;
	size_t j = nc_sparsity;
	while(j--)
		hes_y[j] |= hes_z[j];
	return;
}

/*!
Reverse mode Hessian sparsity pattern for non-linear unary operators.

The C++ source code corresponding to this operation is
\verbatim
        z = fun(y)
\endverbatim
where fun is a non-linear functions; e.g. sin. or
\verbatim
	z = p / y
\endverbatim
where p is a parameter.


\copydetails reverse_sparse_hessian_unary_op
*/
template <class Pack>
inline void reverse_sparse_hessian_nonlinear_unary_op(
	size_t      i_z           ,
	size_t      i_y           ,
	Pack        jac_z         ,
	size_t      nc_sparsity   ,
	const Pack* jac_sparsity  ,
	Pack*       hes_sparsity  )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( i_y < i_z );

	const Pack* hes_z  = hes_sparsity + i_z * nc_sparsity;
	const Pack* jac_y  = jac_sparsity + i_y * nc_sparsity;
	Pack* hes_y         = hes_sparsity + i_y * nc_sparsity;
	size_t j = nc_sparsity;
	while(j--)
		hes_y[j] |= hes_z[j] | (jac_z & jac_y[j]);
	return;
}

CPPAD_END_NAMESPACE
# endif
