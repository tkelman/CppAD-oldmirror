/* $Id$ */
# ifndef CPPAD_STORE_OP_INCLUDED
# define CPPAD_STORE_OP_INCLUDED

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
\defgroup store_op_hpp store_op.hpp
\{
\file store_op.hpp
Changing the current value of a VecAD element.
*/
/*!
Shared documentation for zero order forward implementation of 
op = StppOp, StpvOp, StvpOp, or StvvOp (not called).

The C++ source code corresponding to this operation is
\verbatim
	v[x] = y
\endverbatim
where v is a VecAD<Base> vector, x is an AD<Base> object,
and y is AD<Base> or Base objects. 
We define the index corresponding to v[x] by
\verbatim
	i_v_x = combined[ arg[0] + i_vec ]
\endverbatim
where i_vec is defined under the heading \a arg[1] below:

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param i_z
is the index corresponding to the previous variable on the tape
(only used for error checking).

\param arg
\n
\a arg[0]
is the offset of this VecAD vector relative to the beginning 
of the \a combined VecAD array.
\n
\n 
\a arg[1] 
\n
If this is a StppOp or StpvOp operation 
(the index x is a parameter), i_vec is defined by
\verbatim
	i_vec = arg[1]
\endverbatim
If this is a StvpOp or StvvOp operation 
(the index x is a variable), i_vec is defined by
\verbatim
	i_vec = floor( taylor[ arg[1] * nc_taylor + 0 ] )
\endverbatim
where floor(c) is the greatest integer less that or equal c.
\n
\n
\a arg[2]
\n
index corresponding to the third operand for this operator;
i.e. the index corresponding to y.

\param num_par
is the total number of parameters on the tape
(only used for error checking).

\param nc_taylor
number of columns in the matrix containing the Taylor coefficients.

\param taylor
\b Input: in StvpOp and StvvOp cases, \a taylor[ arg[1] * nc_taylor + 0 ]
is used to compute the index in the definition of i_vec above

\param nc_combined
is the total number of elements in the combined VecAD array.

\param variable
\b Output: If y is a varable (StpvOp or StvvOp), 
\a variable [ \a arg[0] + i_vec ] is set to true.
Otherwise y is a paraemter (StppOp or StvpOp) and 
\a variable [ \a arg[0] + i_vec ] is set to false.

\param combined
\b Output: \a combined [ \a arg[0] + i_vec ]
is set equal to \a arg[2].

\par Check User Errors
\li Check that the index is with in range; i.e.
i_vec < combined[ \a arg[0] - 1 ]
Note that, if x is a parameter, is the corresponding vector index
and it does not change.
In this case, the error above should be detected during tape recording.

\par Checked Assertions 
\li combined != CPPAD_NULL
\li variable != CPPAD_NULL
\li NumArg(op) == 3
\li NumRes(op) == 0
\li 0 <  \a arg[0]
\li \a arg[0] + i_vec < nc_combined
\li if y is a parameter, arg[2] < num_par
\li if x is a variable, arg[1] <= i_z
\li if y is a variable, arg[2] <= i_z
*/
template <class Base>
inline void forward_store_op_0(
	size_t         i_z         ,
	const addr_t*  arg         , 
	size_t         num_par     ,
	size_t         nc_taylor   ,
	Base*          taylor      ,
	size_t         nc_combined ,
	bool*          variable    ,
	size_t*        combined    )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}
/*!
Shared documnetation for sparsity operations corresponding to 
op = StpvOp or StvvOp (not called).

<!-- define sparse_store_op -->
The C++ source code corresponding to this operation is
\verbatim
	v[x] = y
\endverbatim
where v is a VecAD<Base> vector, x is an AD<Base> object,
and y is AD<Base> or Base objects. 
We define the index corresponding to v[x] by
\verbatim
	i_v_x = combined[ arg[0] + i_vec ]
\endverbatim
where i_vec is defined under the heading \a arg[1] below:

\tparam Vector_set
is the type used for vectors of sets. It can be either
\c sparse_pack, \c sparse_set, or \c sparse_list.

\param op
is the code corresponding to this operator; i.e., StpvOp or StvvOp
(only used for error checking).

\param arg
\n
\a arg[0]
is the offset corresponding to this VecAD vector in the combined array.
\n
\n 
\a arg[2]
\n
The set with index \a arg[2] in \a var_sparsity 
is the sparsity pattern corresponding to y.
(Note that \a arg[2] > 0 because y is a variable.) 

\param num_combined
is the total number of elements in the VecAD address array.

\param combined
\a combined [ arg[0] - 1 ]
is the index of the set in \a vecad_sparsity corresponding
to the sparsity pattern for the vector v.
We use the notation i_v below which is defined by
\verbatim
	i_v = combined[ \a arg[0] - 1 ]
\endverbatim

\param var_sparsity
The set  with index \a arg[2] in \a var_sparsity 
is the sparsity pattern for y.
This is an input for forward mode operations.
For reverse mode operations:
The sparsity pattern for v is added to the spartisy pattern for y.

\param vecad_sparsity
The set with index \a i_v in \a vecad_sparsity 
is the sparsity pattern for v.
This is an input for reverse mode operations.
For forward mode operations, the sparsity pattern for y is added
to the sparsity pattern for the vector v.

\par Checked Assertions 
\li NumArg(op) == 3
\li NumRes(op) == 0
\li 0 <  \a arg[0]
\li \a arg[0] < \a num_combined
\li \a arg[2] < \a var_sparsity.n_set()
\li i_v       < \a vecad_sparsity.n_set()
<!-- end sparse_store_op -->
*/
template <class Vector_set>
inline void sparse_store_op(
	OpCode         op             ,
	const addr_t*  arg            , 
	size_t         num_combined   ,
	const size_t*  combined       ,
	Vector_set&    var_sparsity   ,
	Vector_set&    vecad_sparsity )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}


/*!
Zero order forward mode implementation of op = StppOp.

\copydetails forward_store_op_0
*/
template <class Base>
inline void forward_store_pp_op_0(
	size_t         i_z         ,
	const addr_t*  arg         , 
	size_t         num_par     ,
	size_t         nc_taylor   ,
	Base*          taylor      ,
	size_t         nc_combined ,
	bool*          variable    ,
	size_t*        combined    )
{	size_t i_vec = arg[1];

	// Because the index is a parameter, this indexing error should be 
	// caught and reported to the user when the tape is recording.
	CPPAD_ASSERT_UNKNOWN( i_vec < combined[ arg[0] - 1 ] );

	CPPAD_ASSERT_UNKNOWN( variable != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( combined != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( NumArg(StppOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(StppOp) == 0 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[0] + i_vec < nc_combined );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < num_par );

	variable[ arg[0] + i_vec ] = false;
	combined[ arg[0] + i_vec ] = arg[2];
}

/*!
Zero order forward mode implementation of op = StpvOp.

\copydetails forward_store_op_0
*/
template <class Base>
inline void forward_store_pv_op_0(
	size_t         i_z         ,
	const addr_t*  arg         , 
	size_t         num_par     ,
	size_t         nc_taylor   ,
	Base*          taylor      ,
	size_t         nc_combined ,
	bool*          variable    ,
	size_t*        combined    )
{	size_t i_vec = arg[1];

	// Because the index is a parameter, this indexing error should be 
	// caught and reported to the user when the tape is recording.
	CPPAD_ASSERT_UNKNOWN( i_vec < combined[ arg[0] - 1 ] );

	CPPAD_ASSERT_UNKNOWN( variable != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( combined != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( NumArg(StpvOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(StpvOp) == 0 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[0] + i_vec < nc_combined );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) <= i_z );

	variable[ arg[0] + i_vec ] = true;
	combined[ arg[0] + i_vec ] = arg[2];
}

/*!
Zero order forward mode implementation of op = StvpOp.

\copydetails forward_store_op_0
*/
template <class Base>
inline void forward_store_vp_op_0(
	size_t         i_z         ,
	const addr_t*  arg         , 
	size_t         num_par     ,
	size_t         nc_taylor   ,
	Base*          taylor      ,
	size_t         nc_combined ,
	bool*          variable    ,
	size_t*        combined    )
{	
	CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) <= i_z );
	size_t i_vec = Integer( taylor[ arg[1] * nc_taylor + 0 ] );
	CPPAD_ASSERT_KNOWN( 
		i_vec < combined[ arg[0] - 1 ] ,
		"VecAD: index during zero order forward sweep is out of range"
	);

	CPPAD_ASSERT_UNKNOWN( variable != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( combined != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( NumArg(StvpOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(StvpOp) == 0 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[0] + i_vec < nc_combined );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < num_par );

	variable[ arg[0] + i_vec ] = false;
	combined[ arg[0] + i_vec ] = arg[2];
}

/*!
Zero order forward mode implementation of op = StvvOp.

\copydetails forward_store_op_0
*/
template <class Base>
inline void forward_store_vv_op_0(
	size_t         i_z         ,
	const addr_t*  arg         , 
	size_t         num_par     ,
	size_t         nc_taylor   ,
	Base*          taylor      ,
	size_t         nc_combined ,
	bool*          variable    ,
	size_t*        combined    )
{	
	CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) <= i_z );
	size_t i_vec = Integer( taylor[ arg[1] * nc_taylor + 0 ] );
	CPPAD_ASSERT_KNOWN( 
		i_vec < combined[ arg[0] - 1 ] ,
		"VecAD: index during zero order forward sweep is out of range"
	);

	CPPAD_ASSERT_UNKNOWN( variable != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( combined != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( NumArg(StvpOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(StvpOp) == 0 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[0] + i_vec < nc_combined );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) <= i_z );

	variable[ arg[0] + i_vec ] = true;
	combined[ arg[0] + i_vec ] = arg[2];
}

/*!
Forward mode sparsity operations for StpvOp and StvvOp

\copydetails sparse_store_op
*/
template <class Vector_set>
inline void forward_sparse_store_op(
	OpCode              op             ,
	const addr_t*       arg            , 
	size_t              num_combined   ,
	const size_t*       combined       ,
	Vector_set&         var_sparsity   ,
	Vector_set&         vecad_sparsity )
{
	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) == 0 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_combined );
	size_t i_v = combined[ arg[0] - 1 ];
	CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < var_sparsity.n_set() );

	vecad_sparsity.binary_union(i_v, i_v, arg[2], var_sparsity);

	return;
}

/*!
Reverse mode sparsity operations for StpvOp and StvvOp

This routine is given the sparsity patterns for
G(v[x], y , w , u ... ) and it uses them to compute the 
sparsity patterns for  
\verbatim
	H(y , w , u , ... ) = G[ v[x], y , w , u , ... ]
\endverbatim

<!-- replace sparse_store_op -->
The C++ source code corresponding to this operation is
\verbatim
	v[x] = y
\endverbatim
where v is a VecAD<Base> vector, x is an AD<Base> object,
and y is AD<Base> or Base objects. 
We define the index corresponding to v[x] by
\verbatim
	i_v_x = combined[ arg[0] + i_vec ]
\endverbatim
where i_vec is defined under the heading \a arg[1] below:

\tparam Vector_set
is the type used for vectors of sets. It can be either
\c sparse_pack, \c sparse_set, or \c sparse_list.

\param op
is the code corresponding to this operator; i.e., StpvOp or StvvOp
(only used for error checking).

\param arg
\n
\a arg[0]
is the offset corresponding to this VecAD vector in the combined array.
\n
\n 
\a arg[2]
\n
The set with index \a arg[2] in \a var_sparsity 
is the sparsity pattern corresponding to y.
(Note that \a arg[2] > 0 because y is a variable.) 

\param num_combined
is the total number of elements in the VecAD address array.

\param combined
\a combined [ arg[0] - 1 ]
is the index of the set in \a vecad_sparsity corresponding
to the sparsity pattern for the vector v.
We use the notation i_v below which is defined by
\verbatim
	i_v = combined[ \a arg[0] - 1 ]
\endverbatim

\param var_sparsity
The set  with index \a arg[2] in \a var_sparsity 
is the sparsity pattern for y.
This is an input for forward mode operations.
For reverse mode operations:
The sparsity pattern for v is added to the spartisy pattern for y.

\param vecad_sparsity
The set with index \a i_v in \a vecad_sparsity 
is the sparsity pattern for v.
This is an input for reverse mode operations.
For forward mode operations, the sparsity pattern for y is added
to the sparsity pattern for the vector v.

\par Checked Assertions 
\li NumArg(op) == 3
\li NumRes(op) == 0
\li 0 <  \a arg[0]
\li \a arg[0] < \a num_combined
\li \a arg[2] < \a var_sparsity.n_set()
\li i_v       < \a vecad_sparsity.n_set()
<!-- end sparse_store_op -->
*/
template <class Vector_set>
inline void reverse_sparse_jacobian_store_op(
	OpCode             op              ,
	const addr_t*      arg             , 
	size_t             num_combined    ,
	const size_t*      combined        ,
	Vector_set&        var_sparsity    ,
	Vector_set&        vecad_sparsity  )
{
	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) == 0 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_combined );
	size_t i_v = combined[ arg[0] - 1 ];
	CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < var_sparsity.n_set() );

	var_sparsity.binary_union(arg[2], arg[2], i_v, vecad_sparsity);

	return;
}

/*!
Reverse mode sparsity operations for StpvOp and StvvOp

This routine is given the sparsity patterns for
G(v[x], y , w , u ... )
and it uses them to compute the sparsity patterns for 
\verbatim
	H(y , w , u , ... ) = G[ v[x], y , w , u , ... ]
\endverbatim

<!-- replace sparse_store_op -->
The C++ source code corresponding to this operation is
\verbatim
	v[x] = y
\endverbatim
where v is a VecAD<Base> vector, x is an AD<Base> object,
and y is AD<Base> or Base objects. 
We define the index corresponding to v[x] by
\verbatim
	i_v_x = combined[ arg[0] + i_vec ]
\endverbatim
where i_vec is defined under the heading \a arg[1] below:

\tparam Vector_set
is the type used for vectors of sets. It can be either
\c sparse_pack, \c sparse_set, or \c sparse_list.

\param op
is the code corresponding to this operator; i.e., StpvOp or StvvOp
(only used for error checking).

\param arg
\n
\a arg[0]
is the offset corresponding to this VecAD vector in the combined array.
\n
\n 
\a arg[2]
\n
The set with index \a arg[2] in \a var_sparsity 
is the sparsity pattern corresponding to y.
(Note that \a arg[2] > 0 because y is a variable.) 

\param num_combined
is the total number of elements in the VecAD address array.

\param combined
\a combined [ arg[0] - 1 ]
is the index of the set in \a vecad_sparsity corresponding
to the sparsity pattern for the vector v.
We use the notation i_v below which is defined by
\verbatim
	i_v = combined[ \a arg[0] - 1 ]
\endverbatim

\param var_sparsity
The set  with index \a arg[2] in \a var_sparsity 
is the sparsity pattern for y.
This is an input for forward mode operations.
For reverse mode operations:
The sparsity pattern for v is added to the spartisy pattern for y.

\param vecad_sparsity
The set with index \a i_v in \a vecad_sparsity 
is the sparsity pattern for v.
This is an input for reverse mode operations.
For forward mode operations, the sparsity pattern for y is added
to the sparsity pattern for the vector v.

\par Checked Assertions 
\li NumArg(op) == 3
\li NumRes(op) == 0
\li 0 <  \a arg[0]
\li \a arg[0] < \a num_combined
\li \a arg[2] < \a var_sparsity.n_set()
\li i_v       < \a vecad_sparsity.n_set()
<!-- end sparse_store_op -->

\param var_jacobian
\a var_jacobian[ \a arg[2] ] 
is false (true) if the Jacobian of G with respect to y is always zero 
(may be non-zero).

\param vecad_jacobian
\a vecad_jacobian[i_v] 
is false (true) if the Jacobian with respect to x is always zero 
(may be non-zero).
On input, it corresponds to the function G,
and on output it corresponds to the function H.
*/
template <class Vector_set>
inline void reverse_sparse_hessian_store_op(
	OpCode             op           ,
	const addr_t*      arg          , 
	size_t             num_combined ,
	const size_t*      combined     ,
	Vector_set&        var_sparsity ,
	Vector_set&        vecad_sparsity ,
	bool*              var_jacobian   ,
	bool*              vecad_jacobian )
{
	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) == 0 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_combined );
	size_t i_v = combined[ arg[0] - 1 ];
	CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < var_sparsity.n_set() );

	var_sparsity.binary_union(arg[2], arg[2], i_v, vecad_sparsity);

	var_jacobian[ arg[2] ] |= vecad_jacobian[i_v];

	return;
}

/*! \} */
} // END_CPPAD_NAMESPACE
# endif
