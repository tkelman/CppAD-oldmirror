# ifndef CppADCppADvectorIncluded
# define CppADCppADvectorIncluded

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */

/*
$begin CppADvector$$
$spell
	hpp
	CppADvector
	std
	endif
	ublas
	Dir
	valarray
	stdvector
$$

$index CppADvector$$
$index vector, test$$
$index test, vector$$

$section Choosing The CppADvector Template Class$$

$head Syntax$$
$syntax%CppADvector<%Scalar%>%$$
$pre
$$
$bold File$$
$code CppAD/local/CppADvector.h$$



$head Introduction$$
Many of the CppAD $xref/Example//examples/$$ and tests use 
the $code CppADvector$$ template class to pass information.
The default definition for this template class is
$xref/CppAD_vector//CppAD::vector/$$
(see the file $code CppAD/local/CppADvector.h$$).

$head CppAD::vector$$
The default for the preprocessor symbol $code CppADvector$$
is specified by the following source code
$codep */

// The next line is C++ source code and its replacement is discussed below
# if CppAD_CPPADVECTOR
# define CppADvector CppAD::vector
# endif

/* $$
You can replace this default definition of the preprocessor symbol
$code CppADvector$$ by any other $xref/SimpleVector/$$ template class.
This will test using your replacement template vector class with CppAD.

$head std::vector$$
If you specify $code --with-stdvector$$ on the $code configure$$
command line during CppAD $xref/Install//installation/$$,
the following code replaces the definition of $code CppADvector$$ 
$codep */

# if CppAD_STDVECTOR
# include <vector>
# define CppADvector std::vector
# endif

/* $$
then CppAD will use $code std::vector$$ for its examples and tests.
Use of $code CppAD::vector$$, $code std::vector$$,
and $code std::valarray$$ with CppAD is always tested to some degree.
Specifying $code --with-stdvector$$ will increase the amount of
$code std::vector$$ testing. 

$head boost::numeric::ublas::vector$$
If you specify a value for $italic BoostDir$$ on the $code configure$$
command line during CppAD $xref/Install//installation/$$,
the following code replaces the definition of $code CppADvector$$ 
$codep */

# if CppAD_BOOSTVECTOR
# include <boost/numeric/ublas/vector.hpp>
# define CppADvector boost::numeric::ublas::vector
# endif

/* $$
to be the directory where you installed 
In the Windows case, you will also need to change the windows project files
and add $italic BoostDir$$ to the extra include directories.

$end
------------------------------------------------------------------------ 
*/

# endif
