# ifndef CPPAD_ALLOCATOR_INCLUDED
# define CPPAD_ALLOCATOR_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin allocator$$
$spell
	CppAD
	const
	std
	deallocate
	deallocated
	ptrdiff
$$

$section The CppAD Allocator Template Class$$

$index standard, C++ memory allocator$$
$index C++, standard memory allocator$$
$index memory, C++ standard allocator$$
$index allocator, C++ standard$$

$head Syntax$$
$syntax%	allocator<%T%> %a%
%$$
The following is a list of the its member functions and operators:
$pre
$$
$cref%address%allocator%address%$$,
$cref%allocate%allocator%allocate%$$,
$cref%deallocate%allocator%deallocate%$$,
$cref%max_size%allocator%max_size%$$,
$cref%construct%allocator%construct%$$,
$cref%destroy%allocator%destroy%$$.

$head Purpose$$
The CppAD allocator template class is a fast allocator
that meets the requirements for a C++ standard memory allocator
(see section 20.1.5 of the 1998 C++ standard).

$head Type Definitions$$
$table
$bold Expression$$ $pre  $$
	$cnext $bold Type$$
$rnext
$syntax%allocator<%T%>::pointer%$$ $pre  $$
	$cnext $syntax%%T%*%$$
$rnext
$syntax%allocator<%T%>::const_pointer%$$ $pre  $$
	$cnext $syntax%const %T%*%$$
$rnext
$syntax%allocator<%T%>::referenced%$$ $pre  $$
	$cnext $syntax%%T%&%$$
$rnext
$syntax%allocator<%T%>::const_referenced%$$ $pre  $$
	$cnext $syntax%const %T%&%$$
$rnext
$syntax%allocator<%T%>::value_type%$$ $pre  $$
	$cnext $syntax%%T%$$
$rnext
$syntax%allocator<%T%>::size_type%$$ $pre  $$
	$cnext $syntax%std::size_t%$$
$rnext
$syntax%allocator<%T%>::difference_type%$$ $pre  $$
	$cnext $syntax%std::ptrdiff_t%$$
$rnext
$syntax%allocator<%T%>::rebind<%U%>::other%$$ $pre  $$
	$cnext $syntax%allocator<%U%>%$$
$tend

$head Variable Prototypes$$
$table
$bold Prototype$$  $cnext $bold Variable$$
$rnext
$syntax%allocator<%T%>&%$$ $cnext $syntax% %a%, %a1%, %a2%$$
$rnext
$syntax%allocator<%U%>%$$  $cnext $syntax% %b%$$
$rnext
$syntax%%T%*%$$            $cnext $syntax% %p%$$
$rnext
$syntax%const %T%*%$$      $cnext $syntax% %q%$$
$rnext
$syntax%%T%&%$$            $cnext $syntax% %r%$$
$rnext
$syntax%const %T%&%$$      $cnext $syntax% %s%$$
$rnext
$syntax%const %T%&%$$      $cnext $syntax% %t%$$
$rnext
$syntax%const %U%*%$$      $cnext $syntax% %u%$$
$rnext
$syntax%std::size_t%$$     $cnext $syntax% %n%$$
$tend

$head Constructors$$
$index allocator, constructor$$
$index constructor, allocator$$
The syntax
$syntax%
	allocator<%T%> %a%
%$$
constructs an allocator for objects with type $italic T$$.
The syntax
$syntax%
	allocator<%T%> %a%(%b%)
%$$
also constructs an allocator for objects with type $italic T$$
(the standard requires that the allocators $italic a$$ and $italic b$$
be equal, but all CppAD allocators are equal).

$head address$$
$index allocator, address$$
$index address, allocator$$
The syntax
$syntax%
	%p% = %a%.address(%r%)
%$$
sets $italic p$$ to point to the object $italic r$$ where 
$italic r$$ was allocated by
$syntax%%a1%.allocate%$$ where $syntax%%a1% == %a%$$.
The $syntax%
	%q% = %a%.address(%s%)
%$$
sets $italic q$$ to point to the object $italic s$$ where 
$italic s$$ was allocated by
$syntax%%a1%.allocate%$$ where $syntax%%a1% == %a%$$.

$head allocate$$
$index allocator, allocate$$
$index allocate, allocator$$
The syntax
$syntax%
	%p% = %a%.allocate(%n%)
%$$
set $italic p$$ to point to $italic n$$ objects
of type $italic T$$.
The corresponding objects are not constructed.
The $syntax%
	%p% = %a%.allocate(%n%, %u%)
%$$
set $italic p$$ to point to $italic n$$ objects
of type $italic T$$.
The corresponding objects are not constructed.
The pointer $italic u$$ is either zero
or a value returned by $syntax%%b%.allocate%$$
(and not yet passed to $code deallocate$$).

$head deallocate$$
$index allocator, deallocate$$
$index deallocate, allocator$$
The syntax
$syntax%
	%a%.deallocate(%p%, %n%)
%$$
The value $italic p$$ points to $italic n$$ objects
of type $italic T$$ that were allocated by 
$syntax%%a1%.allocate%$$ where $syntax%%a1% == %a%$$.
The memory is returned to the allocation pool.
The destructors for the corresponding objects
are not called.

$head max_size$$
$index max_size, allocator$$
$index allocator, max_size$$
The syntax
$syntax%
	%n% = %a%.max_size()
%$$
sets $italic n$$ to the largest value that is 
allowed to be passed to $syntax%%a%.allocate%$$. 

$head Equal$$
$index allocator, equal$$
$index equal, allocator$$
The syntax
$syntax%
	%a1% == %a2%
%$$
returns true if storage allocated by $italic a1$$ 
can be deallocated by $italic a2$$. 
The syntax
$syntax%
	%a1% != %a2%
%$$
returns $syntax%! ( %a1% == %a2% )%$$.

$head construct$$
$index allocator, construct$$
$index construct, allocator$$
The syntax
$syntax%
        %a%.construct(%p%, %t%)
%$$
calls the constructor for $italic T$$ objects using the memory
pointed to by $italic p$$ and the single argument $italic t$$.
The value $italic p$$ was obtained by
$syntax%%a1%.allocate%$$ where $syntax%%a1% == %a%$$.

$head destroy$$
$index allocator, destroy$$
$index destroy, allocator$$.
The syntax
$syntax%
        %a%.destroy(%p%)
%$$
calls the destructor for $italic T$$ objects using the object
pointed to by $italic p$$.
The value $italic p$$ was obtained by
$syntax%%a1%.allocate%$$ where $syntax%%a1% == %a%$$.

$children%
	example/allocator.cpp
%$$

$head Example$$
The file $cref/allocator.cpp/$$ 
contains an example and test of this routine.
It returns true if it succeeds and false otherwise.

$end
------------------------------------------------------------------------------
*/

# include <cstddef>
# include <limits>
# include <cppad/local/cppad_assert.hpp>

namespace CppAD { // BEGIN CppAD namespace

template <class T>
class allocator {

public:
	// type definitions
	typedef T*              pointer;
	typedef const T*        const_pointer;
	typedef T  &            reference;
	typedef const T&        const_reference;
	typedef T               value_type;
	typedef size_t          size_type;
	typedef std::ptrdiff_t  difference_type;
	template <class U> struct rebind
	{	typedef allocator<U> other; };

	// constructors
	explicit allocator(void)
	{ }
	template <class U>
	explicit allocator(allocator<U> const &b)
	{ }

	// address functions
	T* address(T& r) const
	{	return &r; }
	const T& address(const T& s)
	{	return &s; }


	// allocation function
	T* allocate(size_t n, const T* u = 0)
	{	void *v = ::operator new(n * sizeof(value_type));
		return reinterpret_cast<pointer>(v);
	}

	// deallocation function
	void deallocate(T* p, size_t n)
	{	CPPAD_ASSERT_KNOWN( 
			p != 0,
			"deallocate: p is equal to zero"
		);
		::operator delete(p); 
	}  

	// call constructor with memory specified
	void construct(T* p, const T& r)
	{	CPPAD_ASSERT_KNOWN( 
			p != 0,
			"construct: p is equal to zero"
		);
		new(p) T(r); 
	}

	// call destructor, but do no not free memory
	void destroy(T* p)
	{	CPPAD_ASSERT_KNOWN( 
			p != 0,
			"destroy: p is equal to zero"
		);
		p->~value_type(); 
	}

	// maximum allocation size (so no overflow can occur at multiply)
	size_t max_size(void) const
	{	static size_t max_allocate = 
		std::numeric_limits<size_t>::max() / sizeof(T);

		return max_allocate;
	}

	// equality
	template <class U> 
	bool operator==(const allocator<U> &b)
	{	return true; }

	// inequality
	template <class U> 
	bool operator!=(const allocator<U> &b)
	{	return false; }
};


} // END CppAD namespace

# endif
