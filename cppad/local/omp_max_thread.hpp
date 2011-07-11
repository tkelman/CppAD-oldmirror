/* $Id$ */
# ifndef CPPAD_OMP_MAX_THREAD_INCLUDED
# define CPPAD_OMP_MAX_THREAD_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin omp_max_thread$$
$spell
	omp
	OpenMp
	CppAD
$$

$section OpenMP Maximum Thread Number$$

$head Deprecated$$
$index depreciated, omp_max_thread$$
$index omp_max_thread, depreciated$$
This routine has been deprecated.
You should instead use the $cref/max_num_threads/$$ function.

$head Syntax$$
$syntax%AD<%Base%>::omp_max_thread(%number%)
%$$

$head Purpose$$
By default, for each $syntax%AD<%Base%>%$$ class there is only one 
tape that records $cref/AD of Base/glossary/AD of Base/$$ operations.
This tape is a global variable and hence it cannot be used
by multiple OpenMP threads at the same time. 
The $code omp_max_thread$$ function is used to set the 
maximum number of OpenMP threads that can be active.
In this case, there is a different tape corresponding to each 
$syntax%AD<%Base%>%$$ class and thread pair. 

$head number$$
The argument $italic number$$ has prototype
$syntax%
	size_t %number%
%$$
It must be greater than zero and specifies the maximum number of
OpenMp threads that will be active at one time.


$head Independent$$
Each call to $cref/Independent(x)/Independent/$$ 
creates a new $cref/active/glossary/Tape/Active/$$ tape.
All of the operations with the corresponding variables 
must be preformed by the same OpenMP thread.
This includes the corresponding call to 
$cref/f.Dependent(x,y)/Dependent/$$ or the 
$cref/ADFun f(x, y)/FunConstruct/Sequence Constructor/$$
during which the tape stops recording and the variables
become parameters.

$head Restriction$$
No tapes can be 
$cref/active/glossary/Tape/Active/$$ when this function is called.

$end
-----------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
void AD<Base>::omp_max_thread(size_t number)
{
	CPPAD_ASSERT_KNOWN( 
		number > 0 ,
		"omp_max_thread: argument is zero.\n"
		"This routine has been deprecated, use omp_alloc::max_num_threads."
	);
	
	CPPAD_ASSERT_KNOWN(
		number <= CPPAD_MAX_NUM_THREADS,
		"omp_max_thread argument is too large.\n"
		"This routine has been deprecated, use omp_alloc::max_num_threads."
	);

	omp_alloc::max_num_threads(number);
}

} // END CppAD namespace

# endif
