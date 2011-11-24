/* $Id$ */
# ifndef CPPAD_UNDEF_INCLUDED
# define CPPAD_UNDEF_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// Undo definitions in define.hpp that are not in API
# undef CPPAD_OP_CODE_TYPE
// CPPAD_TAPE_ID_TYPE            is in API
# undef CPPAD_USE_FORWARD0SWEEP
// # CPPAD_BEGIN_NAMESPACE       needed by cppad_ipopt
// # CPPAD_END_NAMESPACE         needed by cppad_ipopt
# undef CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
// # CPPAD_NUL                   is in the API
// # CPPAD_MAX_NUM_THREADS       is in API
# undef CPPAD_FOLD_ASSIGNMENT_OPERATOR
# undef CPPAD_FOLD_AD_VALUED_BINARY_OPERATOR
# undef CPPAD_FOLD_BOOL_VALUED_BINARY_OPERATOR

# endif
