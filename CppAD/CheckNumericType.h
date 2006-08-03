# ifndef CppADCheckNumericTypeIncluded
# define CppADCheckNumericTypeIncluded

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */
/*
$begin CheckNumericType$$
$spell
	CppAD
$$

$section Check NumericType Class Concept$$

$index numeric, check$$
$index check, numeric$$
$index concept, check numeric$$

$head Syntax$$
$code # include <CppAD/CheckNumericType.h>$$
$pre
$$
$syntax%CheckNumericType<%NumericType%>()%$$


$head Purpose$$
The syntax 
$syntax%
	CheckNumericType<%NumericType%>()
%$$
preforms compile and run time checks that the type specified
by $italic NumericType$$ satisfies all the requirements for 
a $xref/NumericType/$$ class. 
If a requirement is not satisfied,
a an error message makes it clear what condition is not satisfied.

$head Include$$
The file $code CppAD/CheckNumericType.h$$ is included by $code CppAD/CppAD.h$$
but it can also be included separately with out the rest
if the CppAD include files.

$head Example$$
$children%
	Example/CheckNumericType.cpp
%$$
The file $xref/CheckNumericType.cpp/$$
contains an example and test of this function. 
It returns true, if it succeeds an false otherwise.
The comments in this example suggest a way to change the example
so an error message occurs.

$end
---------------------------------------------------------------------------
*/



namespace CppAD {

# ifdef NDEBUG
	template <class NumericType>
	inline void CheckNumericType(void)
	{ }
# else
	template <class NumericType>
	NumericType CheckNumericType(void)
	{	// only need execute once per value NumericType type
		static bool runOnce = false;
		if( runOnce )
			return NumericType(0);
		runOnce = true;

		/*
		contructors
		*/
		NumericType check_NumericType_default_constructor;
		NumericType check_NumericType_constructor_from_int(1);

		const NumericType x(1);

		NumericType check_NumericType_copy_constructor(x);

		// assignment
		NumericType check_NumericType_assignment;
		check_NumericType_assignment = x;

		/*
		unary operators
		*/
		const NumericType check_NumericType_unary_plus(1);
		NumericType check_NumericType_unary_plus_result = 
			+ check_NumericType_unary_plus;

		const NumericType check_NumericType_unary_minus(1);
		NumericType check_NumericType_unary_minus_result = 
			- check_NumericType_unary_minus;

		/*
		binary operators
		*/
		const NumericType check_NumericType_binary_addition(1);
		NumericType check_NumericType_binary_addition_result = 
			check_NumericType_binary_addition + x;

		const NumericType check_NumericType_binary_subtraction(1);
		NumericType check_NumericType_binary_subtraction_result =
			check_NumericType_binary_subtraction - x;

		const NumericType check_NumericType_binary_multiplication(1);
		NumericType check_NumericType_binary_multiplication_result =
			check_NumericType_binary_multiplication * x;

		const NumericType check_NumericType_binary_division(1);
		NumericType check_NumericType_binary_division_result =
			check_NumericType_binary_division / x;

		/*
		computed assignment operators
		*/
		NumericType 
		check_NumericType_computed_assignment_addition(1);
		check_NumericType_computed_assignment_addition += x;

		NumericType 
		check_NumericType_computed_assignment_subtraction(1);
		check_NumericType_computed_assignment_subtraction -= x;

		NumericType 
		check_NumericType_computed_assignment_multiplication(1);
		check_NumericType_computed_assignment_multiplication *= x;

		NumericType 
		check_NumericType_computed_assignment_division(1);
		check_NumericType_computed_assignment_division /= x;

		/* 
		use all values so as to avoid warnings
		*/
		check_NumericType_default_constructor = x;
		return
			+ check_NumericType_default_constructor
			+ check_NumericType_constructor_from_int
			+ check_NumericType_copy_constructor
			+ check_NumericType_assignment
			+ check_NumericType_unary_plus_result
			+ check_NumericType_unary_minus_result
			+ check_NumericType_binary_addition_result
			+ check_NumericType_binary_subtraction_result
			+ check_NumericType_binary_multiplication_result
			+ check_NumericType_binary_division_result
			+ check_NumericType_computed_assignment_addition
			+ check_NumericType_computed_assignment_subtraction
			+ check_NumericType_computed_assignment_multiplication
			+ check_NumericType_computed_assignment_division
		;
	}
# endif

} // end namespace CppAD

# endif
