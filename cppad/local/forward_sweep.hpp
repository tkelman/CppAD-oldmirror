/* $Id$ */
# ifndef CPPAD_FORWARD_SWEEP_INCLUDED
# define CPPAD_FORWARD_SWEEP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

CPPAD_BEGIN_NAMESPACE
/*!
\defgroup forward_sweep_hpp forward_sweep.hpp
\{
\file forward_sweep.hpp
Compute zero order forward mode Taylor coefficients.
*/

/*!
\def CPPAD_FORWARD_SWEEP_TRACE
This value is either zero or one. 
Zero is the normal operational value.
If it is one, a trace of every forward_sweep computation is printed.
*/
# define CPPAD_FORWARD_SWEEP_TRACE 0

/*!
Compute arbitrary order forward mode Taylor coefficients.

\tparam Base
base type for the operator; i.e., this operation sequence was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param s_out
Is the stream where output corresponding to \c PriOp operations will
be written.

\param print
If \a print is false,
suppress the output that is otherwise generated by the PriOp instructions.

\param q
is the lowest order of the Taylor coefficients
that are computed during this call. 

\param p
is the highest order of the Taylor coefficients
that are computed during this call. 

\param n
is the number of independent variables on the tape.

\param numvar
is the total number of variables on the tape.
This is also equal to the number of rows in the matrix \a Taylor; i.e.,
Rec->num_rec_var().

\param Rec
The information stored in \a Rec
is a recording of the operations corresponding to the function
\f[
	F : {\bf R}^n \rightarrow {\bf R}^m
\f]
where \f$ n \f$ is the number of independent variables and
\f$ m \f$ is the number of dependent variables.
\n
\n
The object \a Rec is effectly constant.
There are two exceptions to this.
The first exception is that while palying back the tape
the object \a Rec holds information about the current location
with in the tape and this changes during palyback. 
The second exception is the fact that the 
zero order ( \a p = 0 ) versions of the VecAD operators LdpOp and LdvOp 
modify the corresponding \a op_arg values returned by 
\ref player::next_forward and \ref player::next_reverse; see the
\link load_op.hpp LdpOp and LdvOp \endlink operations.

\param J
Is the number of columns in the coefficient matrix \a Taylor.
This must be greater than or equal \a p + 1.

\param Taylor
\b Input: For j = 1 , ... , \a n, and for k = 0 , ... , \a p,
\a Taylor [ j * J + k ]
is the k-th order Taylor coefficient corresponding to 
variable with index j on the tape 
(independent variable with index (j-1) in the independent variable vector).
\n
\n
\b Output: For i = \a n + 1, ... , \a numvar - 1, and for k = 0 , ... , \a p,
\a Taylor [ i * J + k ]
is the k-th order Taylor coefficient for the variable with 
index i on the tape.

\a return
If \a p is not zero, the return value is zero.
If \a p is zero,
the return value is equal to the number of ComOp operations
that have a different result from when the information in 
\a Rec was recorded.
(Note that if NDEBUG is true, there are no ComOp operations
in Rec and hence this return value is always zero.)
*/

template <class Base>
size_t forward_sweep(
	std::ostream&         s_out,
	const bool            print,
	const size_t          q,
	const size_t          p,
	const size_t          n,
	const size_t          numvar,
	player<Base>         *Rec,
	const size_t         J,
	Base                 *Taylor
)
{	CPPAD_ASSERT_UNKNOWN( J >= p + 1 );
	CPPAD_ASSERT_UNKNOWN( q <= p );

	// op code for current instruction
	OpCode           op;

	// index for current instruction
	size_t         i_op;

	// next variables 
	size_t        i_var;

	// arg (not as a constant)
	addr_t*         non_const_arg = CPPAD_NULL;

	// arg (as a constant)
	const addr_t*   arg = CPPAD_NULL;

	// temporary indices
	size_t i, ell;

	// initialize the comparision operator (ComOp) counter
	size_t compareCount = 0;

	// if this is an order zero calculation, initialize vector indices
	pod_vector<size_t> VectorInd;  // address for each element
	pod_vector<bool>   VectorVar;  // is element a variable
	i = Rec->num_rec_vecad_ind();
	if( i > 0 )
	{	VectorInd.extend(i);
		VectorVar.extend(i);
		while(i--)
		{	VectorInd[i] = Rec->GetVecInd(i);
			VectorVar[i] = false;
		}
	}

	// Work space used by UserOp. Note User assumes q = p.
	const size_t user_p1 = p+1;  // number of orders for this user calculation
	vector<bool> user_vx;        // empty vector
	vector<bool> user_vy;        // empty vector
	vector<Base> user_tx;        // argument vector Taylor coefficients 
	vector<Base> user_ty;        // result vector Taylor coefficients 
	vector<size_t> user_iy;      // variable indices for results vector
	size_t user_index = 0;       // indentifier for this atomic operation
	size_t user_id    = 0;       // user identifier for this call to operator
	size_t user_i     = 0;       // index in result vector
	size_t user_j     = 0;       // index in argument vector
	size_t user_m     = 0;       // size of result vector
	size_t user_n     = 0;       // size of arugment vector
	// next expected operator in a UserOp sequence
	enum { user_start, user_arg, user_ret, user_end } user_state = user_start;

	// check numvar argument
	CPPAD_ASSERT_UNKNOWN( Rec->num_rec_var() == numvar );

	// length of the parameter vector (used by CppAD assert macros)
	const size_t num_par = Rec->num_rec_par();

	// pointer to the beginning of the parameter vector
	const Base* parameter = CPPAD_NULL;
	if( num_par > 0 )
		parameter = Rec->GetPar();

	// length of the text vector (used by CppAD assert macros)
	const size_t num_text = Rec->num_rec_text();

	// pointer to the beginning of the text vector
	const char* text = CPPAD_NULL;
	if( num_text > 0 )
		text = Rec->GetTxt(0);

	// skip the BeginOp at the beginning of the recording
	Rec->start_forward(op, arg, i_op, i_var);
	CPPAD_ASSERT_UNKNOWN( op == BeginOp );
# if CPPAD_FORWARD_SWEEP_TRACE
	std::cout << std::endl;
# endif
	while(op != EndOp)
	{
		// this op
		Rec->next_forward(op, arg, i_op, i_var);
		CPPAD_ASSERT_UNKNOWN( (i_op > n)  | (op == InvOp) );  
		CPPAD_ASSERT_UNKNOWN( (i_op <= n) | (op != InvOp) );  

		// action depends on the operator
		switch( op )
		{
			case AbsOp:
			forward_abs_op(q, p, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case AddvvOp:
			forward_addvv_op(q, p, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case AddpvOp:
			CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
			forward_addpv_op(q, p, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case AcosOp:
			// sqrt(1 - x * x), acos(x)
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_acos_op(q, p, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case AsinOp:
			// sqrt(1 - x * x), asin(x)
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_asin_op(q, p, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case AtanOp:
			// 1 + x * x, atan(x)
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_atan_op(q, p, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case CExpOp:
			forward_cond_op(
				q, p, i_var, arg, num_par, parameter, J, Taylor
			);
			break;
			// ---------------------------------------------------

			case ComOp:
			if( q == 0 ) forward_comp_op_0(
			compareCount, arg, num_par, parameter, J, Taylor
			);
			break;
			// ---------------------------------------------------

			case CosOp:
			// sin(x), cos(x)
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_cos_op(q, p, i_var, arg[0], J, Taylor);
			break;
			// ---------------------------------------------------

			case CoshOp:
			// sinh(x), cosh(x)
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_cosh_op(q, p, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case CSumOp:
			// CSumOp has a variable number of arguments and
			// next_forward thinks it one has one argument.
			// we must inform next_forward of this special case.
			Rec->forward_csum(op, arg, i_op, i_var);
			forward_csum_op(
				q, p, i_var, arg, num_par, parameter, J, Taylor
			);
			break;
			// -------------------------------------------------

			case DisOp:
			i = q;
			if( i == 0 )
			{	forward_dis_op_0(i_var, arg, J, Taylor);
				i++;
			}
			while(i <= p)
			{	Taylor[ i_var * J + i] = Base(0);
				i++;
			}
			break;
			// -------------------------------------------------

			case DivvvOp:
			forward_divvv_op(q, p, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case DivpvOp:
			CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
			forward_divpv_op(q, p, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case DivvpOp:
			CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
			forward_divvp_op(q, p, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case EndOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 0);
			break;
			// -------------------------------------------------

			case ExpOp:
			forward_exp_op(q, p, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case InvOp:
			CPPAD_ASSERT_UNKNOWN( NumArg(op) == 0 );
			break;
			// -------------------------------------------------

			case LdpOp:
			if( q == 0 )
			{	non_const_arg = Rec->forward_non_const_arg();
				forward_load_p_op_0(
					i_var, 
					non_const_arg, 
					num_par, 
					parameter, 
					J, 
					Taylor,
					Rec->num_rec_vecad_ind(),
					VectorVar.data(),
					VectorInd.data()
				);
				if( q < p )
					forward_load_op( op, q+1, p, i_var, arg, J, Taylor);
			}
			else
			{	forward_load_op( op, q, p, i_var, arg, J, Taylor);
			}
			break;
			// -------------------------------------------------

			case LdvOp:
			if( q == 0 )
			{	non_const_arg = Rec->forward_non_const_arg();
				forward_load_v_op_0(
					i_var, 
					non_const_arg, 
					num_par, 
					parameter, 
					J, 
					Taylor,
					Rec->num_rec_vecad_ind(),
					VectorVar.data(),
					VectorInd.data()
				);
				if( q < p )
					forward_load_op( op, q+1, p, i_var, arg, J, Taylor);
			}
			else
			{	forward_load_op( op, q, p, i_var, arg, J, Taylor);
			}
			break;
			// -------------------------------------------------

			case LogOp:
			forward_log_op(q, p, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case MulvvOp:
			forward_mulvv_op(q, p, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case MulpvOp:
			CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
			forward_mulpv_op(q, p, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case ParOp:
			i = q;
			if( i == 0 )
			{	forward_par_op_0(
					i_var, arg, num_par, parameter, J, Taylor
				);
				i++;
			}
			while(i <= p)
			{	Taylor[ i_var * J + i] = Base(0); 
				i++;
			}
			break;
			// -------------------------------------------------

			case PowvpOp:
			CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
			forward_powvp_op(q, p, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case PowpvOp:
			CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
			forward_powpv_op(q, p, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case PowvvOp:
			forward_powvv_op(q, p, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case PriOp:
			if( (q == 0) & print ) forward_pri_0(s_out,
				i_var, arg, num_text, text, num_par, parameter, J, Taylor
			);
			break;
			// -------------------------------------------------

			case SignOp:
			// sign(x)
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_sign_op(q, p, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case SinOp:
			// cos(x), sin(x)
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_sin_op(q, p, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case SinhOp:
			// cosh(x), sinh(x)
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_sinh_op(q, p, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case SqrtOp:
			forward_sqrt_op(q, p, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case StppOp:
			if( q == 0 )
			{	forward_store_pp_op_0(
					i_var, 
					arg, 
					num_par, 
					J, 
					Taylor,
					Rec->num_rec_vecad_ind(),
					VectorVar.data(),
					VectorInd.data()
				);
			}
			break;
			// -------------------------------------------------

			case StpvOp:
			if( q == 0 )
			{	forward_store_pv_op_0(
					i_var, 
					arg, 
					num_par, 
					J, 
					Taylor,
					Rec->num_rec_vecad_ind(),
					VectorVar.data(),
					VectorInd.data()
				);
			}
			break;
			// -------------------------------------------------

			case StvpOp:
			if( q == 0 )
			{	forward_store_vp_op_0(
					i_var, 
					arg, 
					num_par, 
					J, 
					Taylor,
					Rec->num_rec_vecad_ind(),
					VectorVar.data(),
					VectorInd.data()
				);
			}
			break;
			// -------------------------------------------------

			case StvvOp:
			if( q == 0 )
			{	forward_store_vv_op_0(
					i_var, 
					arg, 
					num_par, 
					J, 
					Taylor,
					Rec->num_rec_vecad_ind(),
					VectorVar.data(),
					VectorInd.data()
				);
			}
			break;
			// -------------------------------------------------

			case SubvvOp:
			forward_subvv_op(q, p, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case SubpvOp:
			CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
			forward_subpv_op(q, p, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case SubvpOp:
			CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
			forward_subvp_op(q, p, i_var, arg, parameter, J, Taylor);
			break;
			// -------------------------------------------------

			case TanOp:
			// tan(x)^2, tan(x)
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_tan_op(q, p, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case TanhOp:
			// tanh(x)^2, tanh(x)
			CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
			forward_tanh_op(q, p, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case UserOp:
			// start or end an atomic operation sequence
			CPPAD_ASSERT_UNKNOWN( NumRes( UserOp ) == 0 );
			CPPAD_ASSERT_UNKNOWN( NumArg( UserOp ) == 4 );
			if( user_state == user_start )
			{	user_index = arg[0];
				user_id    = arg[1];
				user_n     = arg[2];
				user_m     = arg[3];
				if(user_tx.size() != user_n * user_p1)
					user_tx.resize(user_n * user_p1);
				if(user_ty.size() != user_m * user_p1)
					user_ty.resize(user_m * user_p1);
				if(user_iy.size() != user_m)
					user_iy.resize(user_m);
				user_j     = 0;
				user_i     = 0;
				user_state = user_arg;
			}
			else
			{	CPPAD_ASSERT_UNKNOWN( user_state == user_end );
				CPPAD_ASSERT_UNKNOWN( user_index == size_t(arg[0]) );
				CPPAD_ASSERT_UNKNOWN( user_id    == size_t(arg[1]) );
				CPPAD_ASSERT_UNKNOWN( user_n     == size_t(arg[2]) );
				CPPAD_ASSERT_UNKNOWN( user_m     == size_t(arg[3]) );
				user_state = user_start;

				// call users function for this operation
				atomic_base<Base>* atom = 
					atomic_base<Base>::list(user_index);
				atom->forward(
					user_id, q, p, user_vx, user_vy, user_tx, user_ty
				);
				for(i = 0; i < user_m; i++) 
					if( user_iy[i] > 0 )
						for(ell = q; ell <= p; ell++)
							Taylor[ user_iy[i] * J + ell ] = 
								user_ty[ i * user_p1 + ell ];
			}
			break;

			case UsrapOp:
			// parameter argument in an atomic operation sequence
			CPPAD_ASSERT_UNKNOWN( q == p );
			CPPAD_ASSERT_UNKNOWN( user_state == user_arg );
			CPPAD_ASSERT_UNKNOWN( user_j < user_n );
			CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
			user_tx[user_j * user_p1 + 0] = parameter[ arg[0]];
			for(ell = 1; ell < user_p1; ell++)
				user_tx[user_j * user_p1 + ell] = Base(0);
			++user_j;
			if( user_j == user_n )
				user_state = user_ret;
			break;

			case UsravOp:
			// variable argument in an atomic operation sequence
			CPPAD_ASSERT_UNKNOWN( q == p );
			CPPAD_ASSERT_UNKNOWN( user_state == user_arg );
			CPPAD_ASSERT_UNKNOWN( user_j < user_n );
			CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) <= i_var );
			for(ell = 0; ell < user_p1; ell++)
				user_tx[user_j * user_p1 + ell] = Taylor[ arg[0] * J + ell];
			++user_j;
			if( user_j == user_n )
				user_state = user_ret;
			break;

			case UsrrpOp:
			// parameter result in an atomic operation sequence
			CPPAD_ASSERT_UNKNOWN( q == p );
			CPPAD_ASSERT_UNKNOWN( user_state == user_ret );
			CPPAD_ASSERT_UNKNOWN( user_i < user_m );
			user_iy[user_i] = 0;
			user_ty[user_i * user_p1 + 0] = parameter[ arg[0]];
			for(ell = 1; ell < q; ell++)
				user_ty[user_i * user_p1 + ell] = Base(0);
			user_i++;
			if( user_i == user_m )
				user_state = user_end;
			break;

			case UsrrvOp:
			// variable result in an atomic operation sequence
			CPPAD_ASSERT_UNKNOWN( q == p );
			CPPAD_ASSERT_UNKNOWN( user_state == user_ret );
			CPPAD_ASSERT_UNKNOWN( user_i < user_m );
			user_iy[user_i] = i_var;
			for(ell = 0; ell < q; ell++)
				user_ty[user_i * user_p1 + ell] = Taylor[ i_var * J + ell];
			user_i++;
			if( user_i == user_m )
				user_state = user_end;
			break;
			// -------------------------------------------------

			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
# if CPPAD_FORWARD_SWEEP_TRACE
		size_t       i_tmp  = i_var;
		Base*        Z_tmp  = Taylor + J * i_var;
		printOp(
			std::cout, 
			Rec,
			i_tmp,
			op, 
			arg,
			p + 1, 
			Z_tmp, 
			0, 
			(Base *) CPPAD_NULL
		);
	}
	std::cout << std::endl;
# else
	}
# endif
	CPPAD_ASSERT_UNKNOWN( user_state == user_start );
	CPPAD_ASSERT_UNKNOWN( i_var + 1 == Rec->num_rec_var() );

	return compareCount;
}

# undef CPPAD_FORWARD_SWEEP_TRACE

/*! \} */
CPPAD_END_NAMESPACE
# endif
