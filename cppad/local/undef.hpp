/* $Id$ */
# ifndef CPPAD_UNDEF_INCLUDED
# define CPPAD_UNDEF_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
----------------------------------------------------------------------------
Preprecessor definitions that presist after cppad/cppad.hpp is included:

# undef CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL used by CPPAD_USER_ATOMIC
# undef CPPAD_ASSERT_KNOWN needed by cppad_ipopt
# undef CPPAD_ASSERT_UNKNOWN needed by cppad_ipopt
# undef CPPAD_HASH_TABLE_SIZE used by test_more/optimize.cpp
# undef EIGEN_MATRIXBASE_PLUGIN example use of Eigen with CppAD

# undef CPPAD_BOOL_BINARY         in user api
# undef CPPAD_BOOL_UNARY          in user api
# undef CPPAD_DISCRETE_FUNCTION   in user api
# undef CPPAD_EIGENVECTOR         in user api
# undef CPPAD_INTERNAL_SPARSE_SET in user api
# undef CPPAD_MAX_NUM_THREADS     in user api
# undef CPPAD_NULL                in user api
# undef CPPAD_PACKAGE_STRING      in user api
# undef CPPAD_TAPE_ADDR_TYPE      in user api
# undef CPPAD_TAPE_ID_TYPE        in user api
# undef CPPAD_TESTVECTOR          in user api

# undef CPPAD_TRACK_COUNT    in deprecated api
# undef CPPAD_TRACK_DEL_VEC  in deprecated api
# undef CPPAD_TRACK_EXTEND   in deprecated api
# undef CPPAD_TRACK_NEW_VEC  in deprecated api
# undef CPPAD_USER_ATOMIC    in deprecated api

# undef CPPAD_TEST_VECTOR     deprecated verssion of CPPAD_TESTVECTOR
# undef CppADCreateBinaryBool deprecated version of CPPAD_BOOL_BINARY
# undef CppADCreateDiscrete   deprecated version of CPPAD_DISCRETE_FUNCTION
# undef CppADCreateUnaryBool  deprecated version of CPPAD_BOOL_UNARY
# undef CppADTrackCount       deprecated version of CPPAD_TRACK_COUNT
# undef CppADTrackDelVec      deprecated version of CPPAD_TRACK_DEL_VEC
# undef CppADTrackExtend      deprecated version of CPPAD_TRACK_EXTEND
# undef CppADTrackNewVec      deprecated version of CPPAD_TRACK_NEW_VEC
# undef CppADvector           deprecated version of CPPAD_TEST_VECTOR
-----------------------------------------------------------------------------
*/
// Preprecessor definitions that do not presist 
# undef CPPAD_ASSERT_NARG_NRES
# undef CPPAD_ASSERT_NOT_PARALLEL
# undef CPPAD_BOOSTVECTOR
# undef CPPAD_COND_EXP
# undef CPPAD_COND_EXP_BASE_REL
# undef CPPAD_COND_EXP_REL
# undef CPPAD_CPPADVECTOR
# undef CPPAD_FOLD_AD_VALUED_BINARY_OPERATOR
# undef CPPAD_FOLD_ASSIGNMENT_OPERATOR
# undef CPPAD_FOLD_BOOL_VALUED_BINARY_OPERATOR
# undef CPPAD_FOR_JAC_SWEEP_TRACE
# undef CPPAD_HAS_GETTIMEOFDAY
# undef CPPAD_HAS_NULLPTR
# undef CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
# undef CPPAD_MAX_NUM_CAPACITY
# undef CPPAD_MIN_DOUBLE_CAPACITY
# undef CPPAD_OP_CODE_TYPE
# undef CPPAD_REVERSE_SWEEP_TRACE
# undef CPPAD_REV_HES_SWEEP_TRACE
# undef CPPAD_REV_JAC_SWEEP_TRACE
# undef CPPAD_SIZE_T_SAME_UNSIGNED_INT
# undef CPPAD_STANDARD_MATH_UNARY
# undef CPPAD_STANDARD_MATH_UNARY_AD
# undef CPPAD_STDVECTOR
# undef CPPAD_TRACE_CAPACITY
# undef CPPAD_TRACE_THREAD
# undef CPPAD_TRACK_DEBUG
# undef CPPAD_USE_FORWARD0SWEEP
# undef CPPAD_USER_MACRO
# undef CPPAD_USER_MACRO_ONE
# undef CPPAD_USER_MACRO_TWO
# undef CPPAD_VEC_AD_COMPUTED_ASSIGNMENT

# endif
