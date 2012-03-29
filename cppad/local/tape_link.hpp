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
Routines that Link AD<Base> and ADTape<Base> Objects \ref tape_link_hpp.

All the routines are AD<Base> member functions and, except for \c tape_this, 
they are all static functions. 

\defgroup tape_link_hpp  tape_link.hpp
*/
/* \{ */

/*!
Map from a tape identifier to the corresponding thread number.

\param tape_id
is the identifier for the tape. 

\return
The thread number that should correspond to this tape id 
(if user does not try to use a variable created by a different thread).
*/
inline size_t tape_id2thread_num(size_t tape_id)
{	return tape_id % CPPAD_MAX_NUM_THREADS; }


/*!
Get the identifier for the tape 
that records AD<Base> operations for the specified thread.

\tparam Base
is the base type corresponding to AD<Base> operations.

\param thread
is the index that identifes the current thread.
<tt>0 <= thread < thread_alloc::num_threads()</tt>.

\return
The return value \c r 
is a pointer to the tape identifier for the current thread.
It can be used to get or set the value of <tt>*r</tt> and
any such setting should abide by the following rules:
\li
Only the routines AD<Base>::tape_new and AD<Base>::tape_delete
change the value of <tt>*r</tt>.
\li
If <tt>*r == 0</tt>, there is no tape currently recording for this thread.
\li
If <tt>*r != 0</tt>, there current is a tape recording for this thread.
Furthermore the identifier satisfies the conditions
<tt>*r > CPPAD_MAX_NUM_THREADS</tt> , and
<tt>*r % CPPAD_MAX_NUM_THREADS == thread</tt>. 
\li
The value of <tt>*r</tt> must increase each time it is changed.
*/
template <class Base>
inline size_t * AD<Base>::id_handle(size_t thread)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	// Section 3.6.2 of the 1998 C++ standard specifies that storage for
	// static objects will be zero initalized.
	static size_t id_table[CPPAD_MAX_NUM_THREADS];

	CPPAD_ASSERT_UNKNOWN( thread < thread_alloc::num_threads() );
	CPPAD_ASSERT_UNKNOWN( 
		(id_table[thread] == 0)
		| (id_table[thread] % CPPAD_MAX_NUM_THREADS == thread)
	); 
	return id_table + thread;
}

/*!
Get a pointer to tape 
that records AD<Base> operations for the current thread.

\tparam Base
is the base type corresponding to AD<Base> operations.

\par thread
is the index that identifes the current thread.
If \c _OPENMP is not defined, \c thread is zero.
<tt>0 <= thread < thread_alloc::num_threads()</tt>.

\param id
is the identifier for the tape that is currently recording
AD<Base> operations for the current thread.
It must hold that <tt>thread = id % CPPAD_MAX_NUM_THREADS</tt>.
Note this routine should be faster when NDEBUG is defined (?) than
calling \c tape_ptr without the \c id argument.

\return
The return value \c r is a pointer to the tape that records AD<Base> operations
for the current thread.
If <tt>r == CPPAD_NULL</tt>, there is no tape currently
recording AD<Base> operations for the specified thread.
*/
template <class Base>
inline ADTape<Base> *AD<Base>::tape_ptr(
	size_t        thread      , 
	tape_ptr_job  job         ) 
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	static ADTape<Base> *tape_table[CPPAD_MAX_NUM_THREADS];

	CPPAD_ASSERT_KNOWN(
		thread == thread_alloc::thread_num(),
		"Attepmt to use a variable that was created by a different thread"
	);
	CPPAD_ASSERT_KNOWN(
		thread < thread_alloc::num_threads(),
		"Current thread number is greater than num_threads;\n"
		"See thread_alloc::parallel_setup for setting these values."
	);
	CPPAD_ASSERT_UNKNOWN( 
		job != tape_ptr_return_null_error || tape_table[thread] != CPPAD_NULL 
	);

	switch( job )
	{
		case tape_ptr_return_null_error:
		case tape_ptr_return_null_ok:
		return tape_table[thread];

		case tape_ptr_new:
		CPPAD_ASSERT_UNKNOWN( tape_table[thread]  == CPPAD_NULL );
		tape_table[thread] = new ADTape<Base>( *id_handle(thread) );
		break;

		case tape_ptr_delete:
		CPPAD_ASSERT_UNKNOWN( tape_table[thread]  != CPPAD_NULL );
		delete ( tape_table[thread] );
		tape_table[thread] = CPPAD_NULL;
		break;

		default:
		CPPAD_ASSERT_UNKNOWN( false );
	}
	return CPPAD_NULL;
}

/*!
Create a new tape that records AD<Base> operations for current thread.

\tparam Base
is the base type corresponding to AD<Base> operations.

\par thread
Let \c thread denote the current thread number
see \c thread_alloc::thread_num(). 
If \c _OPENMP is not defined, \c thread is zero.
It is a user error if <tt>thread >= thread_alloc::num_threads()</tt>.

\par tape_ptr
It is assumed that <tt>tape_ptr(thread, true) == CPPAD_NULL</tt>
when \c tape_new is called.
Upon return <tt>tape_ptr(thread)</tt> is a pointer
to the new ADTape<Base> tape that was created.

\par tape_id
We use \c id to denote <tt>AD<Base>::tape_id(thread)</tt>.
If <tt>*id</tt> is zero when \c tape_new is called,
a new value is chosen by \c tape_new.
Otherwise, <tt>*id</tt> is not modified.

\return
The return value \c id is the identifier for the new tape and satisfies
the following conditions:
\li
<tt>id > CPPAD_MAX_NUM_THREADS</tt>
\li
<tt>thread = id % CPPAD_MAX_NUM_THREADS</tt>
\li
Each call to \c tape_new returns a different value \c id.
*/
template <class Base>
size_t  AD<Base>::tape_new(void)
{
	size_t thread       = thread_alloc::thread_num();
	size_t *id          = id_handle(thread);

	CPPAD_ASSERT_KNOWN(
	thread < thread_alloc::num_threads(),
	"Independent: current thread_num is greater than num_threads;\n"
	"See parallel_setup for setting these values."
	);

	// initialize so that id > 1 and thread == id % CPPAD_MAX_NUM_THREADS
	if( *id == 0 )
		*id = thread + 2 * CPPAD_MAX_NUM_THREADS;
	// else *id has been set to its new value by tape_delete

	// create a new tape
	tape_ptr(thread, tape_ptr_new );

	return *id;
}

/*!
Delete the tape, and advance the id number, corresponding to AD<Base> 
operations for the current thread.

\tparam Base
is the base type corresponding to AD<Base> operations.

\param id_old
Is the identifer for the tape that is recording AD<Base> operations
for this thread.

\par thread
Let \c thread denote the current thread number
\c thread_alloc::thread_num(). 
If \c _OPENMP is not defined, \c thread is zero.
It must hold that <tt>thread = id_old % CPPAD_MAX_NUM_THREADS</tt>.

\par tape_ptr
It is assumed that <tt>tape_ptr(thread) != CPPAD_NULL</tt>
when \c tape_delete is called; i.e., AD<Base> operations for this
thread are being recorded.
The destructore for the corresponding tape is called and
upon return <tt>tape_ptr(thread) == CPPAD_NULL</tt>.

\par tape_id
We use \c id to denote <tt>AD<Base>::tape_id(thread)</tt>.
Upon the call to \c tape_delete, <tt>*id == id_old</tt>.
Upon the return, 
<tt>*id > id_old</tt> and <tt>thread = *id % CPPAD_MAX_NUM_THREADS</tt>
*/
template <class Base>
void  AD<Base>::tape_delete(size_t id_old)
{
	size_t thread = id_old % CPPAD_MAX_NUM_THREADS;
	CPPAD_ASSERT_KNOWN(
		thread == thread_alloc::thread_num(),
		"AD tape recording must stop in same thread as it started in."
	);
	size_t        *id   = id_handle(thread);
	CPPAD_ASSERT_UNKNOWN( *id   == id_old     );

	// increase the id for this thread in a way such that 
	// thread = id % CPPAD_MAX_NUM_THREADS
	CPPAD_ASSERT_KNOWN(
	size_t(*id) + CPPAD_MAX_NUM_THREADS <= 
		std::numeric_limits<CPPAD_TAPE_ID_TYPE>::max(),
	"To many different tapes given the type used for CPPAD_TAPE_ID_TYPE"
	);
	*id  += CPPAD_MAX_NUM_THREADS;

	// delete the old tape for this thread
	tape_ptr(thread, tape_ptr_delete );

	return;
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
	size_t thread      = tape_id_ % CPPAD_MAX_NUM_THREADS;
	return tape_ptr(thread, tape_ptr_return_null_ok );
}

/* \} */
CPPAD_END_NAMESPACE
# endif
