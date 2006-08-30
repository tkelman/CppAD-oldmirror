# ifndef CppADCondExpIncluded
# define CppADCondExpIncluded

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */

/*
-------------------------------------------------------------------------------
$begin CondExp$$
$spell
	Atan2
	CondExp
	Taylor
	std
	Cpp
	namespace
	inline
	const
	abs
	Op
	bool
	Lt
	Le
	Eq
	Ge
	Gt
$$

$index conditional, expression$$
$index expression, conditional$$
$index assign, conditional$$

$section AD Conditional Expressions$$

$head Syntax$$
$syntax%%result% = CondExp%Op%(%left%, %right%, %trueCase%, %falseCase%)%$$


$head Purpose$$
Record, 
as part of an AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$,
a the conditional result of the form 
$syntax%
	if( %left% %op% %right% )
		%result% = %trueCase%
	else	%result% = %falseCase%
%$$
The notation $italic Op$$ and $italic op$$ 
above have the following correspondence: 
$table
$italic Op$$ 
	$pre  $$ $cnext $code Lt$$
	$pre  $$ $cnext $code Le$$
	$pre  $$ $cnext $code Eq$$
	$pre  $$ $cnext $code Ge$$
	$pre  $$ $cnext $code Gt$$
$rnext
$italic op$$ 
	$cnext $code <$$
	$cnext $code <=$$
	$cnext $code ==$$
	$cnext $code >=$$
	$cnext $code >$$
$tend
If $italic f$$ is the $xref/ADFun/$$ object corresponding to the
AD operation sequence,
the choice in an AD conditional expression is made each time
$xref/Forward//f.Forward/$$ is used to evaluate the zero order Taylor
coefficients with new values for the 
$xref/glossary/Independent Variable/independent variables/$$.
This is in contrast to the $xref/Compare//AD comparison operators/$$
which are boolean valued and not included in the AD operation sequence. 

$head Type$$
We use $italic Type$$ for the type of
$italic left$$, $italic right$$, $italic trueCase$$, and $italic falseCase$$
(which must all have the same type). 
This type must be
$code float$$, $code double$$, or in the 
$xref/glossary/AD Type Sequence/AD type sequences/$$
above $code float$$ or $code double$$.

$head Op$$
In the syntax above, $italic Op$$ represents one of the following
two characters: $code Lt$$, $code Le$$, $code Eq$$, $code Ge$$, $code Gt$$. 
As in the table above,
$italic Op$$ determines the comparison operator $italic op$$.
 
$head left$$
The argument $italic left$$ has prototype
$syntax%
	const %Type% &%left%
%$$
It specifies the value for the left side of the comparison operator.
 
$head right$$
The argument $italic right$$ has prototype
$syntax%
	const %Type% &%right%
%$$
It specifies the value for the right side of the comparison operator.

$head trueCase$$
The argument $italic trueCase$$ has prototype
$syntax%
	const %Type% &%trueCase%
%$$
It specifies the return value if the result of the comparison is true.

$head falseCase$$
The argument $italic falseCase$$ has prototype
$syntax%
	const %Type% &%falseCase%
%$$
It specifies the return value if the result of the comparison is false.

$head result$$
The $italic result$$ has prototype
$syntax%
	%Type% &%falseCase%
%$$


$head CondExp$$
Previous versions of CppAD used 
$syntax%
	CondExp(%flag%, %trueCase%, %falseCase%)
%$$
for the same meaning as 
$syntax%
	CondExpGt(%flag%, %Type%(0), %trueCase%, %falseCase%)
%$$
Use of $code CondExp$$ is deprecated, but continues to be supported.

$head Operation Sequence$$
This is an AD of $italic Base$$
$xref/glossary/Operation/Atomic/atomic operation/1/$$
and hence is part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.


$head Example$$

$head Test$$
$children%
	Example/CondExp.cpp
%$$
The file
$xref/CondExp.cpp/$$
contains an example and test of this function.   
It returns true if it succeeds and false otherwise.

$head Atan2$$
The following implementation of the
AD $xref/atan2/$$ function is a more complex
example of using conditional expressions:
$code
$verbatim%CppAD/local/atan2_.hpp%0%BEGIN CondExp%// END CondExp%$$
$$


$end
-------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

// ------------ CondExpOp(cop, left, right, trueCase, falseCase) --------------

template <class CompareType, class ResultType>
inline ResultType CondExpTemplate( 
	enum  CompareOp            cop ,
	const CompareType        &left ,
	const CompareType       &right , 
	const ResultType     &trueCase , 
	const ResultType    &falseCase )
{	ResultType returnValue;
	switch( cop )
	{
		case CompareLt:
		if( left < right )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case CompareLe:
		if( left <= right )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case CompareEq:
		if( left == right )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case CompareGe:
		if( left >= right )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case CompareGt:
		if( left > right )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		default:
		CppADUnknownError(0);
		returnValue = trueCase;
	}
	return returnValue;
}

inline float CondExpOp( 
	enum CompareOp     cop ,
	const float      &left ,
	const float     &right , 
	const float  &trueCase , 
	const float &falseCase )
{	return CondExpTemplate(cop, left, right, trueCase, falseCase);
}

inline double CondExpOp( 
	enum CompareOp     cop ,
	const double      &left ,
	const double     &right , 
	const double  &trueCase , 
	const double &falseCase )
{	return CondExpTemplate(cop, left, right, trueCase, falseCase);
}

inline std::complex<float> CondExpOp(
	enum CompareOp             cop       ,
	const std::complex<float> &left      ,
	const std::complex<float> &right     ,
	const std::complex<float> &trueCase  ,
	const std::complex<float> &falseCase )
{	CppADUsageError(
		0,
		"Error: cannot use CondExp with a complex type"
	);
	return std::complex<float>(0);
}


inline std::complex<double> CondExpOp(
	enum CompareOp             cop        ,
	const std::complex<double> &left      ,
	const std::complex<double> &right     ,
	const std::complex<double> &trueCase  ,
	const std::complex<double> &falseCase )
{	CppADUsageError(
		0,
		"Error: cannot use CondExp with a complex type"
	);
	return std::complex<double>(0);
}

template <class Base>
inline AD<Base> CondExpOp(
	enum  CompareOp cop       ,
	const AD<Base> &left      , 
	const AD<Base> &right     , 
	const AD<Base> &trueCase  , 
	const AD<Base> &falseCase )
{
	AD<Base> returnValue;
	CppADUnknownError( returnValue.id == 0 );

	// check first case where do not need to tape
	if( IdenticalPar(left) & IdenticalPar(right) )
	{	switch( cop )
		{
			case CompareLt:
			if( left.value < right.value )
				returnValue = trueCase;
			else	returnValue = falseCase;
			break;

			case CompareLe:
			if( left.value <= right.value )
				returnValue = trueCase;
			else	returnValue = falseCase;
			break;

			case CompareEq:
			if( left.value == right.value )
				returnValue = trueCase;
			else	returnValue = falseCase;
			break;

			case CompareGe:
			if( left.value >= right.value )
				returnValue = trueCase;
			else	returnValue = falseCase;
			break;

			case CompareGt:
			if( left.value > right.value )
				returnValue = trueCase;
			else	returnValue = falseCase;
			break;

			default:
			CppADUnknownError(0);
			returnValue = trueCase;
		}
		return returnValue;
	}

	// must use CondExp incase Base is an AD type and recording
	returnValue.value = CondExpOp(cop, 
		left.value, right.value, trueCase.value, falseCase.value);

	// second case where do not need to tape this operation
	if( AD<Base>::Tape()->State() == Empty ) 
		return returnValue;

	// third case where we do not need to tape this operation
	if(	Parameter(left)      & 
		Parameter(right)     & 
		Parameter(trueCase)  & 
		Parameter(falseCase) 
	)	return returnValue;
	
	// add this operation to the tape
	AD<Base>::Tape()-> RecordCondExp(cop, 
		returnValue, left, right, trueCase, falseCase);

	return returnValue;
}

// --- RecordCondExp(cop, returnValue, left, right, trueCase, falseCase) -----

template <class Base>
void ADTape<Base>::RecordCondExp(
	enum CompareOp  cop         ,
	AD<Base>       &returnValue ,
	const AD<Base> &left        ,
	const AD<Base> &right       ,
	const AD<Base> &trueCase    ,
	const AD<Base> &falseCase   )
{	size_t   ind0, ind1, ind2, ind3, ind4, ind5;
	size_t   returnValue_taddr;

	// taddr of this variable
	returnValue_taddr = Rec.PutOp(CExpOp);

	// ind[0] = cop
	ind0 = size_t( cop );

	// ind[1] = base 2 representaion of the value
	// [Var(left), Var(right), Var(trueCase), Var(falseCase)]
	ind1 = 0;

	// Make sure returnValue is in the list of variables and set its taddr
	if( Parameter(returnValue) )
		returnValue.MakeVariable( returnValue_taddr );
	else	returnValue.taddr = returnValue_taddr;

	// ind[2] = left address
	if( Parameter(left) )
		ind2 = Rec.PutPar(left.value);
	else
	{	ind1 += 1;
		ind2 = left.taddr;	
	}

	// ind[3] = right address
	if( Parameter(right) )
		ind3 = Rec.PutPar(right.value);
	else
	{	ind1 += 2;
		ind3 = right.taddr;	
	}

	// ind[4] = trueCase address
	if( Parameter(trueCase) )
		ind4 = Rec.PutPar(trueCase.value);
	else
	{	ind1 += 4;
		ind4 = trueCase.taddr;	
	}

	// ind[5] =  falseCase address
	if( Parameter(falseCase) )
		ind5 = Rec.PutPar(falseCase.value);
	else
	{	ind1 += 8;
		ind5 = falseCase.taddr;	
	}

	CppADUnknownError( NumInd(CExpOp) == 6 );
	CppADUnknownError( ind1 > 0 );
	Rec.PutInd(ind0, ind1, ind2, ind3, ind4, ind5);

	// check that returnValue is a dependent variable
	CppADUnknownError( Variable(returnValue) );
}

// ------------ CondExpOp(left, right, trueCase, falseCase) ----------------

# define CppADCondExp(Name)                                                \
	template <class Base>                                              \
	inline AD<Base> CondExp##Name(                                     \
		const AD<Base> &left      ,                                \
		const AD<Base> &right     ,                                \
		const AD<Base> &trueCase  ,                                \
		const AD<Base> &falseCase )                                \
	{                                                                  \
		return CondExpOp(Compare##Name,                            \
			left, right, trueCase, falseCase);                 \
	}

// AD<Base>
CppADCondExp(Lt)
CppADCondExp(Le)
CppADCondExp(Eq)
CppADCondExp(Ge)
CppADCondExp(Gt)
template <class Base>
inline AD<Base> CondExp(
	const AD<Base> &flag      , 
	const AD<Base> &trueCase  ,
	const AD<Base> &falseCase )
{	
	return CondExpOp(CompareGt, flag, AD<Base>(0), trueCase, falseCase);
}

# undef CppADCondExp
# define CppADCondExp(Name, Op, Type)                               \
	inline Type CondExp##Name(                                  \
		const Type &left      ,                             \
		const Type &right     ,                             \
		const Type &trueCase  ,                             \
		const Type &falseCase )                             \
	{	Type returnValue;                                   \
		if( left Op right )                                 \
			returnValue = trueCase;                     \
		else	returnValue = falseCase;                    \
		return returnValue;                                 \
	}

// float
CppADCondExp(Lt,  <, float)
CppADCondExp(Le, <=, float)
CppADCondExp(Eq, ==, float)
CppADCondExp(Ge, >=, float)
CppADCondExp(Gt,  >, float)
inline float CondExp(
	const float &flag      , 
	const float &trueCase  ,
	const float &falseCase )
{	
	return CondExpGt(flag, float(0), trueCase, falseCase);
}

// double
CppADCondExp(Lt,  <, double)
CppADCondExp(Le, <=, double)
CppADCondExp(Eq, ==, double)
CppADCondExp(Ge, >=, double)
CppADCondExp(Gt,  >, double)
inline double CondExp(
	const double &flag      , 
	const double &trueCase  ,
	const double &falseCase )
{	
	return CondExpGt(flag, 0., trueCase, falseCase);
}

# undef CppADCondExp

} // END CppAD namespace

# endif 
