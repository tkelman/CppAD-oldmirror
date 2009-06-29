/* $Id$ */
# ifndef CPPAD_FORWARD_SWEEP_INCLUDED
# define CPPAD_FORWARD_SWEEP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin forward_sweep$$ $comment CppAD Developer Documentation$$
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

$section Forward Computation of Taylor Coefficients$$
$index forward_sweep$$
$index mode, forward$$
$index forward, mode$$
$index derivative, forward$$
$index Taylor coefficient, forward$$


$head Syntax$$
$syntax%size_t forward_sweep(
	bool %print%,
	size_t %d%,
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


$head d$$
Given the $th d-1$$ order Taylor coefficients matrix for all the variables,
and the $th d$$ order Taylor coefficients for all the independent variables,
$code forward_sweep$$ computes the $th d$$ order Taylor coefficients 
for all the other variables.


$head numvar$$
is the number of rows in the matrix $italic Taylor$$.
It must also be equal to $syntax%%Rec%->num_rec_var()%$$.


$head J$$
Is the number of columns in the coefficient matrix $italic Taylor$$.
This must be greater than or equal $latex d+1$$.


$head On Input$$

$subhead Independent Variables and Operators$$
The independent variable records come first.
For $latex i = 1, \ldots , n$$ and $latex j = 0 , \ldots , d$$,
$table
	$bold field$$ $cnext 
	$bold Value$$          
$rnext
	$syntax%%Taylor%[%0% * %J% + %j%]%$$      $cnext 
	the variable with index zero is not used
$rnext
	$syntax%%Rec%->GetOp(0)%$$                $cnext 
	the operator with index zero must be a $code NonOp$$
$rnext
	$syntax%%Taylor%[%i% * %J% + %j%]%$$      $cnext 
	$th j$$ order coefficient for variable with index $italic i$$   
$rnext
	$syntax%%Rec%->GetOp(%i%)%$$              $cnext 
	the operator with index $italic i$$ must be a $code InvOp$$
$tend

$subhead Other Variables and Operators$$
The other variables follow the independent variables.
For $latex i = n+1, \ldots , numvar-1$$,
$latex j = 0 , \ldots , d-1$$,
and $latex k = n+1, \ldots ,$$ $syntax%%Rec%->num_rec_op() - 1%$$,
$table
	$bold field$$ $cnext 
	$bold Value$$          
$rnext
	$syntax%%Taylor%[%i% * %J% + %j%]%$$      $cnext 
	$th j$$ coefficient for variable with index $italic i$$     
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
For $latex i = n+1, \ldots , numvar-1$$ and $latex j < d$$,
$syntax%%Taylor%[%i% * %J% + %j%]%$$ is not modified.
For $latex i = n+1, \ldots , numvar-1$$, 
$syntax%%Taylor%[%i% * %J% + %d%]%$$ is set equal to the
$th d$$ order Taylor coefficient for the variable with index $italic i$$.

$contents%
        cppad/local/forward0sweep.hpp
%$$

$end
------------------------------------------------------------------------------
*/
# include <cppad/local/forward0sweep.hpp>

# define CPPAD_FORWARD_SWEEP_TRACE 0

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
size_t forward_sweep(
	bool                  print,
	size_t                d,
	size_t                numvar,
	player<Base>         *Rec,
	size_t                J,
	Base                 *Taylor
)
{
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
	const Base  zero = Base(0);

	// used by Com operator
	bool result;

	Base             *Z = 0;
	Base             *W = 0;
	Base             *U = 0;

	size_t            i;
	size_t          len;
	size_t        n_res = 0;
	size_t        n_arg = 0;


	// initialize the comparision operator (ComOp) counter
	size_t compareCount = 0;

	// if this is an order zero calculation, initialize vector indices
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


	// check numvar argument
	CPPAD_ASSERT_UNKNOWN( Rec->num_rec_var() == numvar );

	// set the number of operators
	numop_m1 = Rec->num_rec_op() - 1;

	// skip the NonOp at the beginning of the recording
	Rec->start_forward(op, arg, i_op, i_var);
	arg_0 = arg;
# if CPPAD_FORWARD_SWEEP_TRACE
	std::cout << std::endl;
# endif
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
			forward_abs_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case AddvvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			X = Taylor + arg[0] * J;
			Y = Taylor + arg[1] * J;
			ForAddvvOp(d, Z, X, Y);
			break;
			// -------------------------------------------------

			case AddpvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			P = Rec->GetPar( arg[0] );
			Y = Taylor + arg[1] * J;
			ForAddpvOp(d, Z, P, Y);
			break;
			// -------------------------------------------------

			case AddvpOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );

			X = Taylor + arg[0] * J;
			P = Rec->GetPar( arg[1] );
			ForAddvpOp(d, Z, X, P);
			break;
			// -------------------------------------------------

			case AcosOp:
			// variables: acos(x),  sqrt(1 - x * x) 
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_acos_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case AsinOp:
			// results: asin(x),  sqrt(1 - x * x) 
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_asin_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case AtanOp:
			// results: atan(x),  1 + x * x 
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );
			forward_atan_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case CExpOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 6);
			CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );
			if( arg[1] & 1 )
				left = Taylor + arg[2] * J;
			else	left = Rec->GetPar(arg[2]);
			if( arg[1] & 2 )
				right = Taylor + arg[3] * J;
			else	right = Rec->GetPar(arg[3]);
			if( d == 0 )
			{	if( arg[1] & 4 )
					trueCase = Taylor + arg[4] * J;
				else	trueCase = Rec->GetPar(arg[4]);
				if( arg[1] & 8 )
					falseCase = Taylor + arg[5] * J;
				else	falseCase = Rec->GetPar(arg[5]);
			}
			else
			{	if( arg[1] & 4 )
					trueCase = Taylor + arg[4] * J + d;
				else	trueCase = &zero;
				if( arg[1] & 8 )
					falseCase = Taylor + arg[5] * J + d;
				else	falseCase = &zero;
			}
			Z[d] = CondExpOp(
				CompareOp( arg[0] ),
				*left,
				*right,
				*trueCase,
				*falseCase
			);
			break;
			// ---------------------------------------------------

			case ComOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 0);
			CPPAD_ASSERT_UNKNOWN( n_arg == 4);
			CPPAD_ASSERT_UNKNOWN( arg[1] > 1 );
			if( d == 0 )
			{	if( arg[1] & 1 )
					result = true;
				else	result = false;
				if( arg[1] & 2 )
					left = Taylor + arg[2] * J;
				else	left = Rec->GetPar(arg[2]);
				if( arg[1] & 4 )
					right = Taylor + arg[3] * J;
				else	right = Rec->GetPar(arg[3]);
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
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( n_res == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in variable record
			W = Taylor + (i_var+1) * J;
			X   = Taylor + arg[0] * J;
			ForTrigSinCos(d, W, Z, X);
			break;
			// ---------------------------------------------------

			case CoshOp:
			// variables: cosh(x), sinh(x)
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( n_res == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in variable record
			W = Taylor + (i_var+1) * J;
			X   = Taylor + arg[0] * J;
			ForHypSinCos(d, W, Z, X);
			break;
			// -------------------------------------------------

			case DisOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			if( d == 0 ) 
			{	X   = Taylor + arg[0] * J;
				Z[0] = ADDiscrete<Base>::Eval(arg[1], X[0]);
			}
			else	Z[d] = Base(0);
			break;
			// -------------------------------------------------

			case DivvvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			X = Taylor + arg[0] * J;
			Y = Taylor + arg[1] * J;
			ForDivvvOp(d, Z, X, Y);
			break;
			// -------------------------------------------------

			case DivpvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			Y = Taylor + arg[1] * J;
			P = Rec->GetPar( arg[0] );
			ForDivpvOp(d, Z, P, Y);
			break;
			// -------------------------------------------------

			case DivvpOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );

			P = Rec->GetPar( arg[1] );
			X = Taylor + arg[0] * J;
			ForDivvpOp(d, Z, X, P);
			break;
			// -------------------------------------------------

			case ExpOp:
			forward_exp_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case InvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 0 );
			break;
			// -------------------------------------------------

			case LdpOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 3 );
			
			CPPAD_ASSERT_UNKNOWN( arg[0] > 0 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < Rec->num_rec_vecad_ind() );
			CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
			CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );

			if( d == 0 )
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
					Z[d]  = Y[d];
				}
				else
				{	i     = VectorInd[ i + arg[0] ];
					i_arg = arg - arg_0;
					Rec->ReplaceInd(i_arg + 2, 0);
					Z[d] = *(Rec->GetPar(i));
					i    = 0;
				}
			}
			else
			{	i = arg[2];
				if( i > 0 )
				{	CPPAD_ASSERT_UNKNOWN( i < i_var );
					Y     = Taylor + i * J;
					Z[d]  = Y[d];
				}
				else	Z[d]  = Base(0);
			}
			break;
			// -------------------------------------------------

			case LdvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 3 );
			
			CPPAD_ASSERT_UNKNOWN( arg[0] > 0 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < Rec->num_rec_vecad_ind() );
			CPPAD_ASSERT_UNKNOWN( VectorInd != CPPAD_NULL );
			CPPAD_ASSERT_UNKNOWN( VectorVar != CPPAD_NULL );

			if( d == 0 )
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
					Z[d]  = Y[d];
				}
				else
				{	i     = VectorInd[ i + arg[0] ];
					i_arg = arg - arg_0;
					Rec->ReplaceInd(i_arg + 2, 0);
					Z[d] = *(Rec->GetPar(i));
					i    = 0;
				}
			}
			else
			{	i = arg[2];
				if( i > 0 )
				{	CPPAD_ASSERT_UNKNOWN( i < i_var );
					Y     = Taylor + i * J;
					Z[d]  = Y[d];
				}
				else	Z[d]  = Base(0);
			}
			break;
			// -------------------------------------------------

			case LogOp:
			forward_log_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case MulvvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			X = Taylor + arg[0] * J;
			Y = Taylor + arg[1] * J;
			ForMulvvOp(d, Z, X, Y);
			break;
			// -------------------------------------------------

			case MulpvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			Y = Taylor + arg[1] * J;
			P = Rec->GetPar( arg[0] );
			ForMulpvOp(d, Z, P, Y);
			break;
			// -------------------------------------------------

			case MulvpOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );

			X = Taylor + arg[0] * J;
			P = Rec->GetPar( arg[1] );
			ForMulvpOp(d, Z, X, P);
			break;
			// -------------------------------------------------

			case NonOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 0 );
			break;
			// -------------------------------------------------

			case ParOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );

			P = Rec->GetPar( arg[0] );
			if( d == 0 )
				Z[d] = *P;
			else	Z[d] = Base(0); 
			break;
			// -------------------------------------------------

			case PowvpOp:
			// variables: log(x), y * log(x), pow(x, y)
			CPPAD_ASSERT_UNKNOWN( n_res == 3);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var);
			U = Z;
			W = U + J;
			Z = W + J;

			// u = log(x)
			X = Taylor + arg[0] * J;
			forward_log_op(d, i_var, arg[0], J, Taylor);

			// w = u * y
			Y = Rec->GetPar( arg[1] );
			ForMulvpOp(d, W, U, Y);

			// z = exp(w)
			// zero order case exactly same as Base type operation
			if( d == 0 )
				Z[0] = pow(X[0], Y[0]);
			else	forward_exp_op(d, i_var+2, i_var+1, J, Taylor);

			break;
			// -------------------------------------------------

			case PowpvOp:
			// variables: log(x), y * log(x), pow(x, y)
			CPPAD_ASSERT_UNKNOWN( n_res == 3);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var);
			U = Z;
			W = U + J;
			Z = W + J;

			// u = log(x)
			X = Rec->GetPar(arg[0]);
			if( d == 0 )
				U[d] = log(X[d]);
			else	U[d] = Base(0);

			// w = u * y
			Y   = Taylor + arg[1] * J;
			ForMulpvOp(d, W, U, Y);

			// z = exp(w)
			// zero order case exactly same as Base type operation
			if( d == 0 )
				Z[0] = pow(X[0], Y[0]);
			else	forward_exp_op(d, i_var+2, i_var+1, J, Taylor);

			break;
			// -------------------------------------------------

			case PowvvOp:
			// variables: log(x), y * log(x), pow(x, y)
			CPPAD_ASSERT_UNKNOWN( n_res == 3);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var);
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var);
			U = Z;
			W = U + J;
			Z = W + J;

			// u = log(x)
			X = Taylor + arg[0] * J;
			forward_log_op(d, i_var, arg[0], J, Taylor);

			// w = u * y
			Y   = Taylor + arg[1] * J;
			ForMulvvOp(d, W, U, Y);

			// z = exp(w)
			// zero order case exactly same as Base type operation
			if( d == 0 )
				Z[0] = pow(X[0], Y[0]);
			else	forward_exp_op(d, i_var+2, i_var+1, J, Taylor);

			break;
			// -------------------------------------------------

			case PripOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 0 );
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			if( print & (d == 0) )
			{	CPPAD_ASSERT_UNKNOWN( arg[0] < Rec->num_rec_text() );
				std::cout << Rec->GetTxt(arg[0]);
				std::cout << *(Rec->GetPar(arg[1]));
			}
			break;
			// -------------------------------------------------

			case PrivOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 0);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			if( print & (d == 0) )
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
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( n_res == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in second variable
			W = Taylor + (i_var+1) * J;
			X   = Taylor + arg[0] * J;
			ForTrigSinCos(d, Z, W, X);
			break;
			// -------------------------------------------------

			case SinhOp:
			// variables: sinh(x), cosh(x)
			CPPAD_ASSERT_UNKNOWN( n_arg == 1 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( n_res == 2);
			CPPAD_ASSERT_UNKNOWN( (i_var+1) < numvar  );

			// use W for data stored in second variable
			W = Taylor + (i_var+1) * J;
			X   = Taylor + arg[0] * J;
			ForHypSinCos(d, Z, W, X);
			break;
			// -------------------------------------------------

			case SqrtOp:
			forward_sqrt_op(d, i_var, arg[0], J, Taylor);
			break;
			// -------------------------------------------------

			case StppOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 0);
			CPPAD_ASSERT_UNKNOWN( n_arg == 3 );

			if( d == 0 )
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

				Z[d] = *( Rec->GetPar( arg[2] ) );
			}
			break;
			// -------------------------------------------------

			case StpvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 0);
			CPPAD_ASSERT_UNKNOWN( n_arg == 3 );

			if( d == 0 )
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
			CPPAD_ASSERT_UNKNOWN( n_res == 0);
			CPPAD_ASSERT_UNKNOWN( n_arg == 3 );

			if( d == 0 )
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

				Z[d] = *( Rec->GetPar( arg[2] ) );
			}
			break;
			// -------------------------------------------------

			case StvvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 0);
			CPPAD_ASSERT_UNKNOWN( n_arg == 3 );

			if( d == 0 )
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
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			X = Taylor + arg[0] * J;
			Y = Taylor + arg[1] * J;
			ForSubvvOp(d, Z, X, Y);
			break;
			// -------------------------------------------------

			case SubpvOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[1] < i_var );

			P = Rec->GetPar( arg[0] );
			Y = Taylor + arg[1] * J;
			ForSubpvOp(d, Z, P, Y);
			break;
			// -------------------------------------------------

			case SubvpOp:
			CPPAD_ASSERT_UNKNOWN( n_res == 1);
			CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
			CPPAD_ASSERT_UNKNOWN( arg[0] < i_var );

			X = Taylor + arg[0] * J;
			P = Rec->GetPar( arg[1] );
			ForSubvpOp(d, Z, X, P);
			break;
			// -------------------------------------------------

			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
# if CPPAD_FORWARD_SWEEP_TRACE
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
	CPPAD_ASSERT_UNKNOWN( (i_var + n_res) == Rec->num_rec_var() );
	if( VectorInd != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC(VectorInd);
	if( VectorVar != CPPAD_NULL )
		CPPAD_TRACK_DEL_VEC(VectorVar);

	return compareCount;
}

} // END CppAD namespace

# undef CPPAD_FORWARD_SWEEP_TRACE

# endif
