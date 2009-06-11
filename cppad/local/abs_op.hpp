/* $Id$ */
# ifndef CPPAD_ABS_OP_INCLUDED
# define CPPAD_ABS_OP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// BEGIN CppAD namespace
namespace CppAD {

/*!
Forward mode Taylor coefficient of the specfied order result of AbsOp operation.

The C++ source code corresponding to this operation is
\verbatim
	z = abs(x)
\endverbatim
The j-th order Taylor coefficient for the variable with index i is
taylor[ i * nc + j ]. These have already been calculated for all i and j < p
and for i < i_z and j = p. This call will calculate taylor[ i_z * nc + p ].
*/
template <class Base>
inline void forward_abs_op(
	/// Variable index corresponding to result for this operation; i.e. z.
	size_t i_z         ,
	/// Operator argument vector, arg[0] is the variable index
	///  to the argument for this operation; i.e., x.
	const size_t *arg  ,
	/// order of the Taylor coefficient that we are calculating
	size_t p           ,
	/// number of columns in matrix containing all the taylor coefficients
	size_t nc          , 
	/// Taylor coefficients for all variables.
	Base   *taylor     )
{
	static Base zero(0);

	// check number of arguments and results for this operator
	CPPAD_ASSERT_UNKNOWN( NumInd(AbsOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumVar(AbsOp) == 1 );

	// check that the argument comes before the result
	CPPAD_ASSERT_UNKNOWN( arg[0] < i_z );

	// Taylor coefficients corresponding to argument
	Base *x = taylor + arg[0] * nc;

	// Taylor coefficients corresponding to result
	Base *z = taylor + i_z * nc;

	// order that decides positive, negative or zero
	size_t k;
	k = 0;
	while( (k < p) & (x[k] == zero) )
		k++; 

	if( GreaterThanZero(x[k]) )
		z[p]  = x[p];
	else if( LessThanZero(x[k]) )
		z[p] = -x[p]; 
	else	z[p] = zero;
}

/*
------------------------------------------------------------------------------
$begin RevAbsOp$$ $comment CppAD Developer Documentation$$
$spell
	Abs
	Taylor
	const
	inline
	Op
	px
	py
	pz
$$

$index abs, reverse$$
$index reverse, abs$$
$index RevAbsOp$$

$section Reverse Mode Absolute Value Function$$

$head Syntax$$

$syntax%inline void RevAbsOp(size_t %d%,
	const %Base% *%z%, const %Base% *%x%,
	 %Base% *%pz%, %Base% *%px%)%$$

$head Description$$
We are given the partial derivatives for a function
$latex G(z, x)$$ and we wish to compute the partial derivatives for
the function
$latex \[
	H(x) = G [ Z(x) , x ]
\]$$
where $latex Z(x)$$ is defined as the 
$th d$$ order Taylor coefficient row vector for $italic Z$$ as
a function of the corresponding row vector for $italic X$$ 
and
$latex \[
	Z = Abs(X)
\]$$
Note that $italic Z$$ has been used both the original absolute value 
function and for the corresponding mapping of Taylor coefficients.

$head x$$
The vector $italic x$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $italic X$$.


$head z$$
The vector $italic z$$ has length $latex d+1$$ and contains
$th d$$ order Taylor coefficient row vector for $italic Z$$.


$head On Input$$

$subhead px$$
The vector $italic px$$ has length $latex d+1$$ and 
$syntax%%px%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic X$$.

$subhead pz$$
The vector $italic pz$$ has length $latex d+1$$ and 
$syntax%%pz%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic Y$$.

$head On Output$$

$subhead px$$
The vector $italic px$$ has length $latex d+1$$ and 
$syntax%%px%[%j%]%$$ contains the partial for $italic H$$
with respect to the $th j$$ order Taylor coefficient for $italic X$$.

$subhead pz$$
The vector $italic pz$$ has length $latex d+1$$ and 
its contents are no longer specified; i.e., it has
been used for work space.

$end
------------------------------------------------------------------------------
*/
/*!
Reverse mode Partial derivatives of the specfied order for AbsOp.

The C++ source code corresponding to this operation is
\verbatim
	z = abs(x)
\endverbatim
The j-th order Taylor coefficient for the variable with index i is
taylor[ i * nc_taylor + j ]. These have already been calculated for 
all i and j < p.

The j-th order partial derivative with respect to variable with index i is
partial[ i * n_partail + j ]. The variables with indices greater than i_z
have already been removed from the corresponding partials using the chain rule.
This operation will remove the variable with index i_z.
*/
template <class Base>
inline void reverse_abs_op(
	/// Variable index corresponding to result for this operation; i.e. z.
	size_t i_z          ,
	/// Operator argument vector, arg[0] is the variable index
	/// to the argument for this operation; i.e., x.
	const size_t *arg   ,
	/// order of the Taylor coefficient that we are calculating
	size_t p            ,
	/// number of columns in matrix containing all the taylor coefficients
	size_t nc_taylor    , 
	/// Taylor coefficients for all variables.
	const Base  *taylor ,
	/// number of columns in matrix containing all the partials
	size_t nc_partial   ,
	/// Partial derivatives for all variables.
	Base   *partial     )
{	size_t j, k;	
	static Base zero(0);

	// check number of arguments and results for this operator
	CPPAD_ASSERT_UNKNOWN( NumInd(AbsOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumVar(AbsOp) == 1 );

	// check that the argument comes before the result
	CPPAD_ASSERT_UNKNOWN( arg[0] < i_z );

	// Taylor coefficients and partials corresponding to argument
	const Base *x  = taylor  + arg[0] * nc_taylor;
	Base *px       = partial + arg[0] * nc_partial;

	// Taylor coefficients and partials corresponding to result
	Base *pz       = partial +    i_z * nc_partial;


	// order that decides positive, negative or zero
	k = 0;
	while( (k < p) & (x[k] == zero) )
		k++; 

	if( GreaterThanZero(x[k]) )
	{	// partial of z w.r.t x is +1
		for(j = k; j <= p; j++)
			px[j] += pz[j];
	}
	else if( LessThanZero(x[k]) )
	{	// partial of z w.r.t x is -1
		for(j = k; j <= p; j++)
			px[j] -= pz[j];
	}

}

} // END CppAD namespace

# endif
