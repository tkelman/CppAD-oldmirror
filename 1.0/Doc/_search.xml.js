// ------------------------------------------------------- 
// Copyright (C) Bradley M. Bell 2003, All rights reserved 
// ------------------------------------------------------- 
Keyword = 
[
"CppAD  A Package for Differentiation of C++ Algorithms: CppAD-06-05-29   algorithmic automatic derivative version introduction require base type include cppad.h namespace",
"License  Your License for the CppAD Software  ",
"Faq  Frequently Asked Questions and Answers   assignment operator independent bugs reporting comparechange ndebug complex test exception errorhandler variable tape avoid record speed taping math functions inverse matrix forward reverse mode namespace cppadvector preprocessor symbols standard using storage memory disk",
"Install  Installing and Testing CppAD  ",
"InstallUnix  Unix Installation and Testing   cppad free configure prefix postfix getstarted introduction example more speed print forward mode speedtest profile std::vector adolc fadbad boost compile flags",
"GetStarted.cpp  Getting Started Using CppAD   simple example",
"Introduction  An Introduction by Example to Algorithmic Differentiation   ad automatic",
"ExpApx  An Example Exponential Approximation   algorithm",
"ExpApx.hpp  An Example Exponential Approximation Source Code  ",
"ExpApx.cpp  ExpApx: Example and Test   introduction",
"ExpApxSeq  An Example Operation Sequence   symbol",
"ExpApxSeq.cpp  ExpApx: Operation Sequence Verification  ",
"ExpApxFor  An Example Forward Mode Sweep  ",
"ExpApxFor.cpp  ExpApx Forward Mode Verification  ",
"ExpApxRev  An Example Reverse Mode Sweep  ",
"ExpApxRev.cpp  ExpApx Reverse Mode Verification  ",
"ExpApxAD.cpp  ExpApx: Computing Derivatives with CppAD  ",
"Introduction.cpp  Run the CppAD Introduction Routines   example all",
"AD  AD Objects  ",
"Constructor  AD Constructors  ",
"Default  AD Default Constructor  ",
"Default.cpp  Default AD Constructor: Example and Test  ",
"CopyBase  AD From Base Type Constructor  ",
"CopyBase.cpp  AD Constructor From Base Type: Example and Test   convert",
"CopyAD  AD Copy Constructor  ",
"CopyAD.cpp  AD Copy Constructor: Example and Test   object",
"Convert  Conversion and Printing of AD Objects   from",
"Value  Convert From an AD Type to its Base Type  ",
"Value.cpp  Convert From AD to its Base Type: Example and Test   record",
"Integer  Convert From AD to Integer  ",
"Integer.cpp  Convert From AD to Integer: Example and Test  ",
"Output  AD Output Stream Operator   << write",
"Output.cpp  AD Output Operator: Example and Test   <<",
"PrintFor  Printing AD Values During Forward Mode  ",
"PrintFor.cpp  Printing During Forward Mode: Example and Test  ",
"Eq  AD Assignment Operator   = multiple",
"Eq.cpp  AD Assignment Operator: Example and Test  ",
"ADValued  AD Valued Operations and Functions  ",
"Arithmetic  AD Arithmetic Operators and Computed Assignments  ",
"UnaryPlus  AD Unary Plus Operator   +",
"UnaryPlus.cpp  AD Unary Plus Operator: Example and Test  ",
"UnaryMinus  AD Unary Minus Operator   -",
"UnaryMinus.cpp  AD Unary Minus Operator: Example and Test  ",
"Add  AD Binary Addition Operator   +",
"Add.cpp  AD Binary Addition: Example and Test   + plus",
"Sub  AD Binary Subtraction Operator   -",
"Sub.cpp  AD Binary Subtraction: Example and Test   - minus",
"Mul  AD Binary Multiplication Operator   multiply *",
"Mul.cpp  AD Binary Multiplication: Example and Test   * multiply times",
"Div  AD Binary Division Operator   divide /",
"Div.cpp  AD Binary Division: Example and Test   / divide quotient",
"AddEq  AD Computed Assignment Addition Operator   +=",
"AddEq.cpp  AD Computed Assignment Addition: Example and Test   += plus",
"SubEq  AD Computed Assignment Subtraction Operator   -=",
"SubEq.cpp  AD Computed Assignment Subtraction: Example and Test   -= plus add",
"MulEq  AD Computed Assignment Multiplication Operator   multiply *=",
"MulEq.cpp  AD Computed Assignment Multiplication: Example and Test   *= multiply plus add",
"DivEq  AD Computed Assignment Division Operator   divide /=",
"DivEq.cpp  AD Computed Assignment Division: Example and Test   /= divide plus add",
"StdMathUnary  AD Standard Math Unary Functions   acos asin atan cosh exp log log10 sinh sqrt",
"Acos.cpp  The AD acos Function: Example and Test  ",
"Asin.cpp  The AD asin Function: Example and Test  ",
"Atan.cpp  The AD atan Function: Example and Test  ",
"Cos.cpp  The AD cos Function: Example and Test  ",
"Cosh.cpp  The AD cosh Function: Example and Test  ",
"Exp.cpp  The AD exp Function: Example and Test  ",
"Log.cpp  The AD log Function: Example and Test  ",
"Log10.cpp  The AD log10 Function: Example and Test  ",
"Sin.cpp  The AD sin Function: Example and Test  ",
"Sinh.cpp  The AD sinh Function: Example and Test  ",
"Sqrt.cpp  The AD sqrt Function: Example and Test  ",
"Tan.cpp  The AD tan Function: Example and Test  ",
"MathOther  Other AD Math Functions   atan2",
"abs  AD Absolute Value Function   directional derivative",
"Abs.cpp  AD Absolute Value Function: Example and Test   derivative directional",
"atan2  AD Two Argument Inverse Tangent Function  ",
"Atan2.cpp  The AD atan2 Function: Example and Test  ",
"erf  The AD Error Function  ",
"Erf.cpp  The AD erf Function: Example and Test  ",
"pow  The AD Power Function   exponent",
"Pow.cpp  The AD Power Function: Example and Test  ",
"CondExp  AD Conditional Expressions   assign",
"CondExp.cpp  Conditional Expressions: Example and Test  ",
"Discrete  Discrete AD Functions  ",
"Discrete.cpp  Discrete AD Functions: Example and Test  ",
"Piecewise.cpp  Piecewise Linear Interpolation: Example and Test   interpolate",
"BoolValued  Bool Valued Operations and Functions with AD Arguments  ",
"Compare  AD Binary Comparison Operators   < <= > >= == !=",
"Compare.cpp  AD Binary Comparison Operators: Example and Test   < <= > >= == !=",
"NearEqualExt  Compare AD and Base Objects for Nearly Equal   with",
"NearEqualExt.cpp  Compare AD with Base Objects: Example and Test  ",
"BoolFun  AD Boolean Functions   unary binary",
"BoolFun.cpp  AD Boolean Functions: Example and Test  ",
"ParVar  Is an AD Object a Parameter or Variable  ",
"ParVar.cpp  AD Parameter and Variable Functions: Example and Test  ",
"VecAD  AD Vectors that Record Index Operations   tape vecad<base>::reference",
"VecAD.cpp  AD Vectors that Record Index Operations: Example and Test  ",
"ADFun  ADFun Objects  ",
"Independent  Declare Independent Variables and Start Recording   state tape",
"Independent.cpp  Independent and ADFun Constructor: Example and Test  ",
"FunConstruct  Construct an ADFun Object and Stop Recording   tape",
"Dependent  Stop Recording and Store Operation Sequence   adfun tape",
"SeqProperty  ADFun Sequence Properties   domain range parameter use_vecad var_size",
"SeqProperty.cpp  ADFun Sequence Properties: Example and Test   domain range parameter size_var",
"FunEval  Evaluate ADFun Functions, Derivatives, and Sparsity Patterns  ",
"Forward  Forward Mode  ",
"ForwardZero  Zero Order Forward Mode: Function Values  ",
"ForwardOne  First Order Forward Mode: Derivative Values  ",
"ForwardAny  Any Order Forward Mode   derivative calculate",
"size_taylor  Number Taylor Coefficients, Per Variable, Currently Stored  ",
"CompareChange  Comparison Changes During Zero Order Forward Mode   adfun ndebug",
"CompareChange.cpp  CompareChange and Re-Tape: Example and Test  ",
"capacity_taylor  Controlling Taylor Coefficients Memory Allocation   forward",
"Forward.cpp  Forward Mode: Example and Test  ",
"Reverse  Reverse Mode  ",
"ReverseOne  First Order Reverse Mode: Derivative Values  ",
"ReverseAny  Any Order Reverse Mode   derivative calculate",
"ReverseOne.cpp  First Order Reverse Mode: Example and Test  ",
"ReverseAny.cpp  Reverse Mode (any Order): Example and Test  ",
"HesTimesDir.cpp  Hessian Times Direction: Example and Test  ",
"Sparse  Calculating Sparsity Patterns  ",
"ForSparseJac  Jacobian Sparsity Pattern: Forward Mode  ",
"ForSparseJac.cpp  Forward Mode Jacobian Sparsity: Example and Test  ",
"RevSparseJac  Jacobian Sparsity Pattern: Reverse Mode  ",
"RevSparseJac.cpp  Reverse Mode Jacobian Sparsity: Example and Test  ",
"RevSparseHes  Hessian Sparsity Pattern: Reverse Mode  ",
"RevSparseHes.cpp  Reverse Mode Hessian Sparsity: Example and Test  ",
"Drivers  First and Second Derivatives: Easy Drivers   forward reverse",
"Jacobian  Jacobian: Driver Routine   first derivative",
"Jacobian.cpp  Jacobian: Example and Test  ",
"ForOne  First Order Partial Derivative: Driver Routine   easy",
"ForOne.cpp  First Order Partial Driver: Example and Test  ",
"RevOne  First Order Derivative: Driver Routine   easy",
"RevOne.cpp  First Order Derivative Driver: Example and Test  ",
"Hessian  Hessian: Easy Driver   second derivative",
"Hessian.cpp  Hessian: Example and Test  ",
"ForTwo  Forward Mode Second Partial Derivative Driver   order easy",
"ForTwo.cpp  Subset of Second Order Partials: Example and Test  ",
"RevTwo  Reverse Mode Second Partial Derivative Driver   order easy",
"RevTwo.cpp  Second Partials Reverse Driver: Example and Test  ",
"FunCheck  Check an ADFun Sequence of Operations  ",
"FunCheck.cpp  ADFun Check and Re-Tape: Example and Test   dependent",
"FunDeprecated  ADFun Object Deprecated Member Functions   size order memory taylor_size",
"library  The CppAD General Purpose Library   numerical c++ template concept",
"ErrorHandler  Replacing the CppAD Error Handler   replace assert exception",
"ErrorHandler.cpp  Replacing The CppAD Error Handler: Example and Test  ",
"NearEqual  Determine if Two Values Are Nearly Equal   absolute difference relative exercise",
"Near_Equal.cpp  NearEqual Function: Example and Test  ",
"SpeedTest  Run One Speed Test  ",
"SpeedExample.cpp  Example Use of SpeedTest  ",
"NumericType  Definition of a Numeric Type   default constructor int copy exercise",
"NumericType.cpp  The NumericType: Example and Test  ",
"CheckNumericType  Check NumericType Class Concept  ",
"CheckNumericType.cpp  The CheckNumericType Function: Example and Test  ",
"SimpleVector  Definition of a Simple Vector   class template default constructor size copy element destructor assignment resize value_type [] exercise ndebug",
"SimpleVector.cpp  Simple Vector Template Class: Example and Test  ",
"CheckSimpleVector  Check Simple Vector Concept  ",
"CheckSimpleVector.cpp  The CheckSimpleVector Function: Example and Test  ",
"PowInt  The Integer Power Function   exponent",
"PowInt.cpp  The Integer Power Function: Example and Test  ",
"Poly  Evaluate a Polynomial or its Derivative   template",
"Poly.cpp  Polynomial Evaluation: Example and Test  ",
"LuDetAndSolve  Compute Determinants and Solve Equations by LU Factorization   matrix linear",
"LuSolve  Compute Determinant and Solve Linear Equations  ",
"LuSolve.h  LuSolve Source Code   determinant linear equation matrix factor",
"LuSolve.cpp  LuSolve With Complex Arguments: Example and Test  ",
"LuFactor  LU Factorization of A Square Matrix   linear equation determinant solve",
"LuFactor.h  LuFactor Source Code   matrix",
"LuFactor.cpp  LuFactor: Example and Test  ",
"LuInvert  Invert an LU Factored Equation   linear",
"LuInvert.h  LuInvert Source Code   matrix",
"LuInvert.cpp  LuInvert: Example and Test  ",
"RombergOne  One DimensionalRomberg Integration   integrate",
"RombergOne.cpp  One Dimensional Romberg Integration: Example and Test  ",
"RombergMul  Multi-dimensional Romberg Integration   integrate",
"RombergMul.cpp  One Dimensional Romberg Integration: Example and Test  ",
"Runge45  An Embedded 4th and 5th Order Runge-Kutta ODE Solver   differential equation",
"Runge45.cpp  Runge45: Example and Test  ",
"Rosen34  A 3rd and 4th Order Rosenbrock ODE Solver   stiff differential equation",
"Rosen34.cpp  Rosen34: Example and Test  ",
"OdeErrControl  An Error Controller for ODE Solvers   differential equation",
"OdeErrControl.cpp  OdeErrControl: Example and Test  ",
"OdeErrMaxabs.cpp  OdeErrControl: Example and Test Using Maxabs Argument  ",
"OdeGear  An Arbitrary Order Gear Method   stiff differential equation",
"OdeGear.cpp  OdeGear: Example and Test  ",
"OdeGearControl  An Error Controller for Gear's Ode Solvers   differential equation",
"OdeGearControl.cpp  OdeGearControl: Example and Test  ",
"BenderQuad  Computing Jacobian and Hessian of Bender's Reduced Objective  ",
"BenderQuad.cpp  BenderQuad: Example and Test  ",
"LuRatio  LU Factorization of A Square Matrix and Stability Calculation   linear equation determinant solve",
"LuRatio.cpp  LuRatio: Example and Test  ",
"CppAD_vector  The CppAD::vector Template Class   assignment [] push_back vectorbool exercise",
"CppAD_vector.cpp  CppAD::vector Template Class: Example and Test  ",
"vectorBool.cpp  CppAD::vectorBool Class: Example and Test  ",
"TrackNewDel  Routines That Track Use of New and Delete   memory tracknewvec ndebug trackdelvec trackextend trackcount",
"TrackNewDel.cpp  Tracking Use of New and Delete: Example and Test  ",
"Example  Examples  ",
"General  General Examples   realistic",
"ExampleUtility  Utility Routines used by CppAD Examples  ",
"ListAllExamples  List of All the CppAD Examples  ",
"Interface2C.cpp  Interfacing to C: Example and Test   difference central",
"JacMinorDet.cpp  Gradient of Determinant Using Expansion by Minors: Example and Test  ",
"JacLuDet.cpp  Gradient of Determinant Using Lu Factorization: Example and Test  ",
"HesMinorDet.cpp  Gradient of Determinant Using Expansion by Minors: Example and Test  ",
"HesLuDet.cpp  Gradient of Determinant Using LU Factorization: Example and Test  ",
"OdeStiff.cpp  A Stiff Ode: Example and Test  ",
"OdeTaylor.cpp  Taylor's Method Ode Solver: An Example and Test  ",
"ComplexPoly.cpp  Complex Polynomial: Example and Test  ",
"MulTape  Using Multiple Tapes   derivative calculation",
"MulTape.cpp  Multiple Tapes: Example and Test  ",
"Example.cpp  Program That Runs all the CppAD Example  ",
"DetOfMinor  Compute the Determinant of a Minor   matrix",
"DetOfMinor.h  Compute the Determinant of a Minor: Source Code   matrix",
"DetOfMinor.cpp  Determinant of a Minor: Example and Test   by minors",
"DetByMinor  Determinant Using Expansion by Minors  ",
"DetByMinor.h  Determinant using Expansion by Minors: Source Code   matrix",
"DetByMinor.cpp  Determinant Using Expansion by Minors: Example and Test  ",
"DetByLu  Determinant Using Lu Factorization  ",
"DetByLu.h  Determinant using Lu Factorization: Source Code   complex",
"DetByLu.cpp  Determinant Using Lu Factorization: Example and Test   minors",
"LuVecAD  Lu Factor and Solve with Recorded Pivoting   linear equation determinant",
"LuVecADOk.cpp  Lu Factor and Solve With Recorded Pivoting: Example and Test  ",
"CppADvector  Choosing The CppADvector Template Class   test",
"Appendix  Appendix  ",
"Theory  The Theory of Derivative Calculations  ",
"ForwardTheory  The Theory of Forward Mode  ",
"ExpForward  Exponential Function Forward Taylor Polynomial Theory  ",
"LogForward  Logarithm Function Forward Taylor Polynomial Theory  ",
"SqrtForward  Square Root Function Forward Taylor Polynomial Theory  ",
"SinCosForward  Trigonometric and Hyperbolic Sine and Cosine Forward Theory   sinh cosh",
"AtanForward  Arctangent Function Forward Taylor Polynomial Theory  ",
"AsinForward  Arcsine Function Forward Taylor Polynomial Theory  ",
"AcosForward  Arccosine Function Forward Taylor Polynomial Theory  ",
"ReverseTheory  The Theory of Reverse Mode  ",
"ExpReverse  Exponential Function Reverse Mode Theory  ",
"LogReverse  Logarithm Function Reverse Mode Theory  ",
"SqrtReverse  Square Root Function Reverse Mode Theory  ",
"SinCosReverse  Trigonometric and Hyperbolic Sine and Cosine Reverse Theory   sinh cosh",
"AtanReverse  Arctangent Function Reverse Mode Theory  ",
"AsinReverse  Arcsine Function Reverse Mode Theory  ",
"AcosReverse  Arccosine Function Reverse Mode Theory  ",
"ReverseIdentity  An Important Reverse Mode Identity  ",
"Speed  Routines That Speed Test CppAD  ",
"Speed.cpp  Run All The Speed Tests  ",
"DetMinorSpeed.cpp  Determinant Using Expansion by Minors: Speed Test  ",
"DetLuSpeed.cpp  Determinant by Lu Factor and Solve: Speed Test  ",
"OdeSpeed.cpp  ODE Solver: Speed Test   ordinary differential equation tape",
"LuSolveSpeed.cpp  Speed Test Lu Factor and Solve: Example and Test  ",
"Adolc  Routines for Comparing CppAD with Adolc   compare",
"SpeedAdolc.cpp  Run All The Adolc Speed Comparison Tests  ",
"ExampleAdolc.cpp  Compare Adolc and CppAD Results  ",
"DetMinorAdolc.cpp  Adolc & CppAD Gradient of Determinant by Minors   speed expand",
"DetLuAdolc.cpp  Adolc & CppAD Gradient of Determinant by Lu Factorization   speed",
"Fadbad  Routines for Comparing CppAD with Fadbad   compare",
"SpeedFadbad.cpp  Run All The Fadbad Speed Comparison Tests  ",
"ExampleFadbad.cpp  Compare Fadbad and CppAD Results  ",
"DetMinorFadbad.cpp  Fadbad & CppAD Gradient of Determinant by Minors   speed expand",
"DetLuFadbad.cpp  Fadbad & CppAD Gradient of Determinant by Lu Factorization   speed",
"glossary  Glossary   ad function of base type sequence dependent variable elementary vector independent operation atomic parameter sparsity pattern efficient tape state empty recording taylor coefficient",
"Bib  Bibliography  ",
"Bugs  Know Bugs and Problems Using CppAD   gcc 3.4.4",
"WishList  The CppAD Wish List   new features atan2 condexp multiple tape",
"WhatsNew06  Changes and Additions to CppAD   aclocal",
"WhatsNew05  Changes and Additions to CppAD During 2005  ",
"WhatsNew04  Changes and Additions to CppAD During 2004  ",
"WhatsNew03  Changes and Additions to CppAD During 2003  "
]

var MaxList = 100;
var Choice  = "";
var Nstring = -1;
var Nkeyword = Keyword.length;
Initialize();

function Initialize()
{
	var i;
	var line;
	for(i = 0; (i < Nkeyword) && (i < MaxList) ; i++)
	{
		line       = Keyword[i].split(/\s+/)
		line[0]    = line[0].toUpperCase();
		line       = line.join(" ");
		Keyword[i] = line;
	}
	Search();
	document.search.string.focus();
}
function Search()
{
	var string  = document.search.string.value;
	if( Nstring == string.length )
		return;
	Nstring     = string.length;

	var word    = string.match(/\S+/g);
	var nword   = 0;
	if(word != null )
		nword   = word.length;

	var pattern = new Array(nword);
	for(var j = 0; j < nword; j++)
		pattern[j] = new RegExp(word[j], "i");

	var nlist = 0;
	var list  = "";
	Choice    = "";

	for(i = 0; (i < Nkeyword) && (nlist < MaxList) ; i++)
	{
		var match = true;
		for(j = 0; j < nword; j++)
			match = match && pattern[j].test(Keyword[i]);

		if( match )
		{
			line     = Keyword[i].split(/\s+/);

			if( Choice == "" )
				Choice = line[0];

			line  = line.join(" ");
			list  = list + line + "\n";
			nlist = nlist + 1;
		}
	}
	document.search.choice.value  = Choice.toLowerCase();
	document.search.list.value    = list;
}
function choose()
{
parent.location = document.search.choice.value.toLowerCase() + ".xml";
}
