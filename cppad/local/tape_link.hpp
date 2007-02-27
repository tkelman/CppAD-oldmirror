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
	%id_tbl% = AD<%Base%>::tape_id()
%$$
returns the table of tape identifiers
where $italic id_tbl$$ has the prototype
$syntax%
	size_t %id_tbl%[CPPAD_LENGTH_TAPE_TABLE] 
%$$
If $syntax%%id% = %id_table%[%i%]%$$
either $italic id$$ is zero or it is the identifier
for an active tape.
Furthermore, if $italic id$$ corresponds to an active tape,
$syntax%
	%i% = mod( %id%, CPPAD_LENGTH_TAPE_TABLE )
%$$


$head tape_table$$
$index tape_table$$
The syntax
$syntax%
	%tape_tbl% = AD<%Base%>::tape_table()
%$$
returns the table of tape pointers
where $italic tape_tbl$$ has the prototype
$syntax%
	ADTape<%Base%> *%tape_tbl%[CPPAD_LENGTH_TAPE_TABLE] 
%$$
Furthermore $syntax%%id_table%[%i%]% == 0%$$ if and only if
$syntax%%tape_tbl%[%i%]% == CPPAD_NULL%$$.

$head tape_atexit$$
$index tape_atexit$$
The function $code tape_atexit$$ has the prototype
$syntax%
	void AD<%Base%>::tape_atexit(void)
%$$
It deletes all of the tape pointer that records that are currently 
store in the $cref/tape_table/tape_link/tape_table/$$
and are not equal to $code CPPAD_NULL$$.

$head tape_active_count$$
$index tape_active_count$$
The syntax
$syntax%
	%count% = AD<%Base%>::tape_active_count(%inc%)
%$$
increments the active tape counter by $italic inc$$
and returns the resulting count.
The argument $italic inc$$ and return value $italic count$$ 
have the following prototypes:
$syntax%
	int    %inc%
	size_t %count%
%$$
The argument $italic inc$$ must be -1, 0, or +1.
The resulting value for the counter must be 
greater than or equal zero and less than $code CPPAD_LENGTH_TAPE_TABLE$$.

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

$head tape_new_id$$
$index tape_new_id$$
The syntax
$syntax%
	%id% = AD<%Base%>::tape_new_id()
%$$
creates a new tape and returns the corresponding tape identifier.
The resulting tape identifier is not equal to zero or to any
of the previous values return by $code tape_new_id$$.
The return value $italic id$$ has prototype
$syntax%
	size_t %id%
%$$

$head tape_delete$$
$index tape_delete$$
The syntax
$syntax%
	AD<%Base%>::tape_delete(%id%)
%$$
deletes the tape corresponding to the tape identifier $italic id$$.
The corresponding $code tape_id$$ entry is set to zero
and the corresponding $code tape_table$$ entry is st to $code CPPAD_NULL$$.

$head tape_ptr$$
$index tape_ptr$$
The syntax
$syntax%
	%ptr% = AD<%Base%>::tape_ptr(%id%)
%$$
returns the pointer to the tape corresponding to the tape identifier 
$italic id$$ (the tape must be active).

$head tape_any$$
$index tape_any$$
The syntax
$syntax%
	%ptr% = AD<%Base%>::tape_any(%id%)
%$$
returns the a pointer to one of the active tapes
If $cref/tape_active_count/tape_link/tape_active_count/$$ is one,
this is the only active tape.
If it is zero, $code tape_any$$ returns $code CPPAD_NULL$$.

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
{	size_t thread = id % CPPAD_MAX_NUM_THREADS;
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
	// check for this error even in optimized code because 
	// it is not yet part of user documentation.
	if( thread >= CPPAD_MAX_NUM_THREADS )
	{	bool known       = true;
		int  line        = __LINE__;
        	const char *file = __FILE__;
		const char *exp  = "thread < CPPAD_MAX_NUM_THREADS";
        	const char *msg  = "too many threads are active.";
		ErrorHandler::Call(known, line, file, exp, msg);
	}

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
