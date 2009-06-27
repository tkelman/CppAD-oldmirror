/* $Id: configure.hpp.in 1401 2009-06-24 17:56:51Z bradbell $ */
/* This file may be inclued multiple times so comment out ifndef and define
# ifndef CPPAD_CONFIGURE_INCLUDED
# define CPPAD_CONFIGURE_INCLUDED
*/

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*!
 \file configure.hpp
Wrapper for the cppad/local/config.h file generated by autoconf and configure.

\li
Un-define all the preprocessor symbols in config.h that do not begin with
CPPAD_.

\li
Include config.h.

\li
Repeat Un-define all the preprocessor symbols in config.h that do not begin 
with CPPAD_.

\li
Define CPPAD_PACKAGE_STRING as equal to "CppAD yyyymmdd" where
yyyy is year, mm is month, and dd is day.
*/
# undef PACKAGE
# undef PACKAGE_BUGREPORT
# undef PACKAGE_NAME
# undef PACKAGE_STRING
# undef PACKAGE_TARNAME
# undef PACKAGE_VERSION
# undef VERSION

// now it is safe to use the config.h file
# include <cppad/config.h>


// undefine all the preprocessor symbols that do not have CPPAD_ in front
// (all such symbols are in the config.h file created by autoconf).
# undef PACKAGE
# undef PACKAGE_BUGREPORT
# undef PACKAGE_NAME
# undef PACKAGE_STRING
# undef PACKAGE_TARNAME
# undef PACKAGE_VERSION
# undef VERSION

/*!
\def CPPAD_PACKAGE_STRING
cppad-yyyymmdd as a C string where yyyy is year, mm is month, and dd is day.
*/
# define CPPAD_PACKAGE_STRING "CppAD 20090627"

/*
$begin configure$$
$spell
	config
	cppad
	autoconf
	CppAD
	undef
$$

$section Preprocessor Definitions Used by CppAD$$
$index configure, symbol$$
$index symbol, configure$$
$index PACKAGE, symbol$$
$index VERSION, symbol$$

$head Preprocessor Symbols$$
$index preprocessor, symbol$$
$index symbol, processor$$
All of the preprocessor symbols used by CppAD begin either with
$code CppAD$$ or with $code CPPAD_$$.

$head Example$$
For example,
the preprocessor symbol 
$small $cref/CPPAD_TEST_VECTOR/test_vector/$$ $$ 
determines which
$xref/SimpleVector/$$ template class is extensively used 
by the tests in the $code Example$$ and $code TestMore$$ directories.

$head config.h$$
$index config.h$$
The following is a list of exceptions to the rule above.
These preprocessor symbols are placed in the file $code cppad/config.h$$
by $code autoconf$$ (which is used to generate the CppAD distribution).
Directly after you include a CppAD file, these symbols may be undefined:
$code PACKAGE$$,
$code PACKAGE_BUGREPORT$$,
$code PACKAGE_NAME$$,
$code PACKAGE_STRING$$,
$code PACKAGE_TARNAME$$,
$code PACKAGE_VERSION$$,
$code VERSION$$.

$end
*/

