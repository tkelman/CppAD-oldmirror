# ifndef CppADCondExpIncluded
# define CppADCondExpIncluded

// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
// END SHORT COPYRIGHT

/*
-------------------------------------------------------------------------------
$begin CondExp$$
$spell
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
$section The Conditional Expression Function$$

$table
$bold Syntax$$ $cnext 
$syntax%CondExp%Op%(%flag%, %trueCase%, %falseCase%)%$$
$tend

$fend 20$$

$head Description$$
Returns a $italic Type$$ object $italic returnValue$$ that is given by
$syntax%
	if( %boolFlag% )
		%returnValue% = %trueCase%
	else	%returnValue% = %falseCase%
%$$
where $italic boolFlag$$ is given by
$table
$bold Op$$ $cnext $rnext
$code Lt$$ $cnext $syntax%  %boolFlag% = %flag% <  %Type%(0)%$$  $rnext
$code Le$$ $cnext $syntax%  %boolFlag% = %flag% <= %Type%(0)%$$  $rnext
$code Eq$$ $cnext $syntax%  %boolFlag% = %flag% == %Type%(0)%$$  $rnext
$code Ge$$ $cnext $syntax%  %boolFlag% = %flag% >= %Type%(0)%$$  $rnext
$code Gt$$ $cnext $syntax%  %boolFlag% = %flag% >  %Type%(0)%$$  
$tend

$head Type$$
We use $italic Type$$ for the type of
$italic flag$$, $italic trueCase$$, and $italic falseCase$$
(which must all have the same type). 
This type must be
$code float$$, $code double$$
or $syntax%AD<%Base%>%$$ where $italic Base$$ is a valid
type for conditional expressions.

$head Op$$
In the syntax above, $italic Op$$ represents one of the following
two characters: $code Lt$$, $code Le$$, $code Eq$$, $code Ge$$, $code Gt$$. 
As in the table above,
$italic Op$$ determines which comparison operator is used when comparing
$italic flag$$ with zero.

$head CondExp$$
Previous versions of CppAD used $code CondExp$$ ($italic Op$$ empty)
for the same meaning as $code CondExpGt$$.
This use is deprecated, but continues to be supported.
 
$head flag$$
The argument $italic flag$$ has prototype
$syntax%
	const %Type% &%flag%
%$$
It specifies the value that should be compared with zero.

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


$head Motivation$$
The choice in a conditional expression is made each time
$xref/Forward//f.Forward/$$ is used to evaluate the zero order Taylor
coefficients.
This enables one to extend the range of independent variable values
for which an $xref/ADFun/$$ object is a valid representation
of the corresponding algorithm.
(See $xref/Discrete/$$ for another type of taped evaluation). 

$head Example$$
$children%
	Example/CondExp.cpp
%$$
The file
$xref/CondExp.cpp/$$
contains an example and a test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

// ------------ CondExpOp(cop, flag, trueCase, falseCase) --------------

inline float CondExpOp( 
	enum CompareOp     cop ,
	const float      &flag , 
	const float  &trueCase , 
	const float &falseCase )
{	float returnValue;
	switch( cop )
	{
		case Lt:
		if( flag < 0. )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case Le:
		if( flag <= 0. )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case Eq:
		if( flag == 0. )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case Ge:
		if( flag >= 0. )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case Gt:
		if( flag > 0. )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		default:
		CppADUnknownError(0);
		returnValue = trueCase;
	}
	return returnValue;
}

inline double CondExpOp( 
	enum CompareOp     cop  ,
	const double     &flag  , 
	const double &trueCase  , 
	const double &falseCase )
{	double returnValue;
	switch( cop )
	{
		case Lt:
		if( flag < 0. )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case Le:
		if( flag <= 0. )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case Eq:
		if( flag == 0. )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case Ge:
		if( flag >= 0. )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		case Gt:
		if( flag > 0. )
			returnValue = trueCase;
		else	returnValue = falseCase;
		break;

		default:
		CppADUnknownError(0);
		returnValue = trueCase;
	}
	return returnValue;
}


inline std::complex<float> CondExpOp(
	enum CompareOp             cop       ,
	const std::complex<float> &flag      ,
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
	const std::complex<double> &flag      ,
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
	const AD<Base> &flag      , 
	const AD<Base> &trueCase  , 
	const AD<Base> &falseCase )
{
	AD<Base> returnValue;
	CppADUnknownError( returnValue.id == 0 );

	// check first case where do not need to tape
	if( IdenticalPar(flag) )
	{	switch( cop )
		{
			case Lt:
			if( flag.value < Base(0) )
				returnValue = trueCase;
			else	returnValue = falseCase;
			break;

			case Le:
			if( flag.value <= Base(0) )
				returnValue = trueCase;
			else	returnValue = falseCase;
			break;

			case Eq:
			if( flag.value == Base(0) )
				returnValue = trueCase;
			else	returnValue = falseCase;
			break;

			case Ge:
			if( flag.value >= Base(0) )
				returnValue = trueCase;
			else	returnValue = falseCase;
			break;

			case Gt:
			if( flag.value > Base(0) )
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
	returnValue.value = 
		CondExpOp(cop, flag.value, trueCase.value, falseCase.value);

	// second case where do not need to tape this operation
	if( AD<Base>::Tape()->State() == Empty ) 
		return returnValue;

	// third case where we do not need to tape this operation
	if( Parameter(flag) & Parameter(trueCase) & Parameter(falseCase) )
		return returnValue;
	
	// add this operation to the tape
	AD<Base>::Tape()->
		RecordCondExp(cop, returnValue, flag, trueCase, falseCase);

	return returnValue;
}

// ----- RecordCondExp(cop, returnValue, flag, trueCase, falseCase) ------

template <class Base>
void ADTape<Base>::RecordCondExp(
	enum CompareOp  cop         ,
	AD<Base>       &returnValue ,
	const AD<Base> &flag        ,
	const AD<Base> &trueCase    ,
	const AD<Base> &falseCase   )
{	size_t   op_taddr;
	size_t   ind0, ind1, ind2, ind3;
	size_t   returnValue_taddr;
	OpCode   op;

	static OpCode List[] = {
		CEpppOp,
		CEppvOp,
		CEpvpOp,
		CEpvvOp,
		CEvppOp,
		CEvpvOp,
		CEvvpOp,
		CEvvvOp
	};
	op_taddr = 0;
	if( Variable(flag) )
		op_taddr += 4;
	if( Variable(trueCase) )
		op_taddr += 2;
	if( Variable(falseCase) )
		op_taddr += 1;

	CppADUnknownError( 0 < op_taddr && op_taddr < 8 ); 
	op       = List[op_taddr];

	// taddr of this variable
	returnValue_taddr = Rec.PutOp(op);

	// Make sure returnValue is in the list of variables and set its taddr
	if( Parameter(returnValue) )
		returnValue.MakeVariable( returnValue_taddr );
	else	returnValue.taddr = returnValue_taddr;

	// ind for flag
	if( Parameter(flag) )
		ind0 = Rec.PutPar(flag.value);
	else	ind0 = flag.taddr;	

	// ind for trueCase
	if( Parameter(trueCase) )
		ind1 = Rec.PutPar(trueCase.value);
	else	ind1 = trueCase.taddr;	

	// ind for falseCase
	if( Parameter(falseCase) )
		ind2 = Rec.PutPar(falseCase.value);
	else	ind2 = falseCase.taddr;	

	// ind for comparison operator
	ind3 = (size_t) cop;

	CppADUnknownError( NumInd(op) == 4 );
	Rec.PutInd(ind0, ind1, ind2, ind3);

	// check that returnValue is a dependent variable
	CppADUnknownError( Variable(returnValue) );
}

// ------------- CondExp(flag, trueCase, falseCase) -------------------

# define CppADCondExp(Name)                                             \
	template <class Base>                                           \
	inline AD<Base> CondExp##Name(                                  \
		const AD<Base> &flag      ,                             \
		const AD<Base> &trueCase  ,                             \
		const AD<Base> &falseCase )                             \
	{                                                               \
		return CondExpOp(Name, flag, trueCase, falseCase);      \
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
	return CondExpOp(Gt, flag, trueCase, falseCase);
}

# undef CppADCondExp
# define CppADCondExp(Name, Op, Type)                               \
	inline Type CondExp##Name(                                  \
		const Type &flag      ,                             \
		const Type &trueCase  ,                             \
		const Type &falseCase )                             \
	{	Type returnValue;                                   \
		if( flag Op 0. )                                    \
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
	return CondExpOp(Gt, flag, trueCase, falseCase);
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
	return CondExpOp(Gt, flag, trueCase, falseCase);
}

# undef CppADCondExp

} // END CppAD namespace

# endif 
