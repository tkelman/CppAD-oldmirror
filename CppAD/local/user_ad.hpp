# ifndef CppADUserADIncluded
# define CppADUserADIncluded

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */
/*
---------------------------------------------------------------------------

$begin AD$$
$spell
	bool
	cos
	Cpp
$$

$section AD Objects$$

$index AD, object$$

$head Purpose$$
The sections listed below describe the operations 
that are available to $xref/glossary/AD of Base/AD of Base/$$ objects.
These objects are used to
$xref/glossary/Tape State/Recording/record/1/$$
an AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.
This operation sequence can
be transferred to an $xref/ADFun/$$ object where it
can be used to evaluate the corresponding 
function and derivative values.

$childtable%
	CppAD/local/constructor.hpp%
	CppAD/local/convert.hpp%
	CppAD/local/eq.hpp%
	CppAD/local/ad_valued.hpp%
	CppAD/local/bool_valued.hpp%
	CppAD/local/vec_ad.hpp
%$$

$end
---------------------------------------------------------------------------
*/

# include <CppAD/local/constructor.hpp>
# include <CppAD/local/convert.hpp>
# include <CppAD/local/eq.hpp>
# include <CppAD/local/ad_valued.hpp>
# include <CppAD/local/bool_valued.hpp>
# include <CppAD/local/vec_ad.hpp>

# endif
