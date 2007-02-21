# ifndef CPPAD_INDEPENDENT_INCLUDED
# define CPPAD_INDEPENDENT_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
---------------------------------------------------------------------------

$begin Independent$$
$spell
	Cpp
	bool
	const
	var
	typename
$$

$index Independent$$
$index start, recording$$
$index recording, start$$
$index variable, independent$$

$section Declare Independent Variables and Start Recording$$

$head Syntax$$
$syntax%Independent(%x%)%$$


$head Purpose$$
Start a recording the 
$xref/glossary/AD of Base/AD of Base/$$ operations
with $italic x$$ as the vector of independent variables.
Once the 
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$ is completed,
it must be transferred to a function object; see below.

$head Variables for a Tape$$
A tape is create by the call 
$syntax%
	Independent(%x%)
%$$
The corresponding operation sequence is transferred to a function object,
and the tape is destroyed,
using either (see $cref/ADFun<Base> f(x, y)/FunConstruct/$$)
$syntax%
	ADFun<%Base%> %f%( %x%, %y%)
%$$
or using (see $cref/f.Dependent(x, y)/Dependent/$$)
$syntax%
	%f%.Dependent( %x%, %y%)
%$$
Between when the tape is created and when it is destroyed,
we refer to the elements of $italic x$$, 
and the values that depend on the elements of $italic x$$,
as variables for the tape created by the call to $code Independent$$. 

$head x$$
The vector $italic x$$ has prototype
$syntax%
	%VectorAD% &%x%
%$$
(see $italic VectorAD$$ below).
The size of the vector $italic x$$, must be greater than zero,
and is the number of independent variables for this
AD operation sequence.


$head VectorAD$$
The type $italic VectorAD$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type/$$
$syntax%AD<%Base%>%$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head Example$$
$children%
	example/independent.cpp
%$$
The file
$xref/Independent.cpp/$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {
// ---------------------------------------------------------------------------

template <typename Base>
template <typename VectorAD>
void ADTape<Base>::Independent(VectorAD &x)
{
	// check VectorAD is Simple Vector class with AD<Base> elements
	CheckSimpleVector< AD<Base>, VectorAD>();

	// dimension of the domain space
	size_t n = x.size();
	CppADUsageError(
		n > 0,
		"Indepdendent: the argument vector x has zero size"
	);
	CppADUnknownError( Rec.NumOp() == 0 );
	CppADUnknownError( Rec.TotNumVar() == 0 );

	// skip the first record (parameters use taddr zero)
	Rec.PutOp(NonOp);

	// no Ind values for this operator
	CppADUnknownError( NumInd(NonOp) == 0 );

	// place each of the independent variables in the tape
	size_t j;
	for(j = 0; j < n; j++)
	{	RecordInvOp(x[j]);
		CppADUnknownError( x[j].taddr_ == j+1 );
	}

	// done specifying all of the independent variables
	size_independent = n;
}

template <typename VectorAD>
inline void Independent(VectorAD &x)
{	typedef typename VectorAD::value_type ADBase;
	typedef typename ADBase::value_type   Base;
# ifndef NDEBUG
	size_t j;
	for(j = 0; j < x.size(); j++) CppADUsageError(
		Parameter(x[j]),
		"Independent: one of the argument components is a variable "
		"\ncorresponding to a previous call to Independent"
	);
# endif
	CppADUsageError(
		ADBase::tape_active_count(0) < ADBase::tape_max_active(0),
		"Independent: cannot create another tape because the"
		"\nmaximum allowable number of tapes are currently active."
	);
	size_t id = ADBase::tape_new_id();

	ADBase::tape_ptr(id)->Independent(x); 
}


} 
// END CppAD namespace

# endif
