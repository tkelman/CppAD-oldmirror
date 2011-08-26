// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin pthread_ad.cpp$$
$spell
	pthread
	pthreads
	CppAD
$$

$section Simple Pthread Parallel AD: Example and Test$$

$index pthread_ad, example$$
$index AD, parallel pthread$$
$index parallel, AD pthread$$

$head Purpose$$
This example demonstrates how CppAD can be used with multiple pthreads.

$head Discussion$$
The function $code arc_tan$$ below
is an implementation of the inverse tangent function where the
$cref/operation sequence/glossary/Operation/Sequence/$$ depends on the
argument values. 
Hence the function needs to be re-taped 
for different argument values.
The $cref/atan2/$$ function uses $cref/CondExp/$$ operations
to avoid the need to re-tape.

$head Source Code$$
$code
$verbatim%example/pthread_ad.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <pthread.h>
# include <cppad/cppad.hpp>

# define NUMBER_THREADS    4

namespace {
	// -------------------------------------------------------------------
	// general purpose code for linking CppAD to pthreads
	size_t num_threads = NUMBER_THREADS; 
	typedef struct {
		// mutex used to synchronize access to this record
		pthread_mutex_t mutex;
		// Pthread unique identifier for one thread.
		pthread_t       thread_id;
		// Angle for this thread_work calculation.
		double          theta;
		// Has no error occurred.
		bool            ok;
	} thread_info;
	// vector with information for each thread
	thread_info info_vector[NUMBER_THREADS];

	// The master thread switches the value of this variable
	static bool multiple_threads_active = false;
	// This function passes parallel mode information to CppAD 
	bool in_parallel(void)
	{	return multiple_threads_active; }

	// This function gets a number less than num_threads that is 
	// unique for each thread. CppAD uses this information.
	size_t thread_num(void)
	{	static bool wait = true;

		// Check for case where we know it this is the master thread
		if( ! multiple_threads_active )
			return 0;

		// The first time this routine is called in parallel mode
		// wait until all the thread_id values have been set (only done once).
		size_t index;
		int rc;
		while( wait )
		{	wait = false;
			pthread_t thread_zero;
			for(index = 0; index < num_threads; index++)
			{	// get a lock on the info_vector[index]	
				rc    = pthread_mutex_lock( &(info_vector[index].mutex) );
				info_vector[index].ok &= (rc == 0);
				if( index == 0 )
				{	// store the identifies corresponding to the master
					thread_zero = info_vector[index].thread_id;
				}
				else
				{	// If this thread has not yet set its thread_id,
					// try again later.
					wait |= info_vector[index].thread_id == thread_zero;
				}
				rc    = pthread_mutex_unlock( &(info_vector[index].mutex));
				info_vector[index].ok &= (rc == 0);
			}
		}
		
		// pthread system unique identifier for this thread
		pthread_t thread_this = pthread_self();
		// convert it to the thread index
		index = 0;
		for(index = 0; index < num_threads; index++)
		{	// pthread system unique identifier for this index
			pthread_t thread_compare = info_vector[index].thread_id;
			// check for a match
			if( pthread_equal(thread_this, thread_compare) )
				return index;
		}
		// no match error (thread_this is not in info_vector).
		assert(false);
		return index;
	}
	// -------------------------------------------------------------------
	// code for problem we are solving
	using CppAD::AD;
	using CppAD::NearEqual;
	AD<double> arc_tan(const AD<double>& x, const AD<double>& y)
	{	double pi  = 4. * atan(1.);
		AD<double> theta;

		// valid for first quadrant 
		if( abs(x) > abs(y) )
			theta = atan(abs(y) / abs(x));
		else	theta = pi / 2. - atan(abs(x) / abs(y) ) ;

		// valid for first or second quadrant
		if( x < 0. )
			theta = pi - theta;

		// valid for any quadrant
		if( y < 0. )
			theta = - theta;

		return theta;
	}
	void* thread_work(void* info_vptr)
	{	// more code for linking CppAD to pthread ------------------------
		thread_info* info = static_cast<thread_info*>(info_vptr);

		// Wait here until master is done initializing info_vector.
		int rc = pthread_mutex_lock( &(info->mutex) );
		info->ok &= (rc == 0);

		// Now set the thread_id for this thread index
		info->thread_id = pthread_self();

		// Unlock this info record so that thread_num can read it.
		rc = pthread_mutex_unlock( &(info->mutex) );
		info->ok &= (rc == 0);
		// ----------------------------------------------------------------

		// CppAD::vector uses the CppAD fast multi-threading allocator
		CppAD::vector< AD<double> > Theta(1), Z(1);
		Theta[0] = info->theta;
		Independent(Theta);
		AD<double> x = cos(Theta[0]);
		AD<double> y = sin(Theta[0]);
		Z[0]  = arc_tan( x, y );
		CppAD::ADFun<double> f(Theta, Z); 

		// Check function value corresponds to the identity 
		// (must get a lock before changing info).
		double eps = 10. * CppAD::epsilon<double>();
		rc         = pthread_mutex_lock( &(info->mutex) );
		info->ok  &= (rc == 0);
		info->ok  &= NearEqual(Z[0], Theta[0], eps, eps);
		rc         = pthread_mutex_unlock( &(info->mutex) );
		info->ok  &= (rc == 0);

		// Check derivative value corresponds to the identity.
		CppAD::vector<double> d_theta(1), d_z(1);
		d_theta[0] = 1.;
		d_z        = f.Forward(1, d_theta);
		rc         = pthread_mutex_lock( &(info->mutex) );
		info->ok  &= (rc == 0);
		info->ok  &= NearEqual(d_z[0], 1., eps, eps);
		rc         = pthread_mutex_unlock( &(info->mutex) );
		info->ok  &= (rc == 0);

		// It this is not the master thread, then terminate it.
		if( ! pthread_equal(info->thread_id, info_vector[0].thread_id) )
		{	void* no_status = 0;
			pthread_exit(no_status);
		}
	
		// return null pointer
		return 0;
	}
}

bool pthread_ad(void)
{	bool all_ok = true;
	using CppAD::AD;
	using CppAD::vector;

		
	// Check that no memory is in use or avialable at start
	// (using thread_alloc in sequential mode)
	size_t index;
	for(index = 0; index < num_threads; index++)
	{	all_ok &= CppAD::thread_alloc::inuse(index) == 0; 
		all_ok &= CppAD::thread_alloc::available(index) == 0; 
	}

	// Setup for using AD<double> in parallel mode
	CppAD::thread_alloc::parallel_setup(num_threads, in_parallel, thread_num);
	CppAD::parallel_ad<double>();

	// now go into parallel execution mode
	multiple_threads_active = true;
	
	// create and run the other threads (this thread is alread running)
	pthread_t      thread[NUMBER_THREADS];
	int            rc;
	pthread_attr_t attr;
	void*          info_vptr;
	rc      = pthread_attr_init(&attr);
	all_ok &= (rc == 0);
	rc      = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	all_ok &= (rc == 0);
	// this master thread is already running, we need num_thread - 1 more
	for(index = 1; index < num_threads; index++)
	{	// initialize and lock the mutex for this thread
		pthread_mutexattr_t* no_mutexattr = 0;
		rc = pthread_mutex_init(&(info_vector[index].mutex), no_mutexattr);
		info_vector[index].ok &= (rc == 0);
		rc = pthread_mutex_lock(&(info_vector[index].mutex));
		info_vector[index].ok &= (rc == 0);

		// create and put this thread to sleep (mutex is locked)
		info_vptr = static_cast<void*> ( &(info_vector[index]) );
		rc = pthread_create(
				&thread[index] , 	
				&attr              ,
				thread_work        ,
				info_vptr
		);
		all_ok &= (rc == 0);
	}
	// now initialize the info vector
 	double pi = 4. * atan(1.);
	for(index = 0; index < num_threads; index++)
	{	// theta is const and will not change
		info_vector[index].theta     = index * pi / num_threads;
		// initialize ok for each thread as true
		info_vector[index].ok        = true;
		// set all threads to have identifier corresponding to master
		info_vector[index].thread_id = pthread_self();
		// now unlock this thread (if it is not the master)
		if( 0 < index )
		{	rc = pthread_mutex_unlock( &(info_vector[index].mutex) );
			info_vector[index].ok &= (rc == 0);
		}
	}

	// now, while other threads are working, do work in master
	info_vptr = static_cast<void*> ( &(info_vector[0]) );
	thread_work(info_vptr);

	// now wait for the other threads to finish
	for(index = 1; index < num_threads; index++)
	{	void* no_status = 0;
		rc      = pthread_join(thread[index], &no_status);
		all_ok &= (rc == 0);
	}

	// now return to sequential execution mode
	multiple_threads_active = false;

	// summarize results
	for(index = 0; index < num_threads; index++)
		all_ok &= info_vector[index].ok;

	// free up the pthread resources that are no longer in use
	rc      = pthread_attr_destroy(&attr);
	all_ok &= (rc == 0);
	for(index = 0; index < num_threads; index++)
	{	rc      = pthread_mutex_destroy(&(info_vector[index].mutex));
		all_ok &= (rc == 0);
	}

	// Check that no memory currently in use, free avialable
	for(index = 0; index < num_threads; index++)
	{	all_ok &= CppAD::omp_alloc::inuse(index) == 0; 
		CppAD::omp_alloc::free_available(index); 
	}

	// return memory allocator to single threading mode
	num_threads = 1;
	CppAD::thread_alloc::parallel_setup(num_threads, in_parallel, thread_num);

	return all_ok;
}
// END PROGRAM
