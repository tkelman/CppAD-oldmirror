/* $Id$ */
# ifndef CPPAD_REV_HES_SWEEP_INCLUDED
# define CPPAD_REV_HES_SWEEP_INCLUDED
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
\file rev_hes_sweep.hpp
Compute Reverse mode Hessian sparsity patterns.
*/

/*!
\def CPPAD_REV_HES_SWEEP_TRACE
This value is either zero or one. 
Zero is the normal operational value.
If it is one, a trace of every rev_hes_sweep computation is printed.
*/
# define CPPAD_REV_HES_SWEEP_TRACE 0

/*!
Given the forward Jacobian sparsity pattern for all the variables,
and the reverse Jacobian sparsity pattern for the dependent variables,
RevHesSweep computes the Hessian sparsity pattern for all the independent 
variables.

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
This is also the number of rows in the entire sparsity pattern 
\a rev_hes_sparse.

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

\param for_jac_sparse
For i = 0 , ... , \a numvar - 1, 
(for all the variables on the tape),
the forward Jacobian sparsity pattern for the variable with index i
corresponds to the from node with index i in \a for_jac_sparse.

\param RevJac
\b Input:
For i = 0, ... , \a numvar - 1 
the if the variable with index i on the tape is an dependent variable and
included in the Hessian, \a RevJac[ i ] is equal to all ones (~ \a Pack(0)),
otherwise it is equal to zero. 
\n
\n
\b Output: The values in \a RevJac upon return are not specified; i.e.,
it is used for temporary work space.

\param rev_hes_sparse
The reverse Hessian sparsity pattern for the variable with index i
corresponds to the from node with index i in \a rev_hes_sparse.
\n
\n
\b Input: For i = 0 , ... , \a numvar - 1  
the reverse Hessian sparsity pattern for the variable with index i is empty.
\n
\n
\b Output: For j = 1 , ... , \a n,
the reverse Hessian sparsity pattern for the independent dependent variable 
with index (j-1) is given by the from connections for the node with index j
in \a rev_hes_sparse. 
The values in the rest of \a rev_hes_sparse are not specified; i.e.,
they are used for temporary work space.
*/

template <class Base, class Pack>
void RevHesSweep(
	size_t                n,
	size_t                numvar,
	player<Base>         *play,
	connection<Pack>&     for_jac_sparse, // should be const
	Pack                 *RevJac,
	connection<Pack>&     rev_hes_sparse
)
{
	OpCode           op;
	size_t         i_op;
	size_t        i_var;

	const size_t   *arg = 0;

	// length of the parameter vector (used by CppAD assert macros)
	const size_t num_par = play->num_rec_par();

	size_t             i, j, k;

	// check numvar argument
	CPPAD_ASSERT_UNKNOWN( play->num_rec_var()     == numvar );
	CPPAD_ASSERT_UNKNOWN( for_jac_sparse.n_from() == numvar );
	CPPAD_ASSERT_UNKNOWN( rev_hes_sparse.n_from() == numvar );
	CPPAD_ASSERT_UNKNOWN( numvar > 0 );

	// number of to nodes in connections
	size_t n_to   = rev_hes_sparse.n_to();
	CPPAD_ASSERT_UNKNOWN( rev_hes_sparse.n_to() == n_to );

	// number of packed values per from node
	size_t npv = rev_hes_sparse.n_pack();
	CPPAD_ASSERT_UNKNOWN( for_jac_sparse.n_pack() == npv );

	// vecad_pattern contains a sparsity pattern for each VecAD object.
	// vecad maps a VecAD index (which corresponds to the beginning of the
	// VecAD object) to the vecad_pattern index for the VecAD object.
	size_t num_vecad_ind   = play->num_rec_vecad_ind();
	size_t num_vecad_vec   = play->num_rec_vecad_vec();
	connection<Pack> vecad_sparsity;
	vecad_sparsity.resize(num_vecad_vec, n_to);
	Pack* vecad_pattern = vecad_sparsity.data();
	size_t* vecad          = CPPAD_NULL;
	if( num_vecad_vec > 0 )
	{	size_t length;
		vecad         = CPPAD_TRACK_NEW_VEC(num_vecad_ind, vecad);
		j             = 0;
		for(i = 0; i < num_vecad_vec; i++)
		{	// length of this VecAD
			length   = play->GetVecInd(j);
			// set to proper index for this VecAD
			vecad[j] = i; 
			for(k = 1; k <= length; k++)
				vecad[j+k] = num_vecad_vec; // invalid index
			// start of next VecAD
			j       += length + 1;
		}
		CPPAD_ASSERT_UNKNOWN( j == play->num_rec_vecad_ind() );
	}

	// create connection objects using existing memory
	// (kludge for step by step conversion to using connection objects)
	Pack* RevHes = rev_hes_sparse.data();

	// Initialize
	play->start_reverse();
	i_op = 2;
# if CPPAD_REV_HES_SWEEP_TRACE
	std::cout << std::endl;
	CppAD::vector<bool> zf_value(n_to);
	CppAD::vector<bool> zh_value(n_to);
# endif
	while(i_op > 1)
	{
		// next op
		play->next_reverse(op, arg, i_op, i_var);
		CPPAD_ASSERT_UNKNOWN( (i_op > n)  | (op == InvOp) );
		CPPAD_ASSERT_UNKNOWN( (i_op <= n) | (op != InvOp) );

# if CPPAD_REV_HES_SWEEP_TRACE
		for(j = 0; j < n_to; j++)
		{	zf_value[j] = for_jac_sparse.get_element(i_var, j);
			zh_value[j] = for_jac_sparse.get_element(i_var, j);
		}

		printOp(
			std::cout, 
			play,
			i_var,
			op, 
			arg,
			1, 
			&zf_value, 
			1, 
			&zh_value
		);
# endif

		// rest of information depends on the case
		switch( op )
		{
			case AbsOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1)
			reverse_sparse_hessian_linear_unary_op(
			i_var, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case AddvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1)
			reverse_sparse_hessian_addsub_op(
			i_var, arg, RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case AddpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1)
			reverse_sparse_hessian_linear_unary_op(
			i_var, arg[1], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case AddvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1)
			reverse_sparse_hessian_linear_unary_op(
			i_var, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case AcosOp:
			// acos(x) and sqrt(1 - x * x) are computed in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2)
			reverse_sparse_hessian_nonlinear_unary_op(
			i_var, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case AsinOp:
			// asin(x) and sqrt(1 - x * x) are computed in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2)
			reverse_sparse_hessian_nonlinear_unary_op(
			i_var, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case AtanOp:
			// atan(x) and 1 + x * x must be computed in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2)
			reverse_sparse_hessian_nonlinear_unary_op(
			i_var, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------
			case CExpOp:
			reverse_sparse_hessian_cond_op(
				i_var, arg, num_par, RevJac, npv, RevHes
			);
			break;
			// ---------------------------------------------------

			case ComOp:
			CPPAD_ASSERT_NARG_NRES(op, 4, 0)
			CPPAD_ASSERT_UNKNOWN( arg[1] > 1 );
			break;
			// --------------------------------------------------

			case CosOp:
			// cosine and sine must come in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2)
			reverse_sparse_hessian_nonlinear_unary_op(
			i_var, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// ---------------------------------------------------

			case CoshOp:
			// hyperbolic cosine and sine must come in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2)
			reverse_sparse_hessian_nonlinear_unary_op(
			i_var, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case DisOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1)

			break;
			// -------------------------------------------------

			case DivvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1)
			reverse_sparse_hessian_div_op(
			i_var, arg, RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case DivpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1)
			reverse_sparse_hessian_nonlinear_unary_op(
			i_var, arg[1], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case DivvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1)
			reverse_sparse_hessian_linear_unary_op(
			i_var, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case ExpOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1)
			reverse_sparse_hessian_nonlinear_unary_op(
			i_var, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case InvOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 1)
			// Z is already defined
			break;
			// -------------------------------------------------

			case LdpOp:
			reverse_sparse_load_op(
				op,
				i_var,
				arg,
				num_vecad_ind,
				vecad,
				num_vecad_vec,
				npv,
				RevHes,
				vecad_pattern
			);
			break;
			// -------------------------------------------------

			case LdvOp:
			reverse_sparse_load_op(
				op,
				i_var,
				arg,
				num_vecad_ind,
				vecad,
				num_vecad_vec,
				npv,
				RevHes,
				vecad_pattern
			);
			break;
			// -------------------------------------------------

			case LogOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1)
			reverse_sparse_hessian_nonlinear_unary_op(
			i_var, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case MulvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1)
			reverse_sparse_hessian_mul_op(
			i_var, arg, RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case MulpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1)
			reverse_sparse_hessian_linear_unary_op(
			i_var, arg[1], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case MulvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1)
			reverse_sparse_hessian_linear_unary_op(
			i_var, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case NonOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 1)

			break;
			// -------------------------------------------------

			case ParOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1)

			break;
			// -------------------------------------------------

			case PowpvOp:
                        // Pow operator is a special case where final result
                        // comes at the end of the three variables
			CPPAD_ASSERT_NARG_NRES(op, 2, 3)
			reverse_sparse_hessian_nonlinear_unary_op(
			i_var+2, arg[1], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case PowvpOp:
                        // Pow operator is a special case where final result
                        // comes at the end of the three variables
			CPPAD_ASSERT_NARG_NRES(op, 2, 3)
			reverse_sparse_hessian_nonlinear_unary_op(
			i_var+2, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case PowvvOp:
                        // Pow operator is a special case where final result
                        // comes at the end of the three variables
			CPPAD_ASSERT_NARG_NRES(op, 2, 3)
                        reverse_sparse_hessian_pow_op(
			i_var+2, arg, RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case PripOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 0);
			break;
			// -------------------------------------------------

			case PrivOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 0);
			break;
			// -------------------------------------------------

			case SinOp:
			// sine and cosine must come in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2)
			reverse_sparse_hessian_nonlinear_unary_op(
			i_var, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case SinhOp:
			// sine and cosine must come in pairs
			// but i_var + 1 should only be used here
			CPPAD_ASSERT_NARG_NRES(op, 1, 2)
			reverse_sparse_hessian_nonlinear_unary_op(
			i_var, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case SqrtOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1)
			reverse_sparse_hessian_nonlinear_unary_op(
			i_var, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case StppOp:
			// sparsity cannot propagate through a parameter
			CPPAD_ASSERT_NARG_NRES(op, 3, 0)
			break;
			// -------------------------------------------------

			case StpvOp:
			reverse_sparse_store_op(
				op,
				arg,
				num_vecad_ind,
				vecad,
				num_vecad_vec,
				numvar,
				npv,
				RevHes,
				vecad_pattern
			);
			break;
			// -------------------------------------------------

			case StvpOp:
			// sparsity cannot propagate through a parameter
			CPPAD_ASSERT_NARG_NRES(op, 3, 0)
			break;
			// -------------------------------------------------

			case StvvOp:
			reverse_sparse_store_op(
				op,
				arg,
				num_vecad_ind,
				vecad,
				num_vecad_vec,
				numvar,
				npv,
				RevHes,
				vecad_pattern
			);
			break;
			// -------------------------------------------------

			case SubvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1)
			reverse_sparse_hessian_addsub_op(
			i_var, arg, RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case SubpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1)
			reverse_sparse_hessian_linear_unary_op(
			i_var, arg[1], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			case SubvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1)
			reverse_sparse_hessian_linear_unary_op(
			i_var, arg[0], RevJac, for_jac_sparse, rev_hes_sparse
			);
			break;
			// -------------------------------------------------

			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
	}
	CPPAD_ASSERT_UNKNOWN( i_op == 1 );
	CPPAD_ASSERT_UNKNOWN( play->GetOp(i_op-1) == NonOp );
	CPPAD_ASSERT_UNKNOWN( i_var == NumRes(NonOp)  );

	if( vecad != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC(vecad);
	return;
}

# undef CPPAD_REV_HES_SWEEP_TRACE

CPPAD_END_NAMESPACE
# endif
