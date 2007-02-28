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
	const AD<%Base%> %x%
	ADTape<%Base%>  *%ptr%
%$$
The argument $italic x$$ must a variable.

$head tape_id$$
$index tape_id$$
The syntax
$syntax%
	%id_table% = AD<%Base%>::tape_id()
%$$
returns the table of tape identifiers
where $italic id_table$$ has the prototype
$syntax%
	size_t %id_table%[CPPAD_MAX_NUM_THREADS] 
%$$
Furthermore, if $italic id$$ is the identifier corresponding to an active
tape, $syntax%%id% > 0%$$,
$syntax%
	%thread% = mod( %id%, omp_max_thread() )
%$$
is the corresponding OpenMP thread number, and
$syntax%
	%id% == %id_table%[%thread%]
%$$


$head tape_table$$
$index tape_table$$
The syntax
$syntax%
	%tape_table% = AD<%Base%>::tape_table()
%$$
returns the table of tape pointers
where $italic tape_table$$ has the prototype
$syntax%
	ADTape<%Base%> *%tape_table%[CPPAD_MAX_NUM_THREADS] 
%$$
Furthermore, if $italic id$$ corresponds to an active tape and
$syntax%
	%thread% = mod(%id%, omp_max_thread() )
%$$
then
$syntax%%
	tape_table%[%thread%]% != CPPAD_NULL
%$$.

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
	%ptr% = AD<%Base%>::tape_ptr(%id%)
%$$
returns the pointer to the tape corresponding to the tape identifier 
$italic id$$ (the corresponding tape must be active).
$pre

$$
The syntax
$syntax%
	%ptr% = AD<%Base%>::tape_ptr()
%$$
returns the a pointer to the tape corresponding to the current thread.
The corresponding tape is active if and only if $syntax%%ptr% == CPPAD_NULL%$$.
Note that if you know the corresponding tape identifier, it is faster
to use the other $code tape_ptr$$ syntax (because the current thread
need not be determined by an OpenMP system call).

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
ADTape<Base> *AD<Base>::tape_this(void) const
{	return tape_ptr(id_); }

// ----------------------------------------------------------------------
// Static functions
//
template <class Base>
size_t * AD<Base>::tape_id(void)
{	// assume all id numbers are initially zero
	static size_t table[CPPAD_MAX_NUM_THREADS];
	return table;
}	
template <class Base>
ADTape<Base> ** AD<Base>::tape_table(void)
{	// assume all pointers initially zero
	static ADTape<Base> *table[CPPAD_MAX_NUM_THREADS];
	CppADUnknownError( CPPAD_NULL == 0 );
	return table;
}
template <class Base>
bool AD<Base>::tape_active(size_t id)
{	size_t thread = id % omp_max_thread(0);
	return ( id > 0)  & (tape_id() [thread] == id );
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
	size_t *id_table = tape_id();
	if( id_table[thread] == 0 )
		id_table[thread] = thread + CPPAD_MAX_NUM_THREADS;

	// tape for this thread must be null at the start
	CppADUnknownError( tape_table() [ thread ] == CPPAD_NULL );
	tape_table() [thread] = new ADTape<Base>( id_table[thread] );

	return id_table[thread];
}

template <class Base>
void  AD<Base>::tape_delete(size_t id)
{	size_t thread = id % CPPAD_MAX_NUM_THREADS;
	CppADUnknownError( tape_table() [thread] != CPPAD_NULL );

	delete ( tape_table() [thread] );
	tape_table() [thread] = CPPAD_NULL;

	// increase the id for this thread in a way such that 
	// thread = id_table[thread] % CPPAD_MAX_NUM_THREADS
	tape_id() [thread] += CPPAD_MAX_NUM_THREADS;

	return;
}
template <class Base>
ADTape<Base> * AD<Base>::tape_ptr(size_t id)
{	CppADUnknownError( tape_active(id) );

	size_t thread = id % CPPAD_MAX_NUM_THREADS;
	ADTape<Base> *tape = tape_table() [thread];

	CppADUnknownError( tape != CPPAD_NULL );
	return tape;
}
template <class Base>
ADTape<Base> *AD<Base>::tape_ptr(void)
{
# ifdef _OPENMP
	size_t thread = static_cast<size_t> ( omp_get_thread_num() );
# else
	size_t thread = 0;
# endif
	return tape_table() [thread];
}

} // END CppAD namespace

# endif
