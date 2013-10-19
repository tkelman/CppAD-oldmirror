/* $Id$ */
# ifndef CPPAD_NUM_SKIP_INCLUDED
# define CPPAD_NUM_SKIP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin number_skip$$
$spell
	optimizer
	var
	taylor_
$$

$index number_skip$$
$index number, skip variables$$
$index skip, number variables$$
$index variables, skip$$

$section Number of Variables that Can be Skipped$$

$head Syntax$$
$icode%n% = %f%.number_skip()%$$

$subhead See Also$$
$cref seq_property$$

$head Purpose$$
The $cref/conditional expressions/CondExp/$$ use either the
$cref/if_true/CondExp/$$ or $cref/if_false/CondExp/$$.
This leads to the fact that some terms only need to be evaluated
depending on the value of the comparison in the conditional expression.
The $cref optimize$$ option is capable of detecting some of these 
case and determining variables that can be skipped.
This routine returns the number such variables.

$head n$$
The return value $icode n$$ has type $code size_t$$
is the number of variables that the optimizer has determined can be skipped 
(given the independent variable values specified by the previous call to
$cref/f.Forward/Forward/$$ for order zero).

$head f$$
The object $icode f$$ has prototype
$codei%
	ADFun<%Base%> %f%
%$$

$children%
	example/number_skip.cpp
%$$
$head Example$$
The file $cref number_skip.cpp$$
contains an example and test of this function.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
size_t ADFun<Base>::number_skip(void)
{	// temporary indices
	size_t count = 0;
	for(size_t i = 0; i < total_num_var_; i++)
		if( cskip_var_[i] )
			count++;
	return count;

}

} // END CppAD namespace
	

# endif
