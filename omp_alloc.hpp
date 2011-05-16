/* $Id$ */
# ifndef CPPAD_OMP_ALLOC_INCLUDED
# define CPPAD_OMP_ALLOC_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin omp_alloc$$
$spell
	openmp
$$

$section A Memory Allocator that works well with OpenMP multi-threading$$

$index multi-threading, memory allocation$$
$index openmp, memory allocation$$
$index memory, multi-threading allocation$$
$index allocation, multi-threading$$ 

$head Under Construction$$

$children%
	example/omp_alloc.cpp
%$$
$head Example$$
The file $cref/omp_alloc.cpp/$$ contains an example and test of this
memory allocator.
It returns true, if it succeeds, and false otherwise.

$end
*/

# include <limits>
# include <memory>
# include <omp.h>

# include <cppad/local/cppad_assert.hpp>
# include <cppad/local/define.hpp>
CPPAD_BEGIN_NAMESPACE
/*!
\file omp_alloc.hpp
File used to defin the CppAD allocator class
*/

/*!
\def CPPAD_MAX_NUM_CAPACITY
Maximum number of different capacities the allocator will attempt.
This must be larger than the log base two of numeric_limit<size_t>::max().
*/
# define CPPAD_MAX_NUM_CAPACITY 100

/*!
\def CPPAD_MIN_CAPACITY
Minimum capacity that will be allocated (must be a positive integer).
*/
# define CPPAD_MIN_CAPACITY 100

/*
Note that Section 3.6.2 of ISO/IEC 14882:1998(E) states: "The storage for 
objects with static storage duration (3.7.1) shall be zero-initialized
(8.5) before any other initialization takes place."
*/

/*
Capacity vector for memory allocation block sizes.
*/

class omp_alloc_capacity {
public:
	size_t number;
	size_t value[CPPAD_MAX_NUM_CAPACITY];
	omp_alloc_capacity(void)
	{
# ifdef _OPENMP
		CPPAD_ASSERT_UNKNOWN( ! omp_in_parallel() );
# endif

		number           = 0;
		size_t capacity  = sizeof(CPPAD_MIN_CAPACITY);
		while( capacity < std::numeric_limits<size_t>::max() / 2 )
		{	assert( number < CPPAD_MAX_NUM_CAPACITY );
			value[number++] = capacity;
			capacity       *= 2;
		} 		 
		CPPAD_ASSERT_UNKNOWN( number > 0 );
	}
};

/*!
Allocator class that works well with an OpenMP multi-threading environment.
*/
class omp_alloc{
// ============================================================================
private:
	/// index in the root_list correspondinig to a memory allocation
	size_t             index_;
	/// pointer to the next memory allocation with the same index
	void*              next_;
	// ---------------------------------------------------------------------
	/// construct information attached to each allocated memory block
	omp_alloc(void)
	: index_(0), next_(0)
	{ }
	// ---------------------------------------------------------------------
	static const omp_alloc_capacity* capacity_info(void)
	{	static omp_alloc_capacity capacity;
		return &capacity;
	}
	// ---------------------------------------------------------------------
	/// number of bytes of memory that are currently in use for each thread
	static size_t* inuse_vector(void)
	{	static size_t inuse[CPPAD_MAX_NUM_THREADS];
		return inuse;
	}
	// ---------------------------------------------------------------------
	/// number of bytes that are currrently available for each thread; i.e.,
	/// have been obtained for each thread and not yet returned to the system.
	static size_t* available_vector(void)
	{	static size_t available[CPPAD_MAX_NUM_THREADS];
		return available;
	}

	// -----------------------------------------------------------------------
	/*!
 	Increase the number of bytes of memory that are currently inuse; i.e.,
	that been obtained with \c get_memory and not yet returned. 

	\param inc
	amount to increase inuse.

	\param thread
	Thread for which we are increasing the number of bytes inuse
	(must be < CPPAD_MAX_NUM_THREADS).
	Durring parallel execution, this must be the thread 
	that is currently executing.
	*/
	static void inc_inuse(size_t inc, size_t thread)
	{	
		CPPAD_ASSERT_UNKNOWN( thread < CPPAD_MAX_NUM_THREADS);
		CPPAD_ASSERT_UNKNOWN( 
			thread == get_thread_num() || (! in_parallel()) 
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
			thread == get_thread_num() || (! in_parallel()) 
		);
		size_t* available_vec = available_vector();
		// do the addition
		size_t result = available_vec[thread] + inc;
		CPPAD_ASSERT_UNKNOWN( result >= available_vec[thread] );

		available_vec[thread] = result;
	}
	// -----------------------------------------------------------------------
	/*!
 	Decrease the number of bytes of memory that are currently inuse; i.e.,
	that been obtained with \c get_memory and not yet returned. 

	\param dec
	amount to decrease inuse.

	\param thread
	Thread for which we are decreasing the number of bytes inuse
	(must be < CPPAD_MAX_NUM_THREADS).
	Durring parallel execution, this must be the thread 
	that is currently executing.
	*/
	static void dec_inuse(size_t dec, size_t thread)
	{	
		CPPAD_ASSERT_UNKNOWN( thread < CPPAD_MAX_NUM_THREADS);
		CPPAD_ASSERT_UNKNOWN( 
			thread == get_thread_num() || (! in_parallel()) 
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
			thread == get_thread_num() || (! in_parallel()) 
		);
		size_t* available_vec = available_vector();
		// do the subtraction
		CPPAD_ASSERT_UNKNOWN( available_vec[thread] >= dec );
		available_vec[thread] =  available_vec[thread] - dec;
	}

	// ----------------------------------------------------------------------
	/// Vector of length CPAD_MAX_NUM_THREADS times CPPAD_MAX_NUM_CAPACITIES 
	/// for use as root nodes of corresponding lists.
	static omp_alloc* root_vector(void)
	{	static omp_alloc  
		root[CPPAD_MAX_NUM_THREADS * CPPAD_MAX_NUM_CAPACITY];
		return root;
	}

// ============================================================================
public:
	// -----------------------------------------------------------------------
	/// Get current OpenMP thread number (zero if _OpenMP not defined).
	static size_t get_thread_num(void)
	{
# ifdef _OPENMP
		size_t thread = static_cast<size_t>( omp_get_thread_num() );
		assert( thread < MAX_NUM_THREADS );
		return thread;
# else
		return 0;
# endif
	}

	// -----------------------------------------------------------------------
	/// Are we in a parallel execution state; i.e., is it possible that
	/// other threads are currently executing.
	static bool in_parallel(void)
	{
# ifdef _OPENMP
		return static_cast<bool>( omp_in_parallel() );
# else
		return false;
# endif
	}
	/*!
 	Use omp_alloc to get a specified amount of memory.

	\param size
	The number of bytes of memory to be obtained.

	\return
	pointer to the beginning of the memory.
 	*/
	static void* get_memory(size_t size)
	{	size_t num_cap = capacity_info()->number;
		assert( size > 0 );

		// determine the capacity for this request
		size_t cap       = 0;
		const size_t* capacity_vec = capacity_info()->value;
		while( capacity_vec[cap] < size )
		{	++cap;	
			assert(cap < num_cap );
		}
		size_t capacity = capacity_vec[cap];

		// determine the thread and index
		size_t thread = get_thread_num();
		size_t index  = thread * num_cap + cap;

		// check if we already have a node we can use
		omp_alloc* root_vec   = root_vector();
		void* v_ptr           = root_vec[index].next_;
		omp_alloc* node       = reinterpret_cast<omp_alloc*>(v_ptr);
		if( node != 0 )
		{	// remove node from linked list
			root_vec[index].next_ = node->next_;

			// set information this allocation
			CPPAD_ASSERT_UNKNOWN( node->index_ == index );
			node->next_  = 0;

			// adjust counts
			inc_inuse(capacity, thread);
			dec_available(capacity, thread);

			// return pointer to memory after omp_alloc information
			return reinterpret_cast<void*>(node + 1);
		}

		// create a new node plus extra memory and use it
		v_ptr           = ::operator new(sizeof(omp_alloc) + capacity);
		node            = reinterpret_cast<omp_alloc*>(v_ptr);
		node->index_    = index;
		node->next_     = 0;

		// adjust counts
		inc_inuse(capacity, thread);

		return reinterpret_cast<void*>(node + 1);
	}

	// -----------------------------------------------------------------------
	/*!
 	Return memory that was obtained by \c get_memory.
	The returned memory becomes available for use by the current thread.

	\param v_ptr
	Value of the pointer returned by \c get_memory.
 	*/
	static void return_memory(void* v_ptr)
	{	size_t num_cap   = capacity_info()->number;

		omp_alloc* node          = reinterpret_cast<omp_alloc*>(v_ptr) - 1;
		omp_alloc* root_vec    	= root_vector();
		size_t index             = node->index_;
		node->next_              = root_vec[index].next_;
		root_vec[index].next_    = node;

		// extract thread and capacity from index
		size_t thread    = index / num_cap;
		size_t cap       = index % num_cap;
		size_t capacity  = capacity_info()->value[cap];
		CPPAD_ASSERT_UNKNOWN( thread == get_thread_num() );

		// adjust counts
		dec_inuse(capacity, thread);
		inc_available(capacity, thread);
	}
	// -----------------------------------------------------------------------
	/*!
	Return all memory being held for all threads to the system.
	This function assumes that only one thread will be executing 
	during this function call.
	*/
	static void free_available(void)
	{	size_t num_cap = capacity_info()->number;
	
		assert( ! in_parallel() );
		if( num_cap == 0 )
			return;
		omp_alloc* root_vec             = root_vector();
		const size_t*     capacity_vec  = capacity_info()->value;
		size_t thread, cap, index;
		for(thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
		{	for(cap = 0; cap < num_cap; cap++)
			{	size_t capacity = capacity_vec[cap];
				index = thread * num_cap + cap;
				void* v_ptr = root_vec[index].next_;
				while( v_ptr != 0 )
				{	omp_alloc* node = reinterpret_cast<omp_alloc*>(v_ptr); 
					void* next      = node->next_;
					::operator delete(v_ptr);
					v_ptr           = next;

					dec_available(capacity, thread);
				}
			}
			assert( available(thread) == 0 );
		}
	}
	/*!
	Determine the amount of memory that is currently inuse.

	\param thread
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
			thread == get_thread_num() || (! in_parallel()) 
		);
		return inuse_vector()[thread];
	}
	/*!
	Determine the amount of memory that is currently available for use.

	\copydetails inuse
	*/
	static size_t available(size_t thread)
	{
		CPPAD_ASSERT_UNKNOWN( thread < CPPAD_MAX_NUM_THREADS);
		CPPAD_ASSERT_UNKNOWN( 
			thread == get_thread_num() || (! in_parallel()) 
		);
		return available_vector()[thread];
	}
};

CPPAD_END_NAMESPACE
# endif
