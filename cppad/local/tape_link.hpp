# ifndef CPPAD_TAPE_LINK_INCLUDED
# define CPPAD_TAPE_LINK_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin tape_link$$ $comment CppAD Developer Documentation$$

$section Routines that Link AD<Base> and ADTape<Base> Objects$$
$spell
	omp
	bool
	ptr
	const
	tbl
	atexit
$$

$head tape_this$$
$index tape_this$$
The syntax
$syntax%
	%ptr% = %x%.tape_this()
%$$
returns the tape corresponding to $italic x$$
where $italic x$$ and $italic ptr$$ have the prototypes
$syntax%
	const AD<%Base%> &%x%
	ADTape<%Base%>  *%ptr%
%$$
The argument $italic x$$ must a variable.

$head id_handle$$
$index id_handle$$
The syntax
$syntax%
	%id% = AD<%Base%>::id_handle()
%$$
returns a pointer to the currently active tape identifier
(for the current OpenMP thread).
If no tape is currently active, it is the identifier for the next
active tape.
The return value $italic id$$ has prototype
$syntax%
	size_t *%id%
%$$


$head tape_handle$$
$index tape_handle$$
The syntax
$syntax%
	%tape% = AD<%Base%>::tape_handle()
%$$
returns a pointer to a pointer to the current tape
(for the current OpenMP thread).
The return value has prototype
$syntax%%
	ADTape<%Base%> **tape
%$$.
If the tape is not currently active,
$syntax%
	*%tape% == CPPAD_NULL
%$$

$head tape_active$$
$index tape_active$$
The syntax
$syntax%
	%flag% = AD<%Base%>::tape_active(%id%)
%$$
set $italic flag$$ true if the tape identifier $italic id$$ 
corresponds to an active tape and false otherwise.
The argument $italic id$$ and return value $italic flag$$
have the prototypes
$syntax%
	size_t %id%
	bool   %flag%
%$$

$head tape_new$$
$index tape_new$$
The syntax
$syntax%
	%id% = AD<%Base%>::tape_new()
%$$
creates a new tape and returns the corresponding tape identifier.
The resulting tape identifier is not equal to zero or to any
of the previous values return by $code tape_new$$.
The return value $italic id$$ has prototype
$syntax%
	size_t %id%
%$$
This function is only called by the user $code Independent$$ routine
and hence usage errors can be reported as coming from that routine.

$head tape_delete$$
$index tape_delete$$
The syntax
$syntax%
	AD<%Base%>::tape_delete(%id%)
%$$
deletes the tape corresponding to the tape identifier $italic id$$.
The corresponding $code tape_id$$ entry is set to a value larger
than any previous tape identifier that corresponds to the same thread.

$head tape_ptr$$
$index tape_ptr$$
The syntax
$syntax%
	%ptr% = AD<%Base%>::tape_ptr()
%$$
returns the a pointer to the tape corresponding to the current thread.
The corresponding tape is active if and only if $syntax%%ptr% == CPPAD_NULL%$$.
The syntax
$syntax%
	%ptr% = AD<%Base%>::tape_ptr(%id%)
%$$
does the same thing but if NDEBUG is not defined, it also check that
the $italic id$$ is the corresponding tape identifier and that $italic ptr$$
is not equal to $code CPPAD_NULL$$.
The argument $italic id$$ has prototype
$syntax%
	size_t %id%
%$$


$end
----------------------------------------------------------------------------
*/

# ifdef _OPENMP
# include <omp.h>
# endif

// BEGIN CppAD namespace
namespace CppAD { 

// ----------------------------------------------------------------------
template <class Base>
inline ADTape<Base> *AD<Base>::tape_this(void) const
{	CppADUnknownError( tape_active(id_) );
	CppADUnknownError( *tape_handle() != CPPAD_NULL );

	return *tape_handle();
}
// ----------------------------------------------------------------------
// Static functions
//
template <class Base>
inline size_t * AD<Base>::id_handle(void)
{	// assume all id numbers are initially zero
	static size_t id;
# ifdef _OPENMP
# pragma omp threadprivate(id)
# endif
	return &id;
}	
template <class Base>
inline ADTape<Base> ** AD<Base>::tape_handle(void)
{	// assume all pointers initially zero
	static ADTape<Base> *tape;
# ifdef _OPENMP
# pragma omp threadprivate(tape)
# endif
	CppADUnknownError( CPPAD_NULL == 0 );
	return &tape;
}
template <class Base>
inline bool AD<Base>::tape_active(size_t id)
{	size_t check  = *id_handle();
	return (id > 0)  & (check == id );
}
template <class Base>
size_t  AD<Base>::tape_new(void)
{
# ifdef _OPENMP
	size_t thread = static_cast<size_t> ( omp_get_thread_num() );
# else
	size_t thread = 0;
# endif
	CppADUsageError(
	thread < omp_max_thread(0),
	"Independent: OpenMP thread number is >= omp_max_thread setting"
	);

	// initialize so that id > 0 and thread == id % CPPAD_MAX_NUM_THREADS
	size_t *id = id_handle();
	if( *id == 0 )
		*id = thread + CPPAD_MAX_NUM_THREADS;

	// tape for this thread must be null at the start
	ADTape<Base> **tape = tape_handle();
	CppADUnknownError( *tape  == CPPAD_NULL );
	*tape = new ADTape<Base>( *id );

	return *id;
}

template <class Base>
void  AD<Base>::tape_delete(size_t id)
{
	ADTape<Base> **tape = tape_handle();

	CppADUnknownError( *tape != CPPAD_NULL );

	delete ( *tape );
	*tape = CPPAD_NULL;

	// increase the id for this thread in a way such that 
	// thread = id % CPPAD_MAX_NUM_THREADS
	*id_handle() += CPPAD_MAX_NUM_THREADS;

	return;
}
template <class Base>
inline ADTape<Base> *AD<Base>::tape_ptr(void)
{	return *tape_handle(); }

template <class Base>
inline ADTape<Base> *AD<Base>::tape_ptr(size_t id)
{	CppADUnknownError( tape_active(id) );
	CppADUnknownError( *tape_handle() != CPPAD_NULL );
	return *tape_handle(); 
}

} // END CppAD namespace

# endif
