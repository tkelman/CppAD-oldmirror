# ifndef CppADLuVecADIncluded
# define CppADLuVecADIncluded

// BEGIN SHORT COPYRIGHT
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */
// END SHORT COPYRIGHT

# include <CppAD/CppAD.h>

namespace CppAD {
	extern CppAD::AD<double> LuVecAD(
		size_t                           n,
		size_t                           m,
		CppAD::VecAD<double>             &Matrix,
		CppAD::VecAD<double>             &Rhs,
		CppAD::VecAD<double>             &Result,
		CppAD::AD<double>                &logdet
	);
}

# endif
