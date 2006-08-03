# ifndef CppADAllocVecIncluded
# define CppADAllocVecIncluded

// BEGIN SHORT COPYRIGHT
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */
// END SHORT COPYRIGHT
/*
$begin AllocVec$$
$spell
	Alloc
	Vec
$$

$section Vector Allocation Routine$$

$table 
$bold Syntax$$
$cnext $syntax%AllocVec<%Type%> %v%(int %length%)%$$
$rnext
$cnext $syntax%%Type%& %v%[int %i%]%$$
$rnext
$cnext $syntax%(%Type%*) %v%$$
$tend

$fend 25$$

$head Description$$
The $code AllocVec$$ template class 
can be used to allocate and automatically free vectors
with an arbitrary type.
$syntax%

AllocVec<%Type%> %v%(int %length%)
%$$
This syntax allocates memory for a vector of the specified
length and with elements of the specified type. The argument
$italic length$$ must be greater than or equal to zero.
$syntax%

%Type%& %v%[int %i%]
%$$
This syntax returns a reference to the $th i$$ element in 
the vector $italic v$$.
The value $italic i$$ must be between zero and $syntax%%length%-1%$$
where $italic length$$ is the value in the constructor for $italic v$$.
$syntax%

(%Type%*) %v%
%$$
This syntax returns a copy of the pointer to the 
memory where the $italic length$$ elements of $italic v$$ are stored
where $italic length$$ is the value in the constructor for $italic v$$.

$children%
	Adolc/AllocVec.cpp
%$$

$head Example$$
The file
$xref/AllocVec.cpp/$$
contains an example and test of these operations.
It returns true if it succeeds and false otherwise.

$end
------------------------------------------------------------------------------
*/



# include <cassert>

// BEGIN CppAD namespace
namespace CppAD {

template <class Type>
class AllocVec {
public:
	// constructor
	AllocVec(int length_) : length(length_) 
	{	assert( length >= 0 );
		if( length == 0 )
			vec = 0;
		else	vec = new Type [length]; 
	}

	// destructor
	~AllocVec()
	{	if( length > 0 )
			delete [] vec; 
	}

	Type &operator[](int i)
	{	assert( i < length);
		assert( 0 <= i );

		return vec[i];
	}

	// pointer to the memory
	operator Type* ()
	{	return vec; }

private:
	const int length;

	Type *vec;
};

} // END CppAD namespace

# endif
