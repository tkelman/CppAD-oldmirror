// $Id$
# ifndef CPPAD_CSKIP_OP_INCLUDED
# define CPPAD_CSKIP_OP_INCLUDED

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
\defgroup cskip_op_hpp cskip_op.hpp
\{
\file cskip_op.hpp
Zero order forward mode set which operations to skip.
*/

/*!
Zero order forward mode execution of op = CSkipOp.

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type
\a Base.

\param i_z
variable index corresponding to the result of the previous operation.
This is used for error checking. To be specific,
the left and right operands for the CExpOp operation must have indexes
less than or equal this value.

\param arg
\a arg[0] 
is the index of the corresponding CExpOp operation. This is used to 
determine if the comparision result is true or false.
\n
\a arg[1] 
is the number of operations to skip if the comparision result is true.
\n
\a arg[2] 
is the number of operations to skip if the comparision result is false.
\n
<tt>arg[2+i]</tt>
for <tt>i = 1 , ... , arg[1]</tt> are the operations to skip if the
comparision result is true.
\n
<tt>arg[2+arg[1]+i]</tt>
for <tt>i = 1 , ... , arg[2]</tt> are the operations to skip if the
comparision result is false.

\param \skip_op_list
is vector specifying which operations are at this point are know to be
unecessary and can be skipped. 
This is both an input and an output.
*/
template <class Base>
inline void forward_cskip_op_0(
	size_t        i_z          ,
	const addr_t* arg          ,
	size_t        skip_op_list )
{
	// This routine not yet implemented
	CPPAD_ASSERT_UNKNOWN(false);
}
/*! \} */
} // END_CPPAD_NAMESPACE
# endif

