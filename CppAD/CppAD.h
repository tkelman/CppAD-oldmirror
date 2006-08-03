# ifndef CppADCppADIncluded
# define CppADCppADIncluded

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the
                 Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */

// ---------------------------------------------------------------------------
// CppAD general purpose library routines (can be included separately)

# include <CppAD/PowInt.h>
# include <CppAD/LuSolve.h>
# include <CppAD/NearEqual.h>
# include <CppAD/Runge45.h>
# include <CppAD/RombergOne.h>
# include <CppAD/RombergMul.h>
# include <CppAD/Rosen34.h>
# include <CppAD/OdeErrControl.h>
# include <CppAD/OdeGear.h>
# include <CppAD/OdeGearControl.h>
# include <CppAD/Poly.h>
# include <CppAD/SpeedTest.h>
# include <CppAD/CppAD_vector.h>

# include <CppAD/CheckSimpleVector.h>
# include <CppAD/CheckNumericType.h>
# include <CppAD/TrackNewDel.h>
# include <CppAD/local/CppADError.h>
// --------------------------------------------------------------------------
// System routines that can be used by rest of CppAD with out including 

# include <cstddef>
# include <iostream>
# include <complex>
# include <cmath>

// ---------------------------------------------------------------------------
// definitions needed by rest of includes

// definitions that come from the installation
# include <CppAD/config.h>

// definitions that are local to the CppAD include files
# include <CppAD/local/Define.h>

// vectors used with CppAD
# include <CppAD/local/CppADvector.h>

// Declare classes and fucntions that are used before defined
# include <CppAD/local/Declare.h>

// ---------------------------------------------------------------------------
// files containing only implementation details

# include <CppAD/local/AD.h>       // the differentaible object class

// ---------------------------------------------------------------------------
// files containing user interface documentation

# include <CppAD/local/Op.h>      // executes taped operations
# include <CppAD/local/UserAD.h>  // AD class methods available to the user
# include <CppAD/local/ADFun.h>   // ADFun objects

// ---------------------------------------------------------------------------
// library routines that require the rest of CppAD
# include <CppAD/local/LuRatio.h>
# include <CppAD/local/BenderQuad.h>

// undo definitions in Define.h
# include <CppAD/local/Undef.h>   

# endif
