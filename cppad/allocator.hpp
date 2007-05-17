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
	typedef
	std
	ptrdiff
	const
	deallocate
	deallocated
$$

$section The CppAD Standard Memory Allocator$$

$head Syntax$$
$syntax%typedef Allocator<%Type%>  %A%;
%$$
$syntax%typedef Allocator<%Other%> %B%;
%$$
$syntax%Allocator<%Type%>  %a%
%$$
$syntax%Allocator<%Other%> %b%
%$$


$head Type Definitions$$
$table
$bold Member Type$$
	$cnext $bold Definition$$ 
$rnext
$syntax%%A%::value_type%$$             $cnext $syntax%%Type%$$
$rnext
$syntax%%A%::size_t%$$                 $cnext $syntax%std::size_t&%$$
$rnext
$syntax%%A%::difference_type%$$        $cnext $syntax%std::ptrdiff_t&%$$
$rnext
$syntax%%A%::pointer%$$                $cnext $syntax%%Type%*%$$
$rnext
$syntax%%A%::const_pointer%$$          $cnext $syntax%const %Type%*%$$
$rnext
$syntax%%A%::reference%$$              $cnext $syntax%%Type%&%$$
$rnext
$syntax%%A%::const_reference%$$        $cnext $syntax%const %Type%&%$$
$rnext
$syntax%%A%::rebind<%Other%>::other%$$ $pre  $$
	$cnext $syntax%%B%$$
$tend

$head Allocator$$
The syntax
$syntax%
	Allocator<%Type%> %a%
%$$
constructs an allocator $italic a$$ of type $italic A$$.
$pre

$$
The syntax
$syntax%
	Allocator<%Type%> %c%(%b%)
%$$
constructs an allocator $italic c$$ of type $italic A$$ where
the argument $italic b$$ is $code const$$.
The expression $syntax%%c% == %b%$$ must return true
(see $cref/allocator/allocator/Equal/$$ below).

$head address$$
If $italic p$$ and $italic r$$ have prototypes
$syntax%
	%Type% *%p%
	%Type% &%r%
%$$
the syntax
$syntax%
	%p% = %a%.address(%r%)
%$$
sets $italic p$$ so it points to the object $italic r$$.
$pre

$$
If $italic p$$ and $italic r$$ have prototypes
$syntax%
	const %Type% *%p%
	const %Type% &%r%
%$$
the syntax
$syntax%
	%p% = %a%.address(%r%)
%$$
sets $italic p$$ so it points to the object $italic r$$.

$head allocate$$
If $italic n$$ and $italic p$$ have prototypes
$syntax%
	std::size_t %n%
	%Type%       *%p%
%$$
the syntax
$syntax%
	%p% = %a%.allocate(%n%)
%$$
sets $italic p$$ so it points to $italic n$$ sequential objects
of type $italic Type$$.
$pre

$$
Suppose that $italic n$$, $italic p$$, and $italic h$$ have prototypes
$syntax%
	std::size_t %n%
	%Type%       *%p%
	%Other%      *%h%
%$$
and the value of $italic h$$ was previously returned by an 
$cref/equal allocator/allocator/Equal/$$ 
(but not yet deallocated).
The syntax
$syntax%
	%p% = %a%.allocate(%n%, %h%)
%$$
sets $italic p$$ so it points to $italic n$$ sequential objects
of type $italic Type$$.
The value $italic h$$ acts like a hint to improve allocation speed.

$head deallocate$$
Suppose that $italic n$$ and $italic p$$ have prototypes
$syntax%
	std::size_t %n%
	%Type%       *%p%
%$$
where $italic n$$ and $italic p$$ correspond to values
previously returned by an equal allocator's $code allocate$$ function.
The syntax
$syntax%
	%a%.deallocate(%p%, %n%)
%$$
frees the memory that was allocated by the previous call.
The value $italic p$$ in the call to $code deallocate$$ must 
not be zero (i.e., null).

$head max_size$$
If $italic n$$ has prototype
$syntax%
	std::size_t %n%
%$$
the syntax
$syntax%
	%n% = %a%.max_size()
%$$
returns the largest value that can meaningfully be passed to 
$syntax%%a%.allocate%$$.

$head Equal$$
If $italic a1$$ and $italic a2$$ are allocators of type $italic A$$,
the syntax
$syntax%
	%a1% == %a2%
%$$ 
returns true if memory allocated by $italic a1$$ 
can be deallocated by $italic a2$$ and vise versa.
The syntax
$syntax%
	%a1% != %a2%
%$$ 
is the same as $syntax%! (%a1% == %a2%)%$$.

$head construct$$
If $italic p$$ and $italic r$$ have the prototypes
$syntax%
	%Type% *%p%
	%Type% &%r%
%$$
the syntax
$syntax%
	%a%.construct(%p%, %r%)
%$$
calls the constructor for $italic Type$$ using the memory
pointed to by $italic p$$ and the single argument $italic r$$.

$head destroy$$
If $italic p$$ has prototype
$syntax%
	%Type% *%p%
%$$
the syntax
$syntax%
	%a%.destroy(%p%)
%$$
calls the constructor for $italic Type$$ using the object
pointed to by $italic p$$.

$children%
	example/allocator.cpp
%$$
$head Example$$
The file
$xref/allocator.cpp/$$
contains an example and test of this routine.
It returns true if it succeeds and false otherwise.

$end
*/

# include <cstddef>
# include <limits>
# include <cppad/local/cppad_error.hpp>

namespace CppAD { // BEGIN CppAD namespace

template <class Type> 
class Allocator {
public:
	// Type definitions
	typedef Type                       value_type;
	typedef std::size_t                size_type;
	typedef std::ptrdiff_t             difference_type;
	typedef Type*                      pointer;
	typedef const Type*                const_pointer;
	typedef Type&                      reference;
	typedef const Type&                const_reference;

	// type corresponding to another allocator
	template <class Other>
	struct rebind { typedef Allocator<Other> other; };


	// constructors
	explicit Allocator(void)
	{ }
	template<class Other>
	explicit Allocator(Allocator<Other> const &b)
	{ }

	// address functions
	pointer address(reference r) const
	{	return &r; }
	const pointer address(const_reference r) const
	{	return &r; }
	
	// allocation function
	pointer allocate(size_type n, const_pointer h = 0)
	{	void *v = ::operator new(n * sizeof(value_type));
		return reinterpret_cast<pointer>(v);
	}

	// deallocation function
	void deallocate(pointer p, size_type n)
	{	CppADUsageError( 
			p != 0,
			"deallocate: p is equal to zero"
		);
		::operator delete(p); 
	}  

	// call constructor with memory specified
	void construct(pointer p, const_reference r)
	{	CppADUsageError( 
			p != 0,
			"construct: p is equal to zero"
		);
		new(p) Type(r); 
	}

	// call destructor, but no not free memory
	void destroy(pointer p)
	{	CppADUsageError( 
			p != 0,
			"destroy: p is equal to zero"
		);
		p->~value_type(); 
	}

	// size (make sure no overflow on call to allocator)
	size_type max_size(void) const
	{	return std::numeric_limits<size_type>::max() 
			/ sizeof(value_type);
	}

	// equality
	template <class Other> 
	bool operator==(const Allocator<Other> &b)
	{	return true; }

	// inequality
	template <class Other> 
	bool operator!=(const Allocator<Other> &b)
	{	return false; }

};

} // END CppAD namespace
# endif
