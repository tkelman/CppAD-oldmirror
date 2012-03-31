/* $Id$ */
# ifndef CPPAD_TAPE_LINK_INCLUDED
# define CPPAD_TAPE_LINK_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/local/define.hpp>
# include <cppad/thread_alloc.hpp>
# include <cppad/local/cppad_assert.hpp>

// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/thread_alloc.hpp>

CPPAD_BEGIN_NAMESPACE
/*!
\file tape_link.hpp
Routines that Link AD<Base> to corresponding ADTape<Base> Objects
\ref tape_link.hpp.

\defgroup tape_link.hpp tape_link.hpp
*/
/* \{ */

/*!
Get a pointer to tape 
that records AD<Base> operations for the current thread.

\tparam Base
is the base type corresponding to AD<Base> operations and the tape.

\param tape_id
is the identifier for the tape that is recording
AD<Base> operations for this thread current thread.
The thread corresponding to the tape_id is
\code
	thread = thread_alloc::thread_num()  if tape_id == 0 else
	thread = tape_id % CPPAD_MAX_NUM_THREADS 
\endcode

\param job
- 
\c tape_ptr_new:
There must not be a tape recording AD<Base> operations for this thread 
when \c tape_ptr is called and there will be a new tape when it returns.  
The argument \c tape_id must be zero.
The return value points to the new tape.
-
\c tape_ptr_delete:
There must be a tape recording AD<Base> operations for this thread
what \c tape_ptr is called and there will be no such tape when it returns.
The argument \c tape_id must be the identifier for the tape that
was recording operations.
The return value will be \c CPPAD_NULL.
-
\c tape_ptr_return_null_ok:
A pointer to the tape that is currently recording 
AD<Base> operations for this thread is returned.
If no such tape exists, \c CPPAD_NULL is returned.
-
\return
The return value is a pointer to the tape that is currently
recording AD<Base> operations for this thread.
*/
template <class Base>
inline ADTape<Base>* AD<Base>::tape_ptr(
	size_t        tape_id     , 
	tape_ptr_job  job         ) 
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	static ADTape<Base> *tape_table[CPPAD_MAX_NUM_THREADS];
	static size_t          id_table[CPPAD_MAX_NUM_THREADS];

	size_t thread;
	if( tape_id == 0 )
		thread = thread_alloc::thread_num();
	else	thread = tape_id % CPPAD_MAX_NUM_THREADS;

	CPPAD_ASSERT_KNOWN(
		thread == thread_alloc::thread_num(),
		"Attepmt to use a variable or tape created by a different thread"
	);
	CPPAD_ASSERT_KNOWN(
		thread < thread_alloc::num_threads(),
		"Current thread number is greater than num_threads;\n"
		"See thread_alloc::parallel_setup for setting these values."
	);
	CPPAD_ASSERT_UNKNOWN( 
		(id_table[thread] == 0)
		| (id_table[thread] % CPPAD_MAX_NUM_THREADS == thread)
	); 

	ADTape<Base>*tape = tape_table[thread];
	switch( job )
	{
		case tape_ptr_return_null_ok:
		return tape; 

		case tape_ptr_new:
		CPPAD_ASSERT_UNKNOWN( tape_id == 0 );
		if( id_table[thread] == 0 )
		{	// initialize id > 1 and thread == id % CPPAD_MAX_NUM_THREADS
			id_table[thread] = thread + 2 * CPPAD_MAX_NUM_THREADS;
		}
		// else id_table[thread] has been set to its new value by tape_delete
		CPPAD_ASSERT_UNKNOWN( tape  == CPPAD_NULL );
		tape_table[thread] = new ADTape<Base>( id_table[thread] );
		return tape_table[thread];

		case tape_ptr_delete:
		CPPAD_ASSERT_UNKNOWN( tape != CPPAD_NULL );
		CPPAD_ASSERT_UNKNOWN( tape_id == tape->id_  );
		CPPAD_ASSERT_UNKNOWN( tape->id_  == id_table[thread] );
		CPPAD_ASSERT_KNOWN( 
			id_table[thread] <= 
				std::numeric_limits<CPPAD_TAPE_ID_TYPE>::max() - 
					CPPAD_MAX_NUM_THREADS ,
			"To many different tapes given the type used for "
			"CPPAD_TAPE_ID_TYPE"
		);
		id_table[thread]  += CPPAD_MAX_NUM_THREADS;
		delete ( tape );
		tape_table[thread] = CPPAD_NULL;
		return CPPAD_NULL;

		default:
		CPPAD_ASSERT_UNKNOWN( false );
	}
	// should not get here, but return value to avoid compiler warning
	return CPPAD_NULL;
}

/*!
Get a pointer to tape that records AD<Base> operations for the current thread.

\tparam Base
is the base type corresponding to AD<Base> operations.

\par thread
is the index that identifes the current thread.
If \c _OPENMP is not defined, \c thread is zero.
<tt>0 <= thread < thread_alloc::num_threads()</tt>.

\par id_
This routine uses AD<Base>::id_ to determine the tape identifier 
corresponding to the current object.
It must hold that <tt>thread = tape_id_ % CPPAD_MAX_NUM_THREADS</tt>.
Note this routine should be faster when NDEBUG is defined (?) than
calling \c tape_ptr without the \c id argument.

\return
The return value \c r is a pointer to the tape that records AD<Base> operations
for the current thread.
If <tt>r == CPPAD_NULL</tt>, there is no tape currently
recording AD<Base> operations for the specified thread.
*/

template <class Base>
inline ADTape<Base> *AD<Base>::tape_this(void) const
{	
	return tape_ptr(tape_id_, tape_ptr_return_null_ok );
}

/* \} */
CPPAD_END_NAMESPACE
# endif
