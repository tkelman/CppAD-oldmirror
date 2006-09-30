# ifndef CppADDetByMinorIncluded
# define CppADDetByMinorIncluded

// BEGIN SHORT COPYRIGHT
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
// END SHORT COPYRIGHT
/*
$begin DetByMinor$$
$spell
	Det
	CppADvector
	namespace
$$

$section Determinant Using Expansion by Minors$$

$table
$bold Syntax$$
$rnext $cnext $syntax%# include "Example/DetByMinor.h"%$$
$rnext $cnext $syntax%DetByMinor<%Type%> %Det%(size_t %n%)%$$
$rnext $cnext $syntax%%Type% %Det%(CppADvector<%Type%> &%A%)%$$
$tend

$fend 25$$

$head Inclusion$$
The template function $code DetByMinor$$ is defined in the $code CppAD$$
namespace by including 
the file $code Example/DetByMinor.h$$.
It is only intended for example and testing purposes, 
so it is not automatically included by
$xref/CppAD//CppAD.h/$$.

$head Constructor$$
The syntax
$syntax%
	DetByMinor<%Type%> %Det%(size_t %n%)
%$$
constructs the object $italic Det$$ which can be used for 
evaluating the determinant of $italic n$$ by $italic n$$ matrices
using expansion by minors.

$head Evaluation$$
The syntax
$syntax%
	%Type% %Det%(CppADvector<%Type%> &%A%)
%$$
returns the determinant of $italic A$$ using expansion by minors.

$children%
	Example/DetByMinor.cpp
%$$


$head Example$$
The file
$xref/DetByMinor.cpp/$$ 
contains an example and a test of $code DetByMinor.h$$.
It returns true if it succeeds and false otherwise.

$head Source Code$$
The file
$xref/DetByMinor.h/$$ 
contains the source for this template function.


$end
---------------------------------------------------------------------------
$begin DetByMinor.h$$
$spell
	Cpp
	ifndef
	endif
	Det
	const
$$

$index DetByMinor$$
$mindex determinant minor matrix$$
$section Determinant using Expansion by Minors: Source Code$$

$comment This file is in the Example subdirectory$$
$code
# ifndef CppADDetByMinorIncluded
$pre
$$
# define CppADDetByMinorIncluded

$verbatim%Example/DetByMinor.h%0%// BEGIN PROGRAM%// END PROGRAM%1%$$

# endif
$$

$end
---------------------------------------------------------------------------

*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include "DetOfMinor.h"

// BEGIN CppAD namespace
namespace CppAD {

template <class Type>
class DetByMinor {
public:
	DetByMinor(size_t m_) : m(m_) , r(m_ + 1) , c(m_ + 1)
	{
		size_t i;

		// values for r and c that correspond to entire matrix
		for( i = 0; i < m; i++)
		{	r[i] = i+1;
			c[i] = i+1;
		}
		r[m] = 0;
		c[m] = 0;
	}

	inline Type operator()(const CppADvector<Type> &x) const
	{
		return DetOfMinor(x, m, m, r, c); 
	}

private:
	size_t              m;

	// mutable because DetOfMinor modifies and restores these 
	mutable CppADvector<size_t> r;
	mutable CppADvector<size_t> c;
};

} // END CppAD namespace

// END PROGRAM
# endif
