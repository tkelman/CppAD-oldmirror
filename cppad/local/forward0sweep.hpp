/* $Id$ */
# ifndef CPPAD_FORWARD0SWEEP_INCLUDED
# define CPPAD_FORWARD0SWEEP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin forward0sweep$$ $comment CppAD Developer Documentation$$
$spell
	Var
	numvar
	bool
	Prip
	Priv
	Inv
	Num
	Len
	const
	Taylor
	CppAD
	zs
	op
	Ind
$$

$section Zero Order Forward Computation of Taylor Coefficients$$
$index forward0sweep$$
$index mode, forward$$
$index forward, mode$$
$index derivative, forward$$
$index Taylor coefficient, forward$$


$head Syntax$$
$syntax%size_t forward0sweep(
	bool %print%,
	size_t %numvar%,
	player<%Base%> *%Rec%,
	size_t %J%,
	Base *%Taylor%,
)%$$


$head Return Value$$
The return value is equal to the number of
$syntax%AD<%Base%>%$$ comparison operations have a different result
from when the information in $italic Rec$$ was recorded.


$head Rec$$
The information stored in $italic Rec$$
is a recording of the operations corresponding to a function
$latex \[
	F : B^n \rightarrow B^m
\] $$

$head print$$
If $italic print$$ is false,
suppress the output that is otherwise generated 
by the PripOp and PrivOp instructions.


$head numvar$$
is the number of rows in the matrix $italic Taylor$$.
It must also be equal to $syntax%%Rec%->num_rec_var()%$$.


$head J$$
Is the number of columns in the coefficient matrix $italic Taylor$$.
This must be greater than or equal $latex 1$$.


$head On Input$$

$subhead Independent Variables and Operators$$
The independent variable records come first.
For $latex i = 1, \ldots , n$$ 
$table
	$bold field$$ $cnext 
	$bold Value$$          
$rnext
	$syntax%%Taylor%[%0% * %J%]%$$      $cnext 
	this index zero variable is not used
$rnext
	$syntax%%Rec%->GetOp(0)%$$                $cnext 
	the operator with index zero must be a $code NonOp$$
$rnext
	$syntax%%Taylor%[%i% * %J%]%$$      $cnext 
	zero order Taylor coefficient for independent variable 
	with index $italic i$$   
$rnext
	$syntax%%Rec%->GetOp(%i%)%$$              $cnext 
	the operator with index $italic i$$ must be a $code InvOp$$
$tend

$subhead Other Variables and Operators$$
The other variables follow the independent variables.
For $latex i = n+1, \ldots , numvar-1$$,
$table
	$bold field$$ $cnext 
	$bold Value$$          
$rnext
	$syntax%%Taylor%[%i% * %J%]%$$      $cnext 
	value of the variable with index $italic i$$     
$rnext
	$syntax%%Rec%->GetOp(%i%)%$$              $cnext 
	any operator except for $code InvOp$$ 
$tend

$head On Output$$

$subhead Rec$$
None of the values stored in $italic Rec$$ are modified.

$subhead Independent Variables$$
For $latex i = 1, \ldots , n$$ and $latex j = 0 , \ldots , J-1$$,
$syntax%%Taylor%[%i% * %J% + %j%]%$$ is not modified.

$subhead Other Variables$$
For $latex i = n+1, \ldots , numvar-1$$, 
$syntax%%Taylor%[%i% * %J%]%$$ is set equal to the
zero order Taylor coefficient for the variable with index $italic i$$.


$end
------------------------------------------------------------------------------
*/
# define CPPAD_FORWARD0SWEEP_TRACE 0
# ifdef NDEBUG
# define CPPAD_GET_PAR(i) (P_0 + i)
# else
# define CPPAD_GET_PAR(i) Rec->GetPar(i)
# endif

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
size_t forward0sweep(
	bool                  print,
	size_t                numvar,
	player<Base>         *Rec,
	size_t                J,
	Base                 *Taylor
)
{
	// some constants
	Base one(1);

	size_t     numop_m1;
	OpCode           op;
	size_t         i_op;
	size_t        i_var;
	size_t        i_arg;

	const size_t   *arg = 0;
	const size_t *arg_0 = 0;
	const Base       *P = 0;
	const Base       *X = 0;
	const Base       *Y = 0;

	// used by CExp operator (left and right also used by Com operator)
	const Base  *left = 0, *right = 0, *trueCase = 0, *falseCase = 0;

	// used by Com operator
	bool result;

	Base             *Z = 0;
	Base             *W = 0;
	Base             *U = 0;

	size_t          i;
	size_t          len;
	size_t          n_res = 0;
	size_t          n_arg = 0;


	// initialize the comparision operator (ComOp) counter
	size_t compareCount = 0;

	// This is an order zero calculation, initialize vector indices
	size_t *VectorInd = CPPAD_NULL;  // address for each element
	bool   *VectorVar = CPPAD_NULL;  // is element a variable
	i = Rec->num_rec_vecad_ind();
	if( i > 0 )
	{	VectorInd = CPPAD_TRACK_NEW_VEC(i, VectorInd);
		VectorVar = CPPAD_TRACK_NEW_VEC(i, VectorVar);
		while(i--)
		{	VectorInd[i] = Rec->GetVecInd(i);
			VectorVar[i] = false;
		}
	}

	const Base     *P_0 = 0;
	if( Rec->num_rec_par() > 0 )
		P_0   = Rec->GetPar(0);



	// check numvar argument
	CPPAD_ASSERT_UNKNOWN( Rec->num_rec_var() == numvar );

	// set the number of operators
	numop_m1 = Rec->num_rec_op() - 1;

	// skip the NonOp at the beginning of the recording
	Rec->start_forward(op, arg, i_op, i_var);
	arg_0 = arg;
	while(i_op < numop_m1)
	{
		// this op
		Rec->next_forward(op, arg, i_op, i_var);

		// number of variables
		n_res  = NumRes(op);

		// index field values for this op
		n_arg  = NumArg(op);

		// value of z for this op
		Z      = Taylor + i_var * J;

		// rest of information depends on the case

		switch( op )
		{
			case AbsOp:
			forward_abs_op_0(i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case AddvvOp:
			n_res = 1;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			X = Taylor + arg[0] * J;
			Y = Taylor + arg[1] * J;
			Z[0] = X[0] + Y[0];
			break;
			// -------------------------------------------------

			case AddpvOp:
			n_res = 1;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			P = CPPAD_GET_PAR(arg[0]);
			Y = Taylor + arg[1] * J;
			Z[0] = P[0] + Y[0];
			break;
			// -------------------------------------------------

			case AddvpOp:
			n_res = 1;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );

			X = Taylor + arg[0] * J;
			P = CPPAD_GET_PAR(arg[1]);
			Z[0] = X[0] + P[0];
			break;
			// -------------------------------------------------

			case AcosOp:
			// variables: acos(x),  sqrt(1 - x * x) 
			n_arg = 1;
			n_res = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in variable record
			W = Taylor + (i_var+1) * J;
			X = Taylor + arg[0] * J;
			W[0] = sqrt( one - X[0] * X[0] );
			Z[0] = acos( X[0] );
			break;
			// -------------------------------------------------

			case AsinOp:
			// variables: asin(x),  sqrt(1 - x * x) 
			n_arg = 1;
			n_res = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in variable record
			W = Taylor + (i_var+1) * J;
			X = Taylor + arg[0] * J;
			W[0] = sqrt( one - X[0] * X[0] );
			Z[0] = asin( X[0] );
			break;
			// -------------------------------------------------

			case AtanOp:
			// variables: atan(x),  1 + x * x 
			n_arg = 1;
			n_res = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in variable record
			W = Taylor + (i_var+1) * J;
			X = Taylor + arg[0] * J;
			W[0] = one + X[0] * X[0];
			Z[0] = atan( X[0] );
			break;
			// -------------------------------------------------

			case CExpOp:
			n_res = 1;
			n_arg = 6;
			CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );
			if( arg[1] & 1 )
				left = Taylor + arg[2] * J;
			else	left = CPPAD_GET_PAR(arg[2]);
			if( arg[1] & 2 )
				right = Taylor + arg[3] * J;
			else	right = CPPAD_GET_PAR(arg[3]);
			// d == 0
			{	if( arg[1] & 4 )
					trueCase = Taylor + arg[4] * J;
				else	trueCase = CPPAD_GET_PAR(arg[4]);
				if( arg[1] & 8 )
					falseCase = Taylor + arg[5] * J;
				else	falseCase = CPPAD_GET_PAR(arg[5]);
			}
			Z[0] = CondExpOp(
				CompareOp( arg[0] ),
				*left,
				*right,
				*trueCase,
				*falseCase
			);
			break;
			// ---------------------------------------------------

			case ComOp:
			n_res = 0;
			n_arg = 4;
			CPPAD_ASSERT_UNKNOWN( arg[1] > 1 );
			// d == 0
			{	if( arg[1] & 1 )
					result = true;
				else	result = false;
				if( arg[1] & 2 )
					left = Taylor + arg[2] * J;
				else	left = CPPAD_GET_PAR(arg[2]);
				if( arg[1] & 4 )
					right = Taylor + arg[3] * J;
				else	right = CPPAD_GET_PAR(arg[3]);
				switch( CompareOp( arg[0] ) )
				{	case CompareLt:
					compareCount += ( result != 
					LessThanZero(*left - *right) );
					break;

					case CompareLe:
					compareCount += ( result !=
					LessThanOrZero(*left - *right) );
					break;

					case CompareEq:
					compareCount += ( result != 
					(*left == *right) );
					break;

					case CompareGe:
					compareCount += ( result !=
					GreaterThanOrZero(*left - *right) );
					break;

					case CompareGt:
					compareCount += ( result != 
					GreaterThanZero(*left - *right) );
					break;

					case CompareNe:
					compareCount += ( result != 
					(*left != *right) );
					break;

					default:
					CPPAD_ASSERT_UNKNOWN(0);
				}
			}
			break;
			// ---------------------------------------------------

			case CosOp:
			// variables: cos(x), sin(x)
			n_arg = 1;
			n_res = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in variable record
			W = Taylor + (i_var+1) * J;
			X = Taylor + arg[0] * J;
			W[0] = sin( X[0] );
			Z[0] = cos( X[0] );
			break;
			// ---------------------------------------------------

			case CoshOp:
			// variables: cosh(x), sinh(x)
			n_arg = 1;
			n_res = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in variable record
			W = Taylor + (i_var+1) * J;
			X = Taylor + arg[0] * J;
			W[0] = sinh( X[0] );
			Z[0] = cosh( X[0] );
			break;
			// -------------------------------------------------

			case DisOp:
			n_res = 1;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			// d == 0
			{	X   = Taylor + arg[0] * J;
				Z[0] = ADDiscrete<Base>::Eval(arg[1], X[0]);
			}
			break;
			// -------------------------------------------------

			case DivvvOp:
			n_res = 1;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			X = Taylor + arg[0] * J;
			Y = Taylor + arg[1] * J;
			Z[0] = X[0] / Y[0];
			break;
			// -------------------------------------------------

			case DivpvOp:
			n_res = 1;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			Y = Taylor + arg[1] * J;
			P = CPPAD_GET_PAR(arg[0]);
			Z[0] = P[0] / Y[0];
			break;
			// -------------------------------------------------

			case DivvpOp:
			n_res = 1;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );

			P = CPPAD_GET_PAR(arg[1]);
			X = Taylor + arg[0] * J;
			Z[0] = X[0] / P[0];
			break;
			// -------------------------------------------------

			case ExpOp:
			forward_exp_op_0(i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case InvOp:
			n_res = 1;
			n_arg = 0;
			break;
			// -------------------------------------------------

			case LdpOp:
			n_res = 1;
			n_arg = 3;
			
			CPPAD_ASSERT_UNKNOWN( arg[0] > 0 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < Rec->num_rec_vecad_ind() );
			CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
			CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );

			// d == 0
			{	i   = arg[1];
				CPPAD_ASSERT_UNKNOWN( 
					i < VectorInd[arg[0] - 1] 
				);
				CPPAD_ASSERT_UNKNOWN( 
					i + arg[0] < Rec->num_rec_vecad_ind() 
				);

				if( VectorVar[ i + arg[0] ] )
				{	i     = VectorInd[ i + arg[0] ];
					i_arg = arg - arg_0;
					Rec->ReplaceInd(i_arg + 2, i);
					CPPAD_ASSERT_UNKNOWN(i > 0 );
					CPPAD_ASSERT_UNKNOWN( i < i_var );
					Y     = Taylor + i * J;
					Z[0]  = Y[0];
				}
				else
				{	i     = VectorInd[ i + arg[0] ];
					i_arg = arg - arg_0;
					Rec->ReplaceInd(i_arg + 2, 0);
					Z[0] = *(CPPAD_GET_PAR(i));
					i    = 0;
				}
			}
			break;
			// -------------------------------------------------

			case LdvOp:
			n_res = 1;
			n_arg = 3;
			
			CPPAD_ASSERT_UNKNOWN( arg[0] > 0 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < Rec->num_rec_vecad_ind() );
			CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
			CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );

			// d == 0
			{
				X   = Taylor + arg[1] * J;
				i   = Integer( X[0] );
				len = VectorInd[ arg[0] - 1 ];
				CPPAD_ASSERT_KNOWN( 
					i < len,
					"VecAD index value >= vector length"
				);
				CPPAD_ASSERT_UNKNOWN( 
					i + arg[0] < Rec->num_rec_vecad_ind() 
				);

				if( VectorVar[ i + arg[0] ] )
				{	i     = VectorInd[ i + arg[0] ];
					i_arg = arg - arg_0;
					Rec->ReplaceInd(i_arg + 2, i);
					CPPAD_ASSERT_UNKNOWN(i > 0 );
					CPPAD_ASSERT_UNKNOWN( i < i_var );
					Y     = Taylor + i * J;
					Z[0]  = Y[0];
				}
				else
				{	i     = VectorInd[ i + arg[0] ];
					i_arg = arg - arg_0;
					Rec->ReplaceInd(i_arg + 2, 0);
					Z[0] = *(CPPAD_GET_PAR(i));
					i    = 0;
				}
			}
			break;
			// -------------------------------------------------

			case LogOp:
			forward_log_op_0(i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case MulvvOp:
			n_res = 1;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			X = Taylor + arg[0] * J;
			Y = Taylor + arg[1] * J;
			Z[0] = X[0] * Y[0];
			break;
			// -------------------------------------------------

			case MulpvOp:
			n_res = 1;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			Y = Taylor + arg[1] * J;
			P = CPPAD_GET_PAR(arg[0]);
			Z[0] = P[0] * Y[0];
			break;
			// -------------------------------------------------

			case MulvpOp:
			n_res = 1;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );

			X = Taylor + arg[0] * J;
			P = CPPAD_GET_PAR(arg[1]);
			Z[0] = X[0] * P[0];
			break;
			// -------------------------------------------------

			case NonOp:
			n_res = 1;
			n_arg = 0;
			break;
			// -------------------------------------------------

			case ParOp:
			n_res = 1;
			n_arg = 1;

			P = CPPAD_GET_PAR(arg[0]);
			Z[0] = *P;
			break;
			// -------------------------------------------------

			case PowvpOp:
			// variables: log(x), y * log(x), pow(x, y)
			n_res = 3;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var);
			U = Z;
			W = U + J;
			Z = W + J;

			// u = log(x)
			X = Taylor + arg[0] * J;
			U[0] = log( X[0] );

			// w = u * y
			Y = CPPAD_GET_PAR(arg[1]);
			W[0] = U[0] * Y[0];

			// z = exp(w)
			// zero order case exactly same as Base type operation
			// d == 0
			Z[0] = pow(X[0], Y[0]);

			break;
			// -------------------------------------------------

			case PowpvOp:
			// variables: log(x), y * log(x), pow(x, y)
			n_res = 3;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var);
			U = Z;
			W = U + J;
			Z = W + J;

			// u = log(x)
			X = CPPAD_GET_PAR(arg[0]);
			U[0] = log(X[0]);

			// w = u * y
			Y = Taylor + arg[1] * J;
			W[0] = U[0] * Y[0];

			// z = exp(w)
			// zero order case exactly same as Base type operation
			Z[0] = pow(X[0], Y[0]);

			break;
			// -------------------------------------------------

			case PowvvOp:
			// variables: log(x), y * log(x), pow(x, y)
			n_res = 3;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var);
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var);
			U = Z;
			W = U + J;
			Z = W + J;

			// u = log(x)
			X = Taylor + arg[0] * J;
			U[0] = log( X[0] );

			// w = u * y
			Y = Taylor + arg[1] * J;
			W[0] = U[0] * Y[0];

			// z = exp(w)
			// zero order case exactly same as Base type operation
			Z[0] = pow(X[0], Y[0]);

			break;
			// -------------------------------------------------

			case PripOp:
			n_res = 0;
			n_arg = 2;
			if( print )
			{	CPPAD_ASSERT_UNKNOWN( arg[0] < Rec->num_rec_text() );
				std::cout << Rec->GetTxt(arg[0]);
				std::cout << *(CPPAD_GET_PAR(arg[1]));
			}
			break;
			// -------------------------------------------------

			case PrivOp:
			n_res = 0;
			n_arg = 2;
			if( print )
			{	CPPAD_ASSERT_UNKNOWN( arg[0] < Rec->num_rec_text() );
				CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

				X      = Taylor + arg[1] * J;
				std::cout << Rec->GetTxt(arg[0]);
				std::cout << X[0];
			}
			break;
			// -------------------------------------------------

			case SinOp:
			// variables: sin(x), cos(x)
			n_arg = 1;
			n_res = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in second variable
			W = Taylor + (i_var+1) * J;
			X = Taylor + arg[0] * J;
			W[0] = cos( X[0] );
			Z[0] = sin( X[0] );
			break;
			// -------------------------------------------------

			case SinhOp:
			// variables: sinh(x), cosh(x)
			n_arg = 1;
			n_res = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in second variable
			W = Taylor + (i_var+1) * J;
			X = Taylor + arg[0] * J;
			W[0] = cosh( X[0] );
			Z[0] = sinh( X[0] );
			break;
			// -------------------------------------------------

			case SqrtOp:
			forward_sqrt_op_0(i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case StppOp:
			n_res = 0;
			n_arg = 3;

			// d == 0
			{	CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( arg[0] < Rec->num_rec_vecad_ind() );

				i   = arg[1];
				CPPAD_ASSERT_UNKNOWN(i < VectorInd[arg[0] - 1]);
				CPPAD_ASSERT_UNKNOWN( 
					i + arg[0] < Rec->num_rec_vecad_ind() 
				);
				VectorInd[ i + arg[0] ] = arg[2];
				VectorVar[ i + arg[0] ] = false;

				Z[0] = *( CPPAD_GET_PAR(arg[2]) );
			}
			break;
			// -------------------------------------------------

			case StpvOp:
			n_res = 0;
			n_arg = 3;

			// d == 0
			{	CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( arg[0] < Rec->num_rec_vecad_ind() );
				CPPAD_ASSERT_UNKNOWN( arg[2] < i_var );

				i   = arg[1];
				CPPAD_ASSERT_UNKNOWN(i < VectorInd[arg[0] - 1]);
				CPPAD_ASSERT_UNKNOWN( 
					i + arg[0] < Rec->num_rec_vecad_ind() 
				);
				VectorInd[ i + arg[0] ] = arg[2];
				VectorVar[ i + arg[0] ] = true;
			}
			break;
			// -------------------------------------------------

			case StvpOp:
			n_res = 0;
			n_arg = 3;

			// d == 0
			{	CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( arg[0] < Rec->num_rec_vecad_ind() );
				CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

				X   = Taylor + arg[1] * J;
				i   = Integer( X[0] );
				len = VectorInd[ arg[0] - 1 ];
				CPPAD_ASSERT_KNOWN( 
					i < len,
					"VecAD index value >= vector length"
				);
				CPPAD_ASSERT_UNKNOWN( 
					i + arg[0] < Rec->num_rec_vecad_ind() 
				);
				VectorInd[ i + arg[0] ] = arg[2];
				VectorVar[ i + arg[0] ] = false;

				Z[0] = *( CPPAD_GET_PAR(arg[2]) );
			}
			break;
			// -------------------------------------------------

			case StvvOp:
			n_res = 0;
			n_arg = 3;

			// d == 0
			{	CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );
				CPPAD_ASSERT_UNKNOWN( arg[0] < Rec->num_rec_vecad_ind() );
				CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );
				CPPAD_ASSERT_UNKNOWN( arg[2] < i_var );

				X   = Taylor + arg[1] * J;
				i   = Integer( X[0] );
				len = VectorInd[ arg[0] - 1 ];
				CPPAD_ASSERT_KNOWN( 
					i < len,
					"VecAD index value >= vector length"
				);
				CPPAD_ASSERT_UNKNOWN( 
					i + arg[0] < Rec->num_rec_vecad_ind() 
				);
				VectorInd[ i + arg[0] ] = arg[2];
				VectorVar[ i + arg[0] ] = true;
			}
			break;
			// -------------------------------------------------

			case SubvvOp:
			n_res = 1;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			X = Taylor + arg[0] * J;
			Y = Taylor + arg[1] * J;
			Z[0] = X[0] - Y[0];
			break;
			// -------------------------------------------------

			case SubpvOp:
			n_res = 1;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			P = CPPAD_GET_PAR(arg[0]);
			Y = Taylor + arg[1] * J;
			Z[0] = P[0] - Y[0];
			break;
			// -------------------------------------------------

			case SubvpOp:
			n_res = 1;
			n_arg = 2;
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );

			X = Taylor + arg[0] * J;
			P = CPPAD_GET_PAR(arg[1]);
			Z[0] = X[0] - P[0];
			break;
			// -------------------------------------------------

			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
		// check not past last index value
		CPPAD_ASSERT_UNKNOWN( arg + n_arg <= arg_0 + Rec->num_rec_op_arg() );

# if CPPAD_FORWARD0SWEEP_TRACE
		size_t       d      = 0;
		size_t       i_tmp  = i_var;
		if( op == PowvpOp || op == PowpvOp || op == PowvvOp )
			i_tmp  += 2;
		printOp(
			std::cout, 
			Rec,
			i_tmp,
			op, 
			arg,
			d + 1, 
			Z, 
			0, 
			(Base *) CPPAD_NULL
		);
	}
	std::cout << std::endl;
# else
	}
# endif
	// check last n_res and n_arg
	CPPAD_ASSERT_UNKNOWN( n_res == NumRes(op) );
	CPPAD_ASSERT_UNKNOWN( n_arg == NumArg(op) );

	CPPAD_ASSERT_UNKNOWN( (i_var + n_res) == Rec->num_rec_var() );
	if( VectorInd != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC(VectorInd);
	if( VectorVar != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC(VectorVar);

	return compareCount;
}

} // END CppAD namespace

# undef CPPAD_FORWARD0SWEEP_TRACE

# endif
