/* $Id$ */
# ifndef CPPAD_PROTOTYPE_OP_INCLUDED
# define CPPAD_PROTOTYPE_OP_INCLUDED
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
\file prototype_op.hpp
Documentation for generic cases (these generic cases are never used).
*/


/*!
Prototype for forward mode unary operator with one result (not used).

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by this routine are done using type Base.

\param p
order of the Taylor coefficient that we are computing.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in taylor corresponding to z. 

\param i_y
variable index corresponding to the argument for this operator;
i.e. the row index in taylor corresponding to y.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: \a taylor [ \a i_y * \a nc_taylor + j ] 
is the j-th order Taylor coefficient corresponding to y 
for j = 0 , ... , \a p . 
\n
\b Input: \a taylor [ \a i_z * \a nc_taylor + j ] 
is the j-th order Taylor coefficient 
corresponding to z 
for j = 0 , ... , \a p -1. 
\n
\b Output: \a taylor [ \a i_z * \a nc_taylor + p ] 
is the p-th order Taylor coefficient 
corresponding to z. 

\par Checked Assertions where op is a unary operator with one result:
\li NumArg(op) == 1
\li NumRes(op) == 1
\li \a i_y < \a i_z 
\li \a p < \a nc_taylor
*/
template <class Base>
inline void forward_unary1_op(
	size_t p           ,
	size_t i_z         ,
	size_t i_y         ,
	size_t nc_taylor   , 
	Base*  taylor      )
{
	// this routine should never be included, much less called.
	CPPAD_ASSERT_UNKNOWN( false );
}

/*!
Prototype for zero order forward mode unary operator with one result (not used). 
\tparam Base
base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by this routine are done using type Base.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in taylor corresponding to z. 

\param i_y
variable index corresponding to the argument for this operator;
i.e. the row index in taylor corresponding to y.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: \a taylor [ \a i_y * \a nc_taylor + 0 ] 
is the zero order Taylor coefficient corresponding to y. 
\n
\b Output: \a taylor [ \a i_z * \a nc_taylor + 0 ] 
is the zero order Taylor coefficient corresponding to z. 

\par Checked Assertions where op is the unary operator with one result:
\li NumArg(op) == 1
\li NumRes(op) == 1
\li \a i_y < \a i_z 
\li \a p < \a nc_taylor
*/
template <class Base>
inline void forward_unary1_op_0(
	size_t i_z         ,
	size_t i_y         ,
	size_t nc_taylor   , 
	Base*  taylor      )
{
	// this routine should never be included, much less called.
	CPPAD_ASSERT_UNKNOWN( false );
}

/*!
Prototype for reverse mode unary operator with one result (not used).

This routine is given the partial derivatives of a function G(z, y, ... )
and it uses them to compute the partial derivatives of 
\verbatim
	H(y, x, ... ) = G[ z(y) , y , x ... ]
\endverbatim

\param p
order of the partial derivative that we are computing 

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in taylor to z. 

\param i_y
variable index corresponding to the argument for this operation;
i.e. the row index in taylor corresponding to y.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\a taylor [ \a i_y * \a nc_taylor + j ] 
is the j-th order Taylor coefficient corresponding to y 
for j = 0 , ... , \a p -1. 
\n
\a taylor [ \a i_z * \a nc_taylor + j ] 
is the j-th order Taylor coefficient corresponding to z 
for j = 0 , ... , \a p -1. 


\param nc_partial
number of colums in the matrix containing all the partial derivatives.

\param partial
\b Input: \a partial [ \a i_y * \a nc_partial + j ] 
is the j-th order partial derivative of G(z , y , x , ... ) with respect to y
for j = 0 , ... , \a p . 
\n
\b Input: \a partial [ \a i_z * \a nc_partial + j ] 
is the j-th order partial derivative of G(z , y , x , ... ) with respect to z.
for j = 0 , ... , \a p. 
\n
\b Output: \a partial [ \a i_y * \a nc_partial + j ]
is the j-th order partial derivative of H(y , x , ... ) with respect to y
for j = 0 , ... , \a p .


\par Checked Assumptions where op is a unary operator with one result:
\li NumArg(op) == 1
\li NumRes(op) == 1
\li \a i_y < \a i_z 
\li \a p < \a nc_taylor
\li \a p < \a nc_partial
*/
template <class Base>
inline void reverse_unary1_op(
	size_t      p            ,
	size_t      i_z          ,
	size_t      i_y          ,
	size_t      nc_taylor    , 
	const Base* taylor       ,
	size_t      nc_partial   ,
	Base*       partial      )
{
	// this routine should never be included, much less called.
	CPPAD_ASSERT_UNKNOWN( false );
}

/*!
Prototype for reverse mode Hessian sparsity, unary operators with one result (not used). 

This routine is given the forward mode Jacobian sparsity patterns for y.
It is also given the reverse mode dependence of G on z.
In addition, it is given the revese mode Hessian sparsity
for the quanity of interest G(z, y, ... )
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

\param jac_z
is all zero (ones) if the Jacobian of G with respect to z is 
zero (non-zero).

\param nc_sparsity
number of packed values corresponding to each sparsity pattern; i.e.,
the number of columns in the sparsity pattern matrices.

\param jac_sparsity
For j = 0 , ... , nc_sparsity - 1,
jac_sparsity[ \a i_y * nc_sparsity + j ]
is the forward mode Jacobian sparsity pattern for the variable y. 

\param hes_sparsity
\b Input: hes_sparsity[ \a i_z * nc_sparsity + j ]
for j = 0 , ... , nc_sparsity -1 
is the Hessian sparsity pattern for the fucntion G
where one of the partials derivative is with respect to z.
\n
\b Input: hes_sparsity[ \a i_y * nc_sparsity + j ]
for j = 0 , ... , nc_sparsity -1 
is the Hessian sparsity pattern for the fucntion G
where one of the partials derivative is with respect to y.
\n
\b Output: hes_sparsity[ \a i_y * nc_sparsity + j ]
for j = 0 , ... , nc_sparsity -1 
is the Hessian sparsity pattern for the fucntion H
where one of the partials derivative is with respect to y.

\par Checked Assertions:
\li \a i_y < \a i_z 
*/
template <class Pack>
inline void reverse_sparse_hessian_unary_op(
	size_t      i_z           ,
	size_t      i_y           ,
	Pack        jac_z         ,
	size_t      nc_sparsity   ,
	const Pack* jac_sparsity  ,
	Pack*       hes_sparsity  )
{	
	// this routine should never be included, much less called
	CPPAD_ASSERT_UNKNOWN( false );
}


CPPAD_END_NAMESPACE
# endif
