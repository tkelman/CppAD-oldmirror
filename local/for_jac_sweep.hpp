/* $Id$ */
# ifndef CPPAD_FOR_JAC_SWEEP_INCLUDED
# define CPPAD_FOR_JAC_SWEEP_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/local/connection.hpp>
CPPAD_BEGIN_NAMESPACE

/*!
\file for_jac_sweep.hpp
Compute Forward mode Jacobian sparsity patterns.
*/

/*!
\def CPPAD_FOR_JAC_SWEEP_TRACE
This value is either zero or one. 
Zero is the normal operational value.
If it is one, a trace of every for_jac_sweep computation is printed.
*/
# define CPPAD_FOR_JAC_SWEEP_TRACE 0

/*!
Given the sparsity pattern for the independent variables,
ForJacSweep computes the sparsity pattern for all the other variables.

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\tparam Pack
is the type used to pack the sparsity pattern bit values; i.e.,
there is more that one bit per Pack value.

\param n
is the number of independent variables on the tape.

\param numvar
is the total number of variables on the tape; i.e.,
\a play->num_rec_var().
This is also the number of from nodes in \a var_sparsity.

\param play
The information stored in \a play
is a recording of the operations corresponding to a function
\f[
	F : {\bf R}^n \rightarrow {\bf R}^m
\f]
where \f$ n \f$ is the number of independent variables
and \f$ m \f$ is the number of dependent variables.
The object \a play is effectly constant.
It is not declared const because while playing back the tape
the object \a play holds information about the currentl location
with in the tape and this changes during playback.

\param var_sparsity
\b Input: For j = 1 , ... , \a n, 
the sparsity pattern for the independent variable with index (j-1)
corresponds to the from node with index j in \a var_sparsity.
\n
\n
\b Output: For i = \a n + 1 , ... , \a numvar - 1,
the sparsity pattern for the variable with index i on the tape
corresponds to the from node with index i in \a var_sparsity.
*/


template <class Base, class Pack>
void ForJacSweep(
	size_t                n            ,
	size_t                numvar       ,
	player<Base>*         play         ,
	connection<Pack>&     var_sparsity )
{
	OpCode           op;
	size_t         i_op;
	size_t        i_var;

	const size_t   *arg = 0;
	const size_t *arg_0 = 0;

	size_t            i, j, k;

	// check numvar argument
	CPPAD_ASSERT_UNKNOWN( play->num_rec_var() == numvar );

	// set the number of operators
	const size_t numop_m1 = play->num_rec_op() - 1;

        // length of the parameter vector (used by CppAD assert macros)
        const size_t num_par = play->num_rec_par();

	// vecad_sparsity contains a sparsity pattern from each VecAD object
	// to all the other variables.
	// vecad_ind maps a VecAD index (the beginning of the
	// VecAD object) to its from index in vecad_sparsity
	size_t n_to            = var_sparsity.n_to();
	size_t num_vecad_ind   = play->num_rec_vecad_ind();
	size_t num_vecad_vec   = play->num_rec_vecad_vec();
	connection<Pack> vecad_sparsity;
	vecad_sparsity.resize(num_vecad_vec, n_to);
	size_t* vecad_ind      = CPPAD_NULL;
	if( num_vecad_vec > 0 )
	{	size_t length;
		vecad_ind     = CPPAD_TRACK_NEW_VEC(num_vecad_ind, vecad_ind);
		j             = 0;
		for(i = 0; i < num_vecad_vec; i++)
		{	// length of this VecAD
			length   = play->GetVecInd(j);
			// set to proper index for this VecAD
			vecad_ind[j] = i; 
			for(k = 1; k <= length; k++)
				vecad_ind[j+k] = num_vecad_vec; // invalid index
			// start of next VecAD
			j       += length + 1;
		}
		CPPAD_ASSERT_UNKNOWN( j == play->num_rec_vecad_ind() );
	}

# if CPPAD_FOR_JAC_SWEEP_TRACE
	std::cout << std::endl;
	CppAD::vector<bool> z_value(n_to);
# endif

	// skip the NonOp at the beginning of the recording
        play->start_forward(op, arg, i_op, i_var);
        arg_0 = arg;
	while(i_op < numop_m1)
	{
		// this op
		play->next_forward(op, arg, i_op, i_var);
		CPPAD_ASSERT_UNKNOWN( (i_op > n)  | (op == InvOp) );  
		CPPAD_ASSERT_UNKNOWN( (i_op <= n) | (op != InvOp) );  

		// rest of information depends on the case
		switch( op )
		{
			case AbsOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], var_sparsity
			);
			break;
			// -------------------------------------------------

			case AddvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_binary_op(
				i_var, arg, var_sparsity
			);
			break;
			// -------------------------------------------------

			case AddpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[1], var_sparsity
			);
			break;
			// -------------------------------------------------

			case AddvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], var_sparsity
			);
			break;
			// -------------------------------------------------

			case AcosOp:
			// acos(x) and sqrt(1 - x * x) are computed in pairs
			// but ivar + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], var_sparsity
			);
			break;
			// -------------------------------------------------

			case AsinOp:
			// asin(x) and sqrt(1 - x * x) are computed in pairs
			// but ivar + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], var_sparsity
			);
			break;
			// -------------------------------------------------

			case AtanOp:
			// atan(x) and 1 + x * x must be computed in pairs
			// but ivar + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], var_sparsity
			);
			break;
			// -------------------------------------------------

			case CExpOp:
			forward_sparse_jacobian_cond_op(
				i_var, arg, num_par, var_sparsity
			);
			break;
			// ---------------------------------------------------

			case ComOp:
			CPPAD_ASSERT_NARG_NRES(op, 4, 0);
			CPPAD_ASSERT_UNKNOWN( arg[1] > 1 );
			break;
			// --------------------------------------------------

			case CosOp:
			// cosine and sine must come in pairs
			// but ivar + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], var_sparsity
			);
			break;
			// ---------------------------------------------------

			case CoshOp:
			// hyperbolic cosine and sine must come in pairs
			// but ivar + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], var_sparsity
			);
			break;
			// -------------------------------------------------

			case DisOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			var_sparsity.clear(i_var);
			break;
			// -------------------------------------------------

			case DivvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_binary_op(
				i_var, arg, var_sparsity
			);
			break;
			// -------------------------------------------------

			case DivpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[1], var_sparsity
			);
			break;
			// -------------------------------------------------

			case DivvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], var_sparsity
			);
			break;
			// -------------------------------------------------

			case ExpOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], var_sparsity
			);
			break;
			// -------------------------------------------------

			case InvOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 1);
			// sparsity pattern is already defined
			break;
			// -------------------------------------------------

			case LdpOp:
			forward_sparse_load_op(
				op,
				i_var,
				arg,
				num_vecad_ind,
				vecad_ind,
				var_sparsity,
				vecad_sparsity
			);
			break;
			// -------------------------------------------------

			case LdvOp:
			forward_sparse_load_op(
				op,
				i_var,
				arg,
				num_vecad_ind,
				vecad_ind,
				var_sparsity,
				vecad_sparsity
			);
			break;
			// -------------------------------------------------

			case LogOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], var_sparsity
			);
			break;
			// -------------------------------------------------

			case MulvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_binary_op(
				i_var, arg, var_sparsity
			);
			break;
			// -------------------------------------------------

			case MulpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[1], var_sparsity
			);
			break;
			// -------------------------------------------------

			case MulvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], var_sparsity
			);
			break;
			// -------------------------------------------------

			case NonOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 1);
			var_sparsity.clear(i_var);
			break;

			case ParOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			var_sparsity.clear(i_var);
			break;
			// -------------------------------------------------

			case PowvpOp:
			// Pow operator is a special case where final result
			// comes at the end of the three variables
			CPPAD_ASSERT_NARG_NRES(op, 2, 3);
			forward_sparse_jacobian_unary_op(
				i_var + 2, arg[0], var_sparsity
			);
			break;
			// -------------------------------------------------

			case PowpvOp:
			// Pow operator is a special case where final result
			// comes at the end of the three variables
			CPPAD_ASSERT_NARG_NRES(op, 2, 3);
			forward_sparse_jacobian_unary_op(
				i_var + 2, arg[1], var_sparsity
			);
			break;
			// -------------------------------------------------

			case PowvvOp:
			// Pow operator is a special case where final result
			// comes at the end of the three variables
			CPPAD_ASSERT_NARG_NRES(op, 2, 3);
			forward_sparse_jacobian_binary_op(
				i_var + 2, arg, var_sparsity
			);
			break;
			// -------------------------------------------------

			case PripOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 0);
			var_sparsity.clear(i_var);
			break;
			// -------------------------------------------------

			case PrivOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 0);
			break;
			// -------------------------------------------------

			case SinOp:
			// sine and cosine must come in pairs
			// but ivar + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], var_sparsity
			);
			break;
			// -------------------------------------------------

			case SinhOp:
			// hyperbolic sine and cosine must come in pairs
			// but ivar + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], var_sparsity
			);
			break;
			// -------------------------------------------------

			case SqrtOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], var_sparsity
			);
			break;
			// -------------------------------------------------

			case StppOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			// storing a parameter does not affect vector sparsity
			break;
			// -------------------------------------------------

			case StpvOp:
			forward_sparse_store_op(
				op,
				arg,
				num_vecad_ind,
				vecad_ind,
				var_sparsity,
				vecad_sparsity
			);
			break;
			// -------------------------------------------------

			case StvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			// storing a parameter does not affect vector sparsity
			break;
			// -------------------------------------------------

			case StvvOp:
			forward_sparse_store_op(
				op,
				arg,
				num_vecad_ind,
				vecad_ind,
				var_sparsity,
				vecad_sparsity
			);
			break;
			// -------------------------------------------------

			case SubvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_binary_op(
				i_var, arg, var_sparsity
			);
			break;
			// -------------------------------------------------

			case SubpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[1], var_sparsity
			);
			break;
			// -------------------------------------------------

			case SubvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			forward_sparse_jacobian_unary_op(
				i_var, arg[0], var_sparsity
			);
			break;
			// -------------------------------------------------

			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
# if CPPAD_FOR_JAC_SWEEP_TRACE
		// value for this variable
		for(j = 0; j < n_to; j++)
			z_value[j] = var_sparsity.get_element(i_var, j);
		printOp(
			std::cout,
			play,
			i_var,
			op,
			arg,
			1,
			&z_value,
			0,
			(CppAD::vector<bool> *) CPPAD_NULL
		);
	}
	std::cout << std::endl;
# else
	}
# endif
	CPPAD_ASSERT_UNKNOWN( (i_var + NumRes(op) ) == play->num_rec_var() );

	if( vecad_ind != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC(vecad_ind);

	return;
}

# undef CPPAD_FOR_JAC_SWEEP_TRACE

CPPAD_END_NAMESPACE
# endif
