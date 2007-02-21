/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin max_active$$
$spell
	CppAD
$$

$section Multiple Active Tapes and Multi-Threading$$

$index tape_max_active$$
$index thread, multiple$$
$index multiple, thread$$
$index multiple, active tapes$$
$index active, multiple tapes$$

$head Syntax$$
$syntax%AD<%Base%>::tape_max_active(%number%)
%$$

$head Purpose$$
By default, 
there can only be one active tape recording
$syntax/AD of Base/glossary/AD of Base/$$ operations
(for each base class $italic Base$$).
Restricting to one active tape prohibits
using multi-threading with CppAD because the tape
is a global variable that is read and modified often.
The $code tape_max_active$$ function is used to change the 
maximum number of tapes that can be active at one time.

$head number$$
The argument $italic number$$ has prototype
$syntax%
	size_t %number%
%$$
It must be greater than zero and specifies the maximum number of
tapes that can be active at one time.


$head Variables on Different Tapes$$
Each CppAD $cref/Variable/glossary/$$ has a corresponding active tape.
It is an error to preform an operation that combines variables
on different tapes.
For example, the following operation is not valid:
$syntax%
	Independent(%x1%);         // create a new active tape
	Independent(%x2%);         // create another active tape
	%y% = %x1%[0] * %x2%[0];   // combine variables from the two tapes
%$$
Note that once a tape becomes inactive, all of its variables become
$cref/Parameters/glossary/Parameter/$$.
For example, the following operation is valid:
$syntax%
	Independent(%x1%);         // create a new active tape
	%y1% = %x1% * %x1%;        // define a new variable on the tape
	%f%.Dependent(%x1%, %y1%); // create f : x1 -> y1 and inactive tape
	Independent(%x2%);         // create another active tape
	%y2% = %x1%[0] * %x2%[0];  // combine parameter x1 with variable x2
%$$

$head Example$$
$children%
	example/mul_thread.cpp
%$$
The file $cref/mul_thread.cpp/$$ contains an example and test
of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
size_t AD<Base>::tape_max_active(size_t number)
{	static size_t max_active = 1;

	// check for this error even in optimized code becasue 
	// it is not yet part of user documentation.
	if( number >= CPPAD_LENGTH_TAPE_TABLE )
	{	bool known       = true;
		int  line        = __LINE__;
        	const char *file = __FILE__;
		const char *exp  = "number < CPPAD_LENGTH_TAPE_TABLE";
        	const char *msg  = "tape_max_active: argument is to large.";
		ErrorHandler::Call(known, line, file, exp, msg);
	}

	// number equal zero case is not part of user interface
	if( number > 0 )
		max_active = number;

	// the return value is not part of the user interface
	return max_active;
}

} // END CppAD namespace
