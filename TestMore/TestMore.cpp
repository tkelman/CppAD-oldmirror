/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */

// system include files used for I/O
# include <iostream>

// AD<Type> template class definition
# include <CppAD/CppAD.h>

// various examples / tests
extern bool Abs(void);
extern bool Acos(void);
extern bool Add(void);
extern bool AddEq(void);
extern bool AddZero(void);
extern bool Asin(void);
extern bool Atan(void);
extern bool Atan2(void);
extern bool Compare(void);
extern bool CompareChange(void);
extern bool CondExp(void);
extern bool CondExpAD(void);
extern bool Copy(void);
extern bool Cos(void);
extern bool Cosh(void);
extern bool Div(void);
extern bool DivEq(void);
extern bool DivZeroOne(void);
extern bool Erf(void);
extern bool Exp(void);
extern bool ForHess(void);
extern bool ForSparseJac(void);
extern bool Forward(void);
extern bool FromBase(void);
extern bool Log(void);
extern bool Log10(void);
extern bool Mul(void);
extern bool MulEq(void);
extern bool MulZeroOne(void);
extern bool NearEqualExt(void);
extern bool Neg(void);
extern bool OdeErrControl(void);
extern bool Poly(void);
extern bool Pow(void);
extern bool PowInt(void);
extern bool Reverse(void);
extern bool RevSparseHes(void);
extern bool RevSparseJac(void);
extern bool RevTwo(void);
extern bool Rosen34(void);
extern bool Runge45(void);
extern bool SimpleVector(void);
extern bool Sin(void);
extern bool SinCos(void);
extern bool Sinh(void);
extern bool SparseVecAD(void);
extern bool Sqrt(void);
extern bool Sub(void);
extern bool SubEq(void);
extern bool SubZero(void);
extern bool Value(void);
extern bool VecAD(void);
extern bool VecADPar(void);
extern bool VecUnary(void);

namespace {
	// function that runs one test
	bool Run(bool TestOk(void), const char *name)
	{	bool ok = true;
		using namespace std;
	
		ok &= TestOk();
	
		if( ok )
			std::cout << "Ok:    " << name << std::endl;
		else	std::cout << "Error: " << name << std::endl;
	
		return ok;
	}
	// check for memory leak in previous calculations
	bool TrackCount(void)
	{	bool ok = (CppADTrackCount() == 0);
		return ok;
	}
}

// main program that runs all the tests
int main(void)
{	bool ok = true;
	using namespace std;

	// This comment is used by OneTest 

	ok &= Run( Abs,             "Abs"            );
	ok &= Run( Acos,            "Acos"           );
	ok &= Run( Add,             "Add"            );
	ok &= Run( AddEq,           "AddEq"          );
	ok &= Run( AddZero,         "AddZero"        );
	ok &= Run( Asin,            "Asin"           );
	ok &= Run( Atan,            "Atan"           );
	ok &= Run( Atan2,           "Atan2"          );
	ok &= Run( Compare,         "Compare"        );
	ok &= Run( CompareChange,   "CompareChange"  );
	ok &= Run( CondExp,         "CondExp"        );
	ok &= Run( CondExpAD,       "CondExpAD"      );
	ok &= Run( Copy,            "Copy"           );
	ok &= Run( Cos,             "Cos"            );
	ok &= Run( Cosh,            "Cosh"           );
	ok &= Run( Div,             "Div"            );
	ok &= Run( DivEq,           "DivEq"          );
	ok &= Run( DivZeroOne,      "DivZeroOne"     );
	ok &= Run( Erf,             "Erf"            );
	ok &= Run( Exp,             "Exp"            );
	ok &= Run( ForHess,         "ForHess"        );
	ok &= Run( ForSparseJac,    "ForSparseJac"   );
	ok &= Run( Forward,         "Forward"        );
	ok &= Run( FromBase,        "FromBase"       );
	ok &= Run( Log,             "Log"            );
	ok &= Run( Log10,           "Log10"          );
	ok &= Run( Mul,             "Mul"            );
	ok &= Run( MulEq,           "MulEq"          );
	ok &= Run( MulZeroOne,      "MulZeroOne"     );
	ok &= Run( NearEqualExt,    "NearEqualExt"   );
	ok &= Run( Neg,             "Neg"            );
	ok &= Run( OdeErrControl,   "OdeErrControl"  );
	ok &= Run( Poly,            "Poly"           );
	ok &= Run( Pow,             "Pow"            );
	ok &= Run( PowInt,          "PowInt"         );
	ok &= Run( Reverse,         "Reverse"        );
	ok &= Run( RevSparseHes,    "RevSparseHes"   );
	ok &= Run( RevSparseJac,    "RevSparseJac"   );
	ok &= Run( RevTwo,          "RevTwo"         );
	ok &= Run( Rosen34,         "Rosen34"        );
	ok &= Run( Runge45,         "Runge45"        );
	ok &= Run( SimpleVector,    "SimpleVector"   );
	ok &= Run( Sin,             "Sin"            );
	ok &= Run( SinCos,          "SinCos"         );
	ok &= Run( Sinh,            "Sinh"           );
	ok &= Run( SparseVecAD,     "SparseVecAD"    );
	ok &= Run( Sqrt,            "Sqrt"           );
	ok &= Run( Sub,             "Sub"            );
	ok &= Run( SubEq,           "SubEq"          );
	ok &= Run( SubZero,         "SubZero"        );
	ok &= Run( Value,           "Value"          );
	ok &= Run( VecAD,           "VecAD"          );
	ok &= Run( VecADPar,        "VecADPar"       );
	ok &= Run( VecUnary,        "VecUnary"       );


	// check for memory leak in previous calculations
	if( CppADTrackCount() != 0 )
	{	ok = false;
		cout << "Error: memroy leak detected" << endl;
	}
	if( ok )
		cout << "All the tests passed." << endl;
	else	cout << "At least one test failed." << endl;

	return static_cast<int>( ! ok );
}
// END PROGRAM
