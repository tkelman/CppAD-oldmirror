/* $Id$ */
# ifndef CPPAD_EXTEND_VECTOR_INCLUDED
# define CPPAD_EXTEND_VECTOR_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/omp_alloc.hpp>
CPPAD_BEGIN_NAMESPACE
/*!
\file extend_vector.hpp
File used to define the extend vector function
*/

/* -----------------------------------------------------------------------
$begin extend_vector$$
$spell
	vec
	const
	cppad.hpp
	num
	ptr
	omp_alloc
	openmp
	CppAD
$$

$section Fast Creation or Extension A Vector In an OpenMP Environment$$

$index extend_vector, openmp$$
$index vector, extend$$
$index vector, allocate$$
$index allocate, vector$$
$index openmp, vector$$

$head Syntax$$
$codei%include <cppad/extend_vector.hpp>
%$$
$icode%vec_out% = extend_vector(%size_in%, %vec_in%, %size_min%, %size_out%)%$$

$head Purpose$$
Create a new vector, or extend an existing vector, using $cref/omp_alloc/$$
a fast memory allocator that works well in a multi-threading OpenMP environment.

$head Include$$
The file $code cppad/extend_vector.hpp$$ is included by
$code cppad/cppad.hpp$$, but it can also be included separately with out
the rest of the $code CppAD$$ routines.

$head size_in$$
This argument has prototype
$codei%
	size_t %size_in%
%$$
It is the number of elements that are in the vector $icode vec_in$$.

$head vec_in$$
This argument has prototype
$codei%
	const %Type%* %vec_in%
%$$.
If $icode size_in$$ is zero, $icode vec_in$$ is not used.

$subhead Thread$$
If $icode size_in$$ is not zero, 
the $cref/current thread/get_thread_num/$$ must be the
same as when $icode vec_in$$ was obtained as the return value of
a previous call to $icode extend_vector$$.
There is an exception to this rule:
when the current execution mode is sequential
(not $cref/parallel/in_parallel/$$) the current thread number does not matter.

$subhead Upon Return$$
The destructor for all $icode size_in$$ elements of
$icode vec_in$$ is called and $icode vec_in$$ is no longer 
a valid pointer after this call.

$head size_min$$
This argument has prototype
$codei%
	size_t %size_min%
%$$
It specifies the minimum number of elements in the vector $icode vec_out$$.
In the special case where it is zero,
$icode size_out$$ and $icode vec_out$$ are also zero.
In this case, we are only returning the memory in $icode vec_in$$ to the 
available pool for the current thread.
To go further and make the memory generally available,
see $cref/free_available/$$.

$head size_out$$
This argument has prototype
$codei%
	size_t& %size_out%
%$$
It's input value does not matter.
Upon return, it is the actual number of elements 
in the vector $icode vec_out$$ 
($icode% size_min %<=% size_out%$$).
In the special case where $icode size_min$$ is zero, $icode size_out$$
is also zero.

$head vec_out$$
The return value $icode vec_out$$ has prototype
$codei%
	%Type%* %vec_out%
%$$
It is vector with $icode size_out$$ elements.
The copy constructor is used to copy the first
$codei%min(%size_in%, %size_min%)%$$ elements of $icode vec_in$$ to 
the corresponding elements of $icode vec_out$$.
The default constructor is used to initialize the rest
of the elements of $icode vec_out$$.
In the special case where $icode size_min$$ is zero, $icode vec_out$$
is also zero.

$children%
	example/extend_vector.cpp
%$$
$head Example$$
The file $cref/extend_vector.cpp/$$ contains an example and test of
$code extend_vector$$.
It returns true, if the test succeeds, and false otherwise.

$end
*/
/*!
 Extend a vector to a longer length.

\param size_in [in]
The size of the vector \c vec_in.

\param vec_in [in]
A vector with at least \c size_in elements.
The \c Type destructor will be called for each of the
\c size_in elements of \c vec_in.
The pointer \c vec_in will no longer be valid after this call.
If \c size_in is zero, \c vec_in is not used.

\param size_min [in]
The minimum number of elements in the result vector.
If \c size_min is zero, then we are only returning
the memory in \c vec_in to the 
available pool for the current thread.
To go further and make the memory generally available,
see \c free_available.

\param size_out [out]
The actual number of elements in the result vector
(<code> size_min <= size_out</code>).
If \c size_min is zero, \c size_out will also be zero.

\return
pointer to the beginning of the result vector.
The copy constructor is used to initialize the first 
<code>min(size_in, size_min)</code> elements of the return vector.
The default constructor is used for the other elements.
If \c size_min is zero, the return value will also be zero.
*/
template <class Type>
Type* extend_vector(
	size_t       size_in  ,
	Type*        vec_in   ,
	size_t       size_min ,
	size_t&      size_out )
{	size_t i;
	void*  v_ptr;

	// get memory for the output vector
	Type* vec_out;
	if( size_min == 0 )
	{	size_out = 0;
		vec_out  = 0;
	}
	else
	{	size_t min_bytes = size_min * sizeof(Type);
		size_t num_bytes;
		v_ptr    = omp_alloc::get_memory(min_bytes, num_bytes);
		vec_out  = reinterpret_cast<Type*>(v_ptr);
		size_out = num_bytes / sizeof(Type);
	}
	
	// copy the first n_copy elements from vec_in to vec_out
	size_t n_copy = std::min(size_in, size_min);
	for(i = 0; i < n_copy; i++)
		new(vec_out + i) Type( vec_in[i] );	

	// use default constructor for other elements
	for(i = n_copy; i < size_out; i++)
		new(vec_out + i) Type();

	// destroy elements in vec_in
	for(i = 0; i < size_in; i++)
		(vec_in + i)->~Type();

	// return vec_in to available memory for this thread
	if( size_in > 0 )
		omp_alloc::return_memory( reinterpret_cast<void*>(vec_in) );

	return vec_out;
}
CPPAD_END_NAMESPACE
# endif
