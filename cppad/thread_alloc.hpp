/* $Id$ */
# ifndef CPPAD_THREAD_ALLOC_INCLUDED
# define CPPAD_THREAD_ALLOC_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <sstream>
# include <limits>
# include <memory>


# ifdef _MSC_VER
// Supress warning that Microsoft compiler changed its behavior and is now 
// doing the correct thing at the statement:
//			new(array + i) Type();
# pragma warning(disable:4345)
# endif

# include <cppad/local/cppad_assert.hpp>
# include <cppad/local/define.hpp>
CPPAD_BEGIN_NAMESPACE
/*!
\file thread_alloc.hpp
File used to define the CppAD multi-threading allocaor class
*/

/*!
\def CPPAD_MAX_NUM_CAPACITY
Maximum number of different capacities the allocator will attempt.
This must be larger than the log base two of numeric_limit<size_t>::max().
*/
# define CPPAD_MAX_NUM_CAPACITY 100

/*!
\def CPPAD_MIN_CAPACITY
Minimum number of bytes that will fit in a thread_alloc memory allocation.
*/
# define CPPAD_MIN_CAPACITY 128

/*!
\def CPPAD_MIN_CHUNK
Minimum number of bytes that will fit in a system memory allocation.
*/
# define CPPAD_MIN_CHUNK 4096

/*!
\def CPPAD_TRACE_CAPACITY
If NDEBUG is not defined, print all calls to \c get_memory and \c return_memory
that correspond to this capacity and thread CPPAD_TRACE_THEAD.
(Note that if CPPAD_TRACE_CAPACITY is zero, or any other value not in the list
of capacities, no tracing will be done.)
*/
# define CPPAD_TRACE_CAPACITY 0

/*!
\def CPPAD_TRACE_THREAD
If NDEBUG is not defined, print all calls to \c get_memory and \c return_memory
that correspond to this thead and capacity CPPAD_TRACE_CAPACITY.
*/
# define CPPAD_TRACE_THREAD 0

/*
Note that Section 3.6.2 of ISO/IEC 14882:1998(E) states: "The storage for 
objects with static storage duration (3.7.1) shall be zero-initialized
(8.5) before any other initialization takes place."
*/

/*!
Capacity vector for memory allocation block sizes.

Only one of these objects should be created and used as a
static variable inside of the \c thread_alloc::capacity_info function.
*/

class thread_alloc_capacity {
public:
	/// number of capacity values actually used
	size_t number;
	/// the different capacity values
	size_t value[CPPAD_MAX_NUM_CAPACITY];
	/// ctor
	thread_alloc_capacity(void)
	{
		// Cannot figure out how to call thread_alloc::in_parallel from here.
		// CPPAD_ASSERT_KNOWN( 
		// ! thread_alloc::in_parallel() ,
		// "thread_alloc: in parallel mode and parallel_setup not yet called."
		// );
		number           = 0;
		size_t capacity  = CPPAD_MIN_CAPACITY;
		while( capacity < std::numeric_limits<size_t>::max() / 2 )
		{	CPPAD_ASSERT_UNKNOWN( number < CPPAD_MAX_NUM_CAPACITY );
			value[number++] = capacity;
			// next capactiy is 3/2 times the current one
			capacity        = 3 * ( (capacity + 1) / 2 );
		} 		 
		CPPAD_ASSERT_UNKNOWN( number > 0 );
	}
};


/*!
Structure for a chunk of memory containing multiple blocks. 
*/
struct thread_alloc_chunk {
	/// pointer to the chunk with same tc_index_
	thread_alloc_chunk*  next_;
	/// number of thread_alloc units, in this chunk, and currently in use.
	size_t               n_inuse_;
};


/*!
Allocator class that works well with an multi-threading environment.
*/
class thread_alloc{
// ============================================================================
private:
/// index that uniquely identifies both thread and capacity
size_t              tc_index_;
/// pointer to the next block with same tc_index_
thread_alloc*       next_;
/// pointer to the chunk containing this block
thread_alloc_chunk* chunk_;
/// number of elements (used by create and delete array functions)
size_t              n_element_;
// ---------------------------------------------------------------------
/// make default constructor private. It is only used by the constructor
/// for \c root arrays below.
thread_alloc(void) : 
tc_index_(0), 
next_(CPPAD_NULL), 
n_element_(0)
{ }
// ---------------------------------------------------------------------
static const thread_alloc_capacity* capacity_info(void)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	static const thread_alloc_capacity capacity;
	return &capacity;
}
// ---------------------------------------------------------------------
/// number of bytes of memory that are currently in use for each thread
static size_t* inuse_vector(void)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	static size_t inuse[CPPAD_MAX_NUM_THREADS];
	return inuse;
}
// ---------------------------------------------------------------------
/// number of bytes that are currrently available for each thread; i.e.,
/// have been obtained for each thread and not yet returned to the system.
static size_t* available_vector(void)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	static size_t available[CPPAD_MAX_NUM_THREADS];
	return available;
}

// -----------------------------------------------------------------------
/*!
Increase the number of bytes of memory that are currently in use; i.e.,
that been obtained with \c get_memory and not yet returned. 

\param inc [in]
amount to increase memory in use.

\param thread [in]
Thread for which we are increasing the number of bytes in use
(must be less than \c num_threads).
Durring parallel execution, this must be the thread 
that is currently executing.
*/
static void inc_inuse(size_t inc, size_t thread)
{	
	CPPAD_ASSERT_UNKNOWN( thread < num_threads() );
	CPPAD_ASSERT_UNKNOWN( 
		thread == thread_num() || (! in_parallel()) 
	);
	size_t* inuse_vec = inuse_vector();
	
	// do the addition
	size_t result = inuse_vec[thread] + inc;
	CPPAD_ASSERT_UNKNOWN( result >= inuse_vec[thread] );

	inuse_vec[thread] = result;
}
// -----------------------------------------------------------------------
/*!
Increase the number of bytes of memory that are currently avaialble; i.e.,
have been obtained obtained from the system and are being held future use.

\copydetails inc_inuse
*/
static void inc_available(size_t inc, size_t thread)
{	
	CPPAD_ASSERT_UNKNOWN( thread < CPPAD_MAX_NUM_THREADS);
	CPPAD_ASSERT_UNKNOWN( 
		thread == thread_num() || (! in_parallel()) 
	);
	size_t* available_vec = available_vector();
	// do the addition
	size_t result = available_vec[thread] + inc;
	CPPAD_ASSERT_UNKNOWN( result >= available_vec[thread] );

	available_vec[thread] = result;
}
// -----------------------------------------------------------------------
/*!
Decrease the number of bytes of memory that are currently in use; i.e.,
that been obtained with \c get_memory and not yet returned. 

\param dec [in]
amount to decrease number of bytes in use.

\param thread [in]
Thread for which we are decreasing the number of bytes in use
(must be less than \c num_threads).
Durring parallel execution, this must be the thread 
that is currently executing.
*/
static void dec_inuse(size_t dec, size_t thread)
{	
	CPPAD_ASSERT_UNKNOWN( thread < num_threads() );
	CPPAD_ASSERT_UNKNOWN( 
		thread == thread_num() || (! in_parallel()) 
	);
	size_t* inuse_vec = inuse_vector();
	// do the subtraction
	CPPAD_ASSERT_UNKNOWN( inuse_vec[thread] >= dec );
	inuse_vec[thread] = inuse_vec[thread] - dec;
}
// -----------------------------------------------------------------------
/*!
Decrease the number of bytes of memory that are currently avaialble; i.e.,
have been obtained obtained from the system and are being held future use.

\copydetails dec_inuse
*/
static void dec_available(size_t dec, size_t thread)
{	
	CPPAD_ASSERT_UNKNOWN( thread < CPPAD_MAX_NUM_THREADS);
	CPPAD_ASSERT_UNKNOWN( 
		thread == thread_num() || (! in_parallel()) 
	);
	size_t* available_vec = available_vector();
	// do the subtraction
	CPPAD_ASSERT_UNKNOWN( available_vec[thread] >= dec );
	available_vec[thread] =  available_vec[thread] - dec;
}

// ----------------------------------------------------------------------
/// Vector of length CPPAD_MAX_NUM_THREADS times CPPAD_MAX_NUM_CAPACITIES 
/// for use as root of list of chunks.
static thread_alloc_chunk* root_chunk(void)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	static thread_alloc_chunk  
		root[CPPAD_MAX_NUM_THREADS * CPPAD_MAX_NUM_CAPACITY];
	return root;
}

// ----------------------------------------------------------------------
/// Vector of length CPPAD_MAX_NUM_THREADS times CPPAD_MAX_NUM_CAPACITIES 
/// for use as root of inuse lists.
static thread_alloc* root_inuse(void)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	static thread_alloc  
		root[CPPAD_MAX_NUM_THREADS * CPPAD_MAX_NUM_CAPACITY];
	return root;
}

// ----------------------------------------------------------------------
/// Vector of length CPPAD_MAX_NUM_THREADS times CPPAD_MAX_NUM_CAPACITIES 
/// for use as root of available lists.
static thread_alloc* root_available(void)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	static thread_alloc  
		root[CPPAD_MAX_NUM_THREADS * CPPAD_MAX_NUM_CAPACITY];
	return root;
}

// ----------------------------------------------------------------------
/// Vector of length CPPAD_MAX_NUM_THREADS times CPPAD_MAX_NUM_CAPACITIES 
/// for use as root of chunck lists (chuncks of memory allocated
/// for thread_alloc by system allocator).
static thread_alloc* root_chunck(void)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	static thread_alloc  
		root[CPPAD_MAX_NUM_THREADS * CPPAD_MAX_NUM_CAPACITY];
	return root;
}
/*!
Set and get the number of threads that are sharing memory.

\param number_new 
If \c number is zero, we are only retreiving the current maximum
number of threads. Otherwise, we are setting and retreiving
maximum number of threads.

\return
the number of threads that are sharing memory.
If \c number_new is non-zero, the return value is equal to 
\c number_new.
*/
static size_t set_get_num_threads(size_t number_new)
{	static size_t number_user = 1;

	CPPAD_ASSERT_UNKNOWN( number_new <= CPPAD_MAX_NUM_THREADS );
	CPPAD_ASSERT_UNKNOWN( ! in_parallel() || (number_new == 0) );

	// case where we are changing the number of threads
	if( number_new != 0 )
		number_user = number_new;

	return number_user;
}
/*!
Set and call the routine that determine if we are in parallel 
execution mode.

\return 
value retuned by most recent setting for \a parallel_new.
If \a set is true,
or the most recent setting is \c CPPAD_NULL (its initial value),
the return value is false.
Otherwise the function corresponding to the most recent setting
is called and its value returned by \c set_get_in_parallel.

\param parallel_new [in]
If \a set is false, \a parallel_new it is not used.
Otherwise, the current value of \c parallel_new becomes the
most recent setting for in_parallel.

\param set
If \a set is true, then \a parallel_new is becomes the most
recent setting for this \c set_get_in_parallel.
*/
static bool set_get_in_parallel(
	bool (*parallel_new)(void) ,
	bool set = false           )
{	static bool (*parallel_user)(void) = CPPAD_NULL;

	if( set )
	{	parallel_user = parallel_new;
		return false;
	}

	if( parallel_user == CPPAD_NULL )
		return false;

	return parallel_user();
}
/*!
Set and call the routine that determine the current thread number.

\return 
returns value for the most recent setting for \a thread_num_new.
If \a set is true,
or the most recent setting is \c CPPAD_NULL (its initial value),
the return value is zero.
Otherwise the routine corresponding to the most recent setting
is called and its value returned by \c set_get_thread_num.

\param thread_num_new [in]
If \a set is false, \a thread_num_new it is not used.
Otherwise, the current value of \c thread_num_new becomes the
most recent setting for thread_num.

\param set
If \a set is true, then \a thread_num_new is becomes the most
recent setting for this \c set_get_thread_num.
*/
static size_t set_get_thread_num(
	size_t (*thread_num_new)(void)  ,
	bool set = false                )
{	static size_t (*thread_num_user)(void) = CPPAD_NULL;

	if( set )
	{	thread_num_user = thread_num_new;
		return 0;
	}

	if( thread_num_user == CPPAD_NULL )
		return 0;

	size_t thread = thread_num_user();
	CPPAD_ASSERT_KNOWN(
		thread < set_get_num_threads(0) ,
		"parallel_setup: thread_num() >= num_threads"
	);
	return thread;
}
// ============================================================================
public:
/*
$begin ta_parallel_setup$$
$spell
	alloc
	num
	bool
$$
$section Setup thread_alloc For Use in Multi-Threading Environment$$

$index setup, thread_alloc$$
$index thread_alloc, setup$$
$index parallel, setup$$
$index setup, parallel$$

$index num_threads$$
$index in_parallel$$
$index thread_num$$

$index multi-threading, initialize$$
$index initialize, multi-threading$$

$head Syntax$$
$codei%thread_alloc::parallel_setup(%num_threads%, %in_parallel%, %thread_num%)
%$$

$head Purpose$$
By default there is only one thread and all execution is in sequential mode,
i.e., multiple threads are not sharing the same memory; i.e.
not in parallel mode.

$head num_threads$$
This argument has prototype
$codei%
	size_t %num_threads%
%$$ 
and must be greater than zero.
It specifies the number of threads that are sharing memory.
The case $icode%num_threads% == 1%$$ is a special case that is 
used to terminate a multi-threading environment.

$head in_parallel$$
This function has prototype
$codei%
	bool %in_parallel%(void) 
%$$
It must return $code true$$ if there is more than one thread
currently executing.
Otherwise it can return false.
$pre

$$
In the special case where $icode%num_threads% == 1%$$,
the routine $icode in_parallel$$ is not used.

$head thread_num$$
This function has prototype
$codei%
	size_t %thread_num%(void) 
%$$
It must return a thread number that uniquely identifies the
currently executing thread. 
Furthermore
$code%
	0 <= %thread_num%() < %num_threads%
%$$.
In the special case where $icode%num_threads% == 1%$$,
the routine $icode thread_num$$ is not used.
$pre

$$
Note that this function is called by other routines so,
as soon as a new thread is executing,
one must be certain that $icode thread_num()$$ will
work for that thread.

$head Restrictions$$
The function $code parallel_setup$$ must be called before 
the program enters $cref/parallel/ta_in_parallel/$$ execution mode.
In addition, this function cannot be called while in parallel mode.

$head Example$$
The files 
$cref team_openmp.cpp$$, 
$cref team_bthread.cpp$$, and
$cref team_pthread.cpp$$, 
contain examples and tests that use this function.   

$end
*/
/*!
Set thread_alloc up for parallel mode usage.

\param num_threads [in]
Is the number of thread that may be executing at the same time.

\param in_parallel [in]
Is the routine that determines if we are in parallel mode or not.

\param thread_num [in]
Is the routine that determines the current thread number
(between zero and num_threads minus one).
*/

static void parallel_setup(
	size_t num_threads         ,
	bool (*in_parallel)(void)  ,
	size_t (*thread_num)(void) )
{

	// go back to single thread mode right away
	// (previous settings may no longer be valid)
	if( num_threads == 1 )
	{	bool set = true;
		set_get_num_threads(num_threads);
		set_get_in_parallel(CPPAD_NULL, set);
		set_get_thread_num(CPPAD_NULL, set);
		return;
	}

	CPPAD_ASSERT_KNOWN( 
		num_threads <= CPPAD_MAX_NUM_THREADS ,
		"parallel_setup: num_threads is too large"
	);
	CPPAD_ASSERT_KNOWN( 
		num_threads != 0 ,
		"parallel_setup: num_threads == zero"
	);
	CPPAD_ASSERT_KNOWN( 
		in_parallel != CPPAD_NULL ,
		"parallel_setup: the function pointer in_parallel == zero"
	);
	CPPAD_ASSERT_KNOWN( 
		thread_num != CPPAD_NULL ,
		"parallel_setup: the function pointer thread_num == zero"
	);

	// Make sure that constructors for all static variables in this file 
	// are called in sequential mode.	
	capacity_info();
	inuse_vector();
	available_vector();
	root_inuse();
	root_available();
	size_t cap_bytes;
	void* v_ptr = get_memory(0, cap_bytes);

	// free memory allocated by call to get_memory above
	return_memory(v_ptr);
	free_available( set_get_thread_num(CPPAD_NULL) );

	// delay this so thread_num() call above is in previous mode
	// (current setings may not yet be valid)
	if( num_threads > 1 )
	{	bool set = true;
		set_get_num_threads(num_threads);
		set_get_in_parallel(in_parallel, set);
		set_get_thread_num(thread_num, set);
	}
}
/*
$begin ta_num_threads$$
$spell
	inv
	CppAD
	num
	alloc
$$
$section Get Number of Threads$$

$index num_threads, thread_alloc$$
$index thread_alloc, num_threads$$
$index threads, number of$$

$head Syntax$$
$icode%number% = thread_alloc::num_threads()%$$

$head Purpose$$
Determine the number of threads as set during $cref/parallel_setup/ta_parallel_setup/$$.

$head number$$
The return value $icode number$$ has prototype
$icode%
	size_t %number%
%$$ 
and is equal to the value of 
$cref/num_threads/ta_parallel_setup/num_threads/$$
in the previous call to $icode parallel_setup$$.
If there was no such previous call, the value one is returned.

$head Example$$
The example and test $cref thread_alloc.cpp$$ uses this routine.

$end
*/
/*!
Get the current number of threads that thread_alloc can use.
*/
static size_t num_threads(void)
{	return set_get_num_threads(CPPAD_NULL); }
/* -----------------------------------------------------------------------
$begin ta_in_parallel$$

$section Is The Current Execution in Parallel Mode$$
$spell
	thread_alloc
	bool
$$

$index in_parallel, thread_alloc$$
$index thread_alloc, in_parallel$$
$index parallel, execution$$
$index execution, parallel$$
$index sequential, execution$$

$head Syntax$$
$icode%flag% = thread_alloc::in_parallel()%$$

$head Purpose$$
Some of the $cref/thread_alloc/$$ allocation routines have different
specifications for parallel (not sequential) execution mode.
This routine enables you to determine if the current execution mode
is sequential or parallel.

$head flag$$
The return value has prototype
$codei%
	bool %flag%
%$$
It is true if the current execution is in parallel mode 
(possibly multi-threaded) and false otherwise (sequential mode).

$head Example$$
$cref/thread_alloc.cpp/$$

$end
*/
/// Are we in a parallel execution state; i.e., is it possible that
/// other threads are currently executing. 
static bool in_parallel(void)
{	return set_get_in_parallel(CPPAD_NULL); }
/* -----------------------------------------------------------------------
$begin ta_thread_num$$
$spell
	CppAD
	num
	thread_alloc
	cppad.hpp
$$

$section Get the Current Thread Number$$

$index thread_num, thread_alloc$$
$index thread_alloc, thread_num$$
$index thread, current$$
$index current, thread$$

$head Syntax$$
$icode%thread% = thread_alloc::thread_num()%$$

$head Purpose$$
Some of the $cref/thread_alloc/$$ allocation routines have a thread number.
This routine enables you to determine the current thread.

$head thread$$
The return value $icode thread$$ has prototype
$codei%
	size_t %thread%
%$$
and is the currently executing thread number.
If $code _OPENMP$$ is not defined, $icode thread$$ is zero.

$head Example$$
$cref/thread_alloc.cpp/$$

$end
*/
/// Get current thread number 
static size_t thread_num(void)
{	return set_get_thread_num(CPPAD_NULL); }
/* -----------------------------------------------------------------------
$begin ta_get_memory$$
$spell
	num
	ptr
	thread_alloc
$$

$section Get At Least A Specified Amount of Memory$$

$index thread_num, thread_alloc$$
$index thread_alloc, thread_num$$
$index memory, allocate$$
$index allocate, memory$$

$head Syntax$$
$icode%v_ptr% = thread_alloc::get_memory(%min_bytes%, %cap_bytes%)%$$

$head Purpose$$
Use $cref/thread_alloc/$$ to obtain a minimum number of bytes of memory
(for use by the $cref/current thread/ta_thread_num/$$).

$head min_bytes$$
This argument has prototype
$codei%
	size_t %min_bytes%
%$$
It specifies the minimum number of bytes to allocate.

$head cap_bytes$$
This argument has prototype
$codei%
	size_t& %cap_bytes%
%$$
It's input value does not matter.
Upon return, it is the actual number of bytes (capacity) 
that have been allocated for use,
$codei%
	%min_bytes% <= %cap_bytes%
%$$

$head v_ptr$$
The return value $icode v_ptr$$ has prototype
$codei%
	void* %v_ptr%
%$$
It is the location where the $icode cap_bytes$$ of memory 
that have been allocated for use begins.

$head Allocation Speed$$
This allocation should be faster if the following conditions hold:
$list number$$
The memory allocated by a previous call to $code get_memory$$ 
is currently available for use.
$lnext
The current $icode min_bytes$$ is between 
the previous $icode min_bytes$$ and previous $icode cap_bytes$$.
$lend

$head Example$$
$cref/thread_alloc.cpp/$$

$end
*/
/*!
 	Use thread_alloc to get a specified amount of memory.

If the memory allocated by a previous call to \c get_memory is now 
avaialable, and \c min_bytes is between its previous value
and the previous \c cap_bytes, this memory allocation will have
optimal speed. Otherwise, the memory allocation is more complicated and
may have to wait for other threads to complete an allocation.

\param min_bytes [in]
The minimum number of bytes of memory to be obtained for use.

\param cap_bytes [out]
The actual number of bytes of memory obtained for use.

\return
pointer to the beginning of the memory allocted for use.
 	*/
static void* get_memory(size_t min_bytes, size_t& cap_bytes)
{	// see first_trace below	
	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

	size_t num_cap = capacity_info()->number;
	using std::cout;
	using std::endl;

	// determine the capacity for this request
	size_t c_index   = 0;
	const size_t* capacity_vec = capacity_info()->value;
	while( capacity_vec[c_index] < min_bytes )
	{	++c_index;	
		CPPAD_ASSERT_UNKNOWN(c_index < num_cap );
	}
	cap_bytes = capacity_vec[c_index];

	// determine the thread and capacity index
	size_t thread    = thread_num();
	size_t tc_index  = thread * num_cap + c_index;

# ifndef NDEBUG
	// trace allocation
	static bool first_trace = true;
	if(	cap_bytes == CPPAD_TRACE_CAPACITY && 
	     thread    ==  CPPAD_TRACE_THREAD  && first_trace )
	{	cout << endl;	
		cout << "thread_alloc: Trace for Thread = " << thread;
		cout << " and capacity = " << cap_bytes << endl;
		if( first_trace )
			first_trace = false;
	}

	// Root blocks for both lists. Note these are different for different 
	// threads because tc_index is different for different threads.
	thread_alloc* inuse_root     = root_inuse() + tc_index;
# endif
	thread_alloc* available_root = root_available() + tc_index;

	// check if any memory is avialble for allocation
	thread_alloc* block;
	if( available_root->next_ == CPPAD_NULL )
	{
		if( num_threads() == 1 )
		{	// create one block of memory
			void* v_block = ::operator new( 
				sizeof(thread_alloc) + cap_bytes 
			);
			block              = reinterpret_cast<thread_alloc*>(v_block);
			block->chunk_      = CPPAD_NULL; // no corresponding chunk
			block->tc_index_   = tc_index;
			block->next_       = available_root->next_;
			available_root->next_ = block;
			inc_available(cap_bytes, thread);
		}
		else
		{	size_t n_block = 1 + (CPPAD_MIN_CHUNK - 1)/cap_bytes; 
			// Create a new chunk of contigious memory with multiple blocks.
			void* v_chunk  = ::operator new( sizeof(thread_alloc_chunk)
				+ n_block * ( sizeof(thread_alloc) + cap_bytes)
			);
# ifndef NDEBUG
			// trace allocation
			if(	cap_bytes == CPPAD_TRACE_CAPACITY && 
	     		thread    ==  CPPAD_TRACE_THREAD   )
			{	cout << "get_memory:    v_chunk = " << v_chunk << endl; } 
# endif
			thread_alloc_chunk* chunk_root = root_chunk() + tc_index;
			thread_alloc_chunk* chunk      = 
				reinterpret_cast<thread_alloc_chunk*>(v_chunk);
			//
			chunk->n_inuse_    = 0;
			chunk->next_       = chunk_root->next_;
			chunk_root->next_  = chunk;
			//
			CPPAD_ASSERT_UNKNOWN( sizeof(char) == 1 );
			char* c_chunk = reinterpret_cast<char*>(v_chunk);
			c_chunk      += sizeof(thread_alloc_chunk);
			size_t i_block;
			for(i_block = 0; i_block < n_block; i_block++)
			{	block = reinterpret_cast<thread_alloc*>(c_chunk);
				block->chunk_          = chunk;
				block->tc_index_       = tc_index;
				block->next_           = available_root->next_;
				available_root->next_ = block;
				inc_available(cap_bytes, thread);
				//
				c_chunk += sizeof(thread_alloc) + cap_bytes;
			}
		}
	}
	block = available_root->next_;
	CPPAD_ASSERT_UNKNOWN( block != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( block->tc_index_ == tc_index );

	// remove block from available list
	available_root->next_ = block->next_;

	// return value for get_memory
	void* v_ptr = reinterpret_cast<void*>(block + 1);
# ifndef NDEBUG
	// add block to inuse list
	block->next_          = inuse_root->next_;
	inuse_root->next_     = block;

	// trace allocation
	if(	cap_bytes == CPPAD_TRACE_CAPACITY && 
	     thread    ==  CPPAD_TRACE_THREAD   )
	{	cout << "get_memory:    v_ptr = " << v_ptr;
		cout << ", num_threads = " << num_threads() << endl; 
	} 
# endif

	// adjust counts
	inc_inuse(cap_bytes, thread);
	dec_available(cap_bytes, thread);

	// add 1 thread_alloc unit to corresponding chunk inuse counter
	thread_alloc_chunk* chunk = block->chunk_;
	if( chunk != CPPAD_NULL )
		(chunk->n_inuse_)++;

	// return pointer to memory, do not inclue thread_alloc information
	return v_ptr;
}
/* -----------------------------------------------------------------------
$begin ta_return_memory$$
$spell
	num
	ptr
	thread_alloc
$$

$section Return Memory to thread_alloc$$

$index return_memory, thread_alloc$$
$index thread_alloc, return_memory$$
$index memory, available$$
$index available, memory$$
$index thread, available memory$$

$head Syntax$$
$codei%thread_alloc::return_memory(%v_ptr%)%$$

$head Purpose$$
If $cref/num_threads/ta_num_threads/$$ is one,
the memory is returned to the system.
Otherwise, the memory is retained by $cref thread_alloc$$ for quick future use
by the thread that allocated to memory.

$head v_ptr$$
This argument has prototype
$codei%
	void* %v_ptr%
%$$.
It must be a pointer to memory that is currently in use; i.e.
obtained by a previous call to $cref/get_memory/ta_get_memory/$$ and not yet returned.

$head Thread$$
Either the $cref/current thread/ta_thread_num/$$ must be the same as during
the corresponding call to $cref/get_memory/ta_get_memory/$$,
or the current execution mode must be sequential 
(not $cref/parallel/ta_in_parallel/$$).

$head NDEBUG$$
If $code NDEBUG$$ is defined, $icode v_ptr$$ is not checked (this is faster).
Otherwise, a list of in use pointers is searched to make sure
that $icode v_ptr$$ is in the list. 

$head Example$$
$cref/thread_alloc.cpp/$$

$end
*/
/*!
 	Return memory that was obtained by \c get_memory.
If  <code>num_threads() == 1</code>,
the memory is returned to the system.
Otherwise, it is retained by \c thread_alloc and available for use by 
\c get_memory for this thread.

\param v_ptr [in]
Value of the pointer returned by \c get_memory and still in use.
After this call, this pointer will available (and not in use).

\par
We must either be in sequential (not parallel) execution mode,
or the current thread must be the same as for the corresponding call
to \c get_memory.
 	*/
static void return_memory(void* v_ptr)
{	size_t num_cap   = capacity_info()->number;

	thread_alloc* block  = reinterpret_cast<thread_alloc*>(v_ptr) - 1;
	size_t tc_index      = block->tc_index_;
	size_t thread        = tc_index / num_cap;
	size_t c_index       = tc_index % num_cap;
	size_t capacity      = capacity_info()->value[c_index];

	CPPAD_ASSERT_UNKNOWN( thread < CPPAD_MAX_NUM_THREADS );
	CPPAD_ASSERT_KNOWN( 
		thread == thread_num() || (! in_parallel()),
		"Attempt to return memory for a different thread "
		"while in parallel mode"
	);

# ifndef NDEBUG
	// remove block from inuse list
	thread_alloc* inuse_root     = root_inuse() + tc_index;
	thread_alloc* previous       = inuse_root;
	while((previous->next_ != CPPAD_NULL) & (previous->next_ != block))
		previous = previous->next_;	

	// check that v_ptr is valid
	if( previous->next_ != block )
	{	using std::endl;
		std::ostringstream oss;
		oss << "return_memory: attempt to return memory not in use";
		oss << endl;
		oss << "v_ptr    = " << v_ptr    << endl;   
		oss << "thread   = " << thread   << endl;   
		oss << "capacity = " << capacity << endl;   
		oss << "See CPPAD_TRACE_THREAD & CPPAD_TRACE_CAPACITY in";
		oss << endl << "# include <cppad/thread_alloc.hpp>" << endl;
		CPPAD_ASSERT_KNOWN(false, oss.str().c_str()	); 
	}

	// trace option
	if( capacity==CPPAD_TRACE_CAPACITY && thread==CPPAD_TRACE_THREAD )
	{	std::cout << "return_memory: v_ptr = " << v_ptr;
		std::cout << ", num_threads = " << num_threads() << std::endl; 
	}

	// remove v_ptr from inuse list
	previous->next_  = block->next_;
# endif
	// capacity bytes are removed from the inuse pool
	dec_inuse(capacity, thread);

	// check for case where we just return the memory to the system
	thread_alloc_chunk* chunk = block->chunk_;
	if( (num_threads() == 1) & (chunk == CPPAD_NULL) )
	{	::operator delete( reinterpret_cast<void*>(block) );
		return;
	}

	// remove 1 thread_alloc unit from corresponding chunk inuse counter
	if( chunk != CPPAD_NULL )
	{	CPPAD_ASSERT_UNKNOWN(chunk->n_inuse_ > 0 );
		(chunk->n_inuse_)--;
	}

	// add this block to available list for this thread and capacity
	thread_alloc* available_root = root_available() + tc_index;
	block->next_                 = available_root->next_;
	available_root->next_        = block;

	// capacity bytes are added to the available pool
	inc_available(capacity, thread);
}
/* -----------------------------------------------------------------------
$begin ta_free_available$$
$spell
	num
	thread_alloc
$$

$section Free Memory Currently Available for Quick Use by a Thread$$

$index free_available, thread_alloc$$
$index thread_alloc, free_available$$
$index free, available$$
$index available, free$$
$index thread, free memory$$

$head Syntax$$
$codei%thread_alloc::free_available(%thread%)%$$

$head Purpose$$
Free memory, currently available for quick use by a specific thread, 
for general future use.

$head thread$$
This argument has prototype
$codei%
	size_t %thread%
%$$
Either $cref/thread_num/ta_thread_num/$$ must be the same as $icode thread$$,
or the current execution mode must be sequential 
(not $cref/parallel/ta_in_parallel/$$).

$head Example$$
$cref/thread_alloc.cpp/$$

$end
*/
/*!
Return all the memory being held as available for a thread to the system.

\param thread [in]
this thread that will no longer have any available memory after this call.
This must either be the thread currently executing, or we must be 
in sequential (not parallel) execution mode.
*/
static void free_available(size_t thread)
{	CPPAD_ASSERT_KNOWN(
		thread < CPPAD_MAX_NUM_THREADS,
		"Attempt to free memory for a thread >= CPPAD_MAX_NUM_THREADS"
	);
	CPPAD_ASSERT_KNOWN( 
		thread == thread_num() || (! in_parallel()),
		"Attempt to free memory for a different thread "
		"while in parallel mode"
	);

	size_t num_cap = capacity_info()->number;
	if( num_cap == 0 )
		return;
	const size_t*     capacity_vec  = capacity_info()->value;
	size_t c_index, tc_index;
	for(c_index = 0; c_index < num_cap; c_index++)
	{	size_t capacity = capacity_vec[c_index];
		tc_index           = thread * num_cap + c_index;

		// remove entire available linked list
		thread_alloc* root  = root_available() + tc_index;
		thread_alloc* block = root->next_;
		while( block != CPPAD_NULL )
		{	thread_alloc* next  = block->next_;
			if( block->chunk_ == CPPAD_NULL )
				::operator delete( reinterpret_cast<void*>(block) );
			block  = next;
			dec_available(capacity, thread);
		}
		root->next_ = CPPAD_NULL;

		// delete all chunks that are no longer in use
		thread_alloc_chunk* previous = root_chunk() + tc_index;
		thread_alloc_chunk* chunk    = previous->next_;
		while( chunk != CPPAD_NULL )
		{	thread_alloc_chunk* next = chunk->next_;
			if( chunk->n_inuse_ == 0 )
			{	// return this chunk to system memory
# ifndef NDEBUG
				// trace allocation
				void* v_chunk = reinterpret_cast<void*>(chunk);
				if(	capacity  == CPPAD_TRACE_CAPACITY && 
	     			thread    ==  CPPAD_TRACE_THREAD   )
				{	std::cout << "free_available:    v_chunk = ";
					std::cout << v_chunk << std::endl; 
				} 
# endif
				previous->next_ = next;
				::operator delete( reinterpret_cast<void*>(chunk) );
			}
			chunk = next;
		}
	}
	CPPAD_ASSERT_UNKNOWN( available(thread) == 0 );
}
/* -----------------------------------------------------------------------
$begin ta_inuse$$
$spell
	num
	inuse
	thread_alloc
$$

$section Amount of Memory a Thread is Currently Using$$

$index inuse, thread_alloc$$
$index thread_alloc, inuse$$
$index use, memory$$
$index thread, memory inuse$$

$head Syntax$$
$icode%num_bytes% = thread_alloc::inuse(%thread%)%$$

$head Purpose$$
Memory being managed by $cref/thread_alloc/$$ has two states,
currently in use by the specified thread,
and quickly available for future use by the specified thread.
This function informs the program how much memory is in use.

$head thread$$
This argument has prototype
$codei%
	size_t %thread%
%$$
Either $cref/thread_num/ta_thread_num/$$ must be the same as $icode thread$$,
or the current execution mode must be sequential 
(not $cref/parallel/ta_in_parallel/$$).

$head num_bytes$$
The return value has prototype
$codei%
	size_t %num_bytes%
%$$
It is the number of bytes currently in use by the specified thread.

$head Example$$
$cref/thread_alloc.cpp/$$

$end
*/
/*!
Determine the amount of memory that is currently inuse.

\param thread [in]
Thread for which we are determining the amount of memory
(must be < CPPAD_MAX_NUM_THREADS).
Durring parallel execution, this must be the thread 
that is currently executing.

\return
The amount of memory in bytes.
*/
static size_t inuse(size_t thread)
{ 
	CPPAD_ASSERT_UNKNOWN( thread < CPPAD_MAX_NUM_THREADS);
	CPPAD_ASSERT_UNKNOWN( 
		thread == thread_num() || (! in_parallel()) 
	);
	return inuse_vector()[thread];
}
/* -----------------------------------------------------------------------
$begin ta_available$$
$spell
	num
	thread_alloc
$$

$section Amount of Memory Available for Quick Use by a Thread$$

$index available, thread_alloc$$
$index thread_alloc, available$$
$index memory, available$$
$index thread, available memory$$

$head Syntax$$
$icode%num_bytes% = thread_alloc::available(%thread%)%$$

$head Purpose$$
Memory being managed by $cref/thread_alloc/$$ has two states,
currently in use by the specified thread,
and quickly available for future use by the specified thread.
This function informs the program how much memory is available.

$head thread$$
This argument has prototype
$codei%
	size_t %thread%
%$$
Either $cref/thread_num/ta_thread_num/$$ must be the same as $icode thread$$,
or the current execution mode must be sequential 
(not $cref/parallel/ta_in_parallel/$$).

$head num_bytes$$
The return value has prototype
$codei%
	size_t %num_bytes%
%$$
It is the number of bytes currently available for use by the specified thread.
One way that bytes become available is after a call to
$cref/return_memory/ta_return_memory/$$.
There is another way that bytes become available:
for small memory allocations multiple memory units with the same 
$cref/cap_bytes/ta_get_memory/cap_bytes/$$ are reserved for the current 
$icode thread$$.

$head Example$$
$cref/thread_alloc.cpp/$$

$end
*/
/*!
Determine the amount of memory that is currently available for use.

\copydetails inuse
*/
static size_t available(size_t thread)
{
	CPPAD_ASSERT_UNKNOWN( thread < CPPAD_MAX_NUM_THREADS);
	CPPAD_ASSERT_UNKNOWN( 
		thread == thread_num() || (! in_parallel()) 
	);
	return available_vector()[thread];
}
/* -----------------------------------------------------------------------
$begin ta_create_array$$
$spell
	inuse
	thread_alloc
	sizeof
$$

$section Allocate An Array and Call Default Constructor for its Elements$$

$index create_array, thread_alloc$$
$index thread_alloc, create_array$$
$index array, allocate$$
$index allocate, array$$

$head Syntax$$
$icode%array% = thread_alloc::create_array<%Type%>(%size_min%, %size_out%)%$$.

$head Purpose$$
Create a new raw array using $cref/thread_alloc/$$ memory allocator 
(works well in a multi-threading environment)
and call default constructor for each element.

$head Type$$
The type of the elements of the array.

$head size_min$$
This argument has prototype
$codei%
	size_t %size_min%
%$$
This is the minimum number of elements that there can be
in the resulting $icode array$$.

$head size_out$$
This argument has prototype
$codei%
	size_t& %size_out%
%$$
The input value of this argument does not matter.
Upon return, it is the actual number of elements 
in $icode array$$ 
($icode% size_min %<=% size_out%$$).

$head array$$
The return value $icode array$$ has prototype
$codei%
	%Type%* %array%
%$$
It is array with $icode size_out$$ elements.
The default constructor for $icode Type$$ is used to initialize the 
elements of $icode array$$.
Note that $cref/delete_array/ta_delete_array/$$
should be used to destroy the array when it is no longer needed.

$head Delta$$
The amount of memory $cref/inuse/ta_inuse/$$ by the current thread, 
will increase $icode delta$$ where
$codei%
	sizeof(%Type%) * (%size_out% + 1) > %delta% >= sizeof(%Type%) * %size_out%
%$$
The $cref/available/ta_available/$$ memory will decrease by $icode delta$$,
(and the allocation will be faster)
if a previous allocation with $icode size_min$$ between its current value
and $icode size_out$$ is available. 

$head Example$$
$cref/thread_alloc.cpp/$$

$end 
*/
/*!
Use thread_alloc to allocate an array, then call default construtor
for each element.

\tparam Type
The type of the elements of the array.

\param size_min [in]
The minimum number of elements in the array.

\param size_out [out]
The actual number of elements in the array.

\return
pointer to the first element of the array.
The default constructor is used to initialize 
all the elements of the array.

\par
The \c n_element field, in the \c thread_alloc block before the return value,
is set to size_out.
*/
template <class Type>
static Type* create_array(size_t size_min, size_t& size_out)
{	// minimum number of bytes to allocate
	size_t min_bytes = size_min * sizeof(Type);
	// do the allocation 
	size_t num_bytes;
	void*  v_ptr          = get_memory(min_bytes, num_bytes);
	// This is where the array starts
	Type*  array          = reinterpret_cast<Type*>(v_ptr);
	// number of Type values in the allocation
	size_out              = num_bytes / sizeof(Type);
	// store this number in the size field
	thread_alloc* block   = reinterpret_cast<thread_alloc*>(v_ptr) - 1;
	block->n_element_     = size_out;

	// call default constructor for each element
	size_t i;
	for(i = 0; i < size_out; i++)
		new(array + i) Type();

	return array;
}
/* -----------------------------------------------------------------------
$begin ta_delete_array$$
$spell
	inuse
	thread_alloc
	sizeof
	deallocate
$$

$section Deallocate An Array and Call Destructor for its Elements$$

$index delete_array, thread_alloc$$
$index thread_alloc, delete_array$$
$index array, allocate$$
$index allocate, array$$

$head Syntax$$
$codei%thread_alloc::delete_array(%array%)%$$.

$head Purpose$$
Returns memory corresponding to an array created by 
(create by $cref/create_array/ta_create_array/$$) to the 
$cref/available/ta_available/$$ memory pool for the current thread.

$head Type$$
The type of the elements of the array.

$head array$$
The argument $icode array$$ has prototype
$codei%
	%Type%* %array%
%$$
It is a value returned by $cref/create_array/ta_create_array/$$ and not yet deleted.
The $icode Type$$ destructor is called for each element in the array.

$head Thread$$
The $cref/current thread/ta_thread_num/$$ must be the
same as when $cref/create_array/ta_create_array/$$ returned the value $icode array$$.
There is an exception to this rule:
when the current execution mode is sequential
(not $cref/parallel/ta_in_parallel/$$) the current thread number does not matter.

$head Delta$$
The amount of memory $cref/inuse/ta_inuse/$$ will decrease by $icode delta$$,
and the $cref/available/ta_available/$$ memory will increase by $icode delta$$,
where $cref/delta/ta_create_array/Delta/$$ 
is the same as for the corresponding call to $code create_array$$.

$head Example$$
$cref/thread_alloc.cpp/$$

$end 
*/
/*!
Return Memory Used for an Array to the Available Pool
(include destructor call for each element).

\tparam Type
The type of the elements of the array.

\param array [in]
A value returned by \c create_array that has not yet been deleted.
The \c Type destructor is used to destroy each of the elements 
of the array.

\par
Durring parallel execution, the current thread must be the same
as during the corresponding call to \c create_array.
*/
template <class Type>
static void delete_array(Type* array)
{	// determine the number of values in the array
	thread_alloc* block = reinterpret_cast<thread_alloc*>(array) - 1;
	size_t size         = block->n_element_;

	// call destructor for each element
	size_t i;
	for(i = 0; i < size; i++)
		(array + i)->~Type();

	// return the memory to the available pool for this thread
	thread_alloc::return_memory( reinterpret_cast<void*>(array) );
}

}; // end class thread_alloc


CPPAD_END_NAMESPACE

// preprocessor symbols local to this file
# undef CPPAD_MAX_NUM_CAPACITY
# undef CPPAD_TRACE_CAPACITY
# undef CPPAD_TRACE_THREAD
# endif
