// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------
$begin pthread_team.cpp$$
$spell
	hpp
	pthread
	pthreads
	num
	ta
	Cpp
$$
$section Using AD with a Team of Pthreads$$

$index pthread, AD team$$
$index AD, pthread team$$
$index team, AD pthread$$

$head Syntax$$
$codei%include "pthread_team.hpp"
%$$
$icode%ok% = start_team(%num_threads%)
%$$
$icode%ok% = work_team(%worker%)
%$$
$icode%ok% = stop_team()
%$$

$head Purpose$$
These routines start, use, and stop a team of $code pthreads$$.
In addition, these thread can be used with the type $code AD<double>$$.

$head thread_num$$
The thread that calls 
$code start_team$$,
$code work_team$$, and
$code stop_team$$, has 
$icode/thread_num/ta_thread_num/$$ equal to zero.

$head start_team$$
Execution must be in sequential mode when this routine is called.
The argument $icode num_threads$$ specifies the number of pthreads
in this team.
If $codei%num_threads% > 1%$$,
the value returned by the $cref/in_parallel/ta_in_parallel/$$ after
$code start_team$$ will be true (because parallel mode is enabled).
This will continue until $code stop_team$$ is called.
(unless $cref/parallel_setup/ta_parallel_setup/$$ is called by
a routine other than $code stop_team$$ which should not happen).

$head work_team$$
This routine may be called one or more times
between the call to $code start_team$$ and $code stop_team$$.
Although $cref/in_parallel/ta_in_parallel/$$ returns true,
actual execution between calls to $code team_work$$ is sequential; i.e.,
there is only one thread.
Each call to $code team_work$$ runs $icode num_threads$$ versions
of $icode worker$$ with the corresponding value of
$icode/thread_num/ta_thread_num/$$ different for each thread.

$head stop_team$$
Execution must be in sequential mode when this routine is called.
This routine terminates all the other threads except for
thread number zero and calls
$cref/parallel_setup/ta_parallel_setup/$$ to inform CppAD
that we are back to the single thread execution mode.

$head Example$$
See $cref pthread_simple_ad.cpp$$.

$head Source$$
$code
$verbatim%multi_thread/pthread/pthread_team.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <pthread.h>
# include <cppad/cppad.hpp>

# define MAX_NUMBER_THREADS        48
# define DEMONSTRATE_BUG_IN_CYGWIN 0

namespace {
	size_t num_threads_ = 1; 

	// Barrier used to wait for all thread identifiers to be set
	enum thread_job_t { init_enum, work_enum, join_enum } thread_job_;
	pthread_barrier_t wait_for_work_;
	pthread_barrier_t wait_for_job_;

	// general purpose vector with information for each thread
	typedef struct {
		// pthread unique identifier for thread that uses this struct
		pthread_t       pthread_id;
		// cppad unique identifier for thread that uses this struct
		size_t          thread_num;
		// true if no error for this thread, false otherwise.
		bool            ok;
	} thread_one_t;
	thread_one_t thread_all_[MAX_NUMBER_THREADS];

	// pointer to function that does the work
	void (* worker_)(void) = 0;

	// ---------------------------------------------------------------------
	// in_parallel()
	bool in_parallel(void)
	{	return num_threads_ > 1; }

	// ---------------------------------------------------------------------
	// thread_number()
	size_t thread_number(void)
	{
		// pthread unique identifier for this thread
		pthread_t thread_this = pthread_self();

		// convert thread_this to the corresponding thread_num
		size_t thread_num = 0;
		for(thread_num = 0; thread_num < num_threads_; thread_num++)
		{	// pthread unique identifier for this thread_num
			pthread_t thread_compare = thread_all_[thread_num].pthread_id;

			// check for a match
			if( pthread_equal(thread_this, thread_compare) )
				return thread_num;
		}
		// no match error (thread_this is not in thread_all_).
		std::cerr << "thread_number: unknown pthread id" << std::endl;
		exit(1);

		return 0;
	}
	// --------------------------------------------------------------------
	// function that gets called by pthread_create
	void* thread_work(void* thread_one_vptr)
	{
		// thread management information for this thread
		thread_one_t* thread_one = 
			static_cast<thread_one_t*>(thread_one_vptr);

		// thread_num to problem specific information for this thread
		size_t thread_num = thread_one->thread_num;

		while( thread_job_ != join_enum )
		{	switch( thread_job_ )
			{
				case init_enum:
				break;

				case work_enum:
				worker_();
				break;

				default:
				std::cerr << "thread_work: default case" << std::endl;
				exit(1);
			}
			// Wait for other threads to do thier work.
			int rc = pthread_barrier_wait(&wait_for_work_);
			thread_all_[thread_num].ok &= 
				(rc == 0 || rc == PTHREAD_BARRIER_SERIAL_THREAD);

			// if this is the master, exit the loop
			if( thread_num == 0 )
				return 0;

			// Wait for master to specify a new job
			rc = pthread_barrier_wait(&wait_for_job_);
			thread_all_[thread_num].ok &= 
				(rc == 0 || rc == PTHREAD_BARRIER_SERIAL_THREAD);
		}
		// It this is not the master thread, then terminate it.
# if DEMONSTRATE_BUG_IN_CYGWIN
		if( ! pthread_equal(
			thread_one->pthread_id, thread_all_[0].pthread_id) )
		{	void* no_status = 0;
			pthread_exit(no_status);
		}
# endif
		// return null pointer
		return 0;
	}
}

bool start_team(size_t num_threads)
{	using CppAD::thread_alloc;
	bool ok = true;;

	// set number global version of number of threads
	num_threads_ = num_threads;

	// initialize two barriers, one for work done, one for new job ready
	pthread_barrierattr_t *no_barrierattr = 0;
	int rc = pthread_barrier_init(
		&wait_for_work_, no_barrierattr, num_threads
	); 
	ok &= (rc == 0);
	rc  = pthread_barrier_init(
		&wait_for_job_, no_barrierattr, num_threads
	); 
	ok &= (rc == 0);

	// initial job for the threads
	thread_job_ = init_enum;
	
	// structure used to create the threads
	pthread_t      pthread_id;
	pthread_attr_t attr;
	void*          thread_one_vptr;
	//
	rc  = pthread_attr_init(&attr);
	ok &= (rc == 0);
	rc  = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	ok &= (rc == 0);

	// This master thread is already running, we need to create
	// num_threads - 1 more threads
	thread_all_[0].pthread_id = pthread_self();
	thread_all_[0].thread_num = 0;
	thread_all_[0].ok         = true;
	size_t thread_num;
	for(thread_num = 1; thread_num < num_threads; thread_num++)
	{	thread_all_[thread_num].ok         = true;
		thread_all_[thread_num].thread_num = thread_num;
		// Create the thread with thread number equal to thread_num
		thread_one_vptr = static_cast<void*> (&(thread_all_[thread_num]));
		rc = pthread_create(
				&pthread_id ,
				&attr       ,
				thread_work ,
				thread_one_vptr
		);
		thread_all_[thread_num].pthread_id = pthread_id;
		ok &= (rc == 0);
	}

	// Done creating threads and hence no longer need this attribute object
	rc  = pthread_attr_destroy(&attr);
	ok &= (rc == 0);

	// Now that thread_number() has necessary information, 
	// call setup for using CppAD::AD<double> in parallel mode
	thread_alloc::parallel_setup(num_threads, in_parallel, thread_number);
	CppAD::parallel_ad<double>();

	//  wait until all threads have completed wait_for_work_
	thread_one_vptr = static_cast<void*> (&(thread_all_[0]));
	thread_work(thread_one_vptr);

	// Current state is all threads have completed wait_for_work_
	// Thread zero has not completed wait_for_job_
	for(thread_num = 0; thread_num < num_threads; thread_num++)
		ok &= thread_all_[thread_num].ok;
	return ok;
}

bool work_team(void worker(void))
{	using CppAD::thread_alloc;
	bool ok = true;;

	// Current state is all threads have completed wait_for_work_
	// Thread zero has not completed wait_for_job_

	// set global version of work routine
	worker_ = worker;

	// set the new job that other threads are waiting for
	thread_job_ = work_enum;

	// reset wait_for_work_
	int rc = pthread_barrier_destroy(&wait_for_work_);
	ok    &= (rc == 0);
	pthread_barrierattr_t *no_barrierattr = 0;
	rc     = pthread_barrier_init(
		&wait_for_work_, no_barrierattr, num_threads_
	); 
	ok &= (rc == 0);

	// wait until all threads have completed wait_for_job_
	rc  = pthread_barrier_wait(&wait_for_job_);
	ok &= (rc == 0 || rc == PTHREAD_BARRIER_SERIAL_THREAD);

	// reset wait_for_job_
	rc  = pthread_barrier_destroy(&wait_for_job_);
	ok &= (rc == 0);
	rc  = pthread_barrier_init(
		&wait_for_job_, no_barrierattr, num_threads_
	); 
	ok &= (rc == 0);

	//  wait until all threads have completed wait_for_work_
	void* thread_one_vptr = static_cast<void*> (&(thread_all_[0]));
	thread_work(thread_one_vptr);

	// Current state is all threads have completed wait_for_work_
	// Thread zero has not completed wait_for_job_
	size_t thread_num;
	for(thread_num = 0; thread_num < num_threads_; thread_num++)
		ok &= thread_all_[thread_num].ok;
	return ok;
}

bool stop_team(void)
{	bool ok = true;

	// Current state is all threads have completed wait_for_work_
	// Thread zero has not completed wait_for_job_

	// set the new job that other threads are waiting for
	thread_job_ = join_enum;

	// destroy wait_for_work_
	int rc  = pthread_barrier_destroy(&wait_for_work_);
	ok     &= (rc == 0);

	// wait until all threads have completed wait_for_job_
	rc  = pthread_barrier_wait(&wait_for_job_);
	ok &= (rc == 0 || rc == PTHREAD_BARRIER_SERIAL_THREAD);

	// destroy wait_for_job_
	rc  = pthread_barrier_destroy(&wait_for_job_);
	ok &= (rc == 0);

	// now wait for the other threads to finish
	size_t thread_num;
	for(thread_num = 1; thread_num < num_threads_; thread_num++)
	{	void* no_status = 0;
		rc      = pthread_join(
			thread_all_[thread_num].pthread_id, &no_status
		);
		ok &= (rc == 0);
	}

	// check ok before changing num_threads_
	for(thread_num = 0; thread_num < num_threads_; thread_num++)
		ok &= thread_all_[thread_num].ok;

	// now return the CppAD system to one thread mode
	num_threads_ = 1;
	using CppAD::thread_alloc;
	thread_alloc::parallel_setup(num_threads_, in_parallel, thread_number);

	return ok;
}
// END PROGRAM
