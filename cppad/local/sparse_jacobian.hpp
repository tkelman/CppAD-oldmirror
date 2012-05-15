/* $Id$ */
# ifndef CPPAD_SPARSE_JACOBIAN_INCLUDED
# define CPPAD_SPARSE_JACOBIAN_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sparse_jacobian$$
$spell
	valarray
	std
	CppAD
	Bool
	jac
	Jacobian
	const
	Taylor
$$

$section Sparse Jacobian: Easy Driver$$
$index SparseJacobian$$
$index jacobian, sparse$$

$head Syntax$$
$icode%jac% = %f%.SparseJacobian(%x%)
%$$
$icode%jac% = %f%.SparseJacobian(%x%, %p%)
%$$
$icode%n_sweep% = f%.SparseJacobian(%x%, %p%, %r%, %c%, %jac%)%$$

$head Purpose$$
We use $latex n$$ for the $cref/domain/seq_property/Domain/$$ size,
and $latex m$$ for the $cref/range/seq_property/Range/$$ size of $icode f$$.
We use $latex F : \R^n \rightarrow \R^m$$ do denote the
$cref/AD function/glossary/AD Function/$$
corresponding to $icode f$$. 
The syntax above sets $icode jac$$ to the Jacobian 
$latex \[
	jac = F^{(1)} (x) 
\] $$
This routine takes advantage of the sparsity of the Jacobian
in order to reduce the amount of computation necessary.
If $icode r$$ and $icode c$$ are present, it also takes
advantage of the reduced set of elements of the Jacobian that
need to be computed.
One can use speed tests (e.g. $cref speed_test$$)
to verify that results are computed faster
than when using the routine $cref Jacobian$$.

$head f$$
The object $icode f$$ has prototype
$codei%
	ADFun<%Base%> %f%
%$$
Note that the $cref ADFun$$ object $icode f$$ is not $code const$$
(see $cref/Uses Forward/sparse_jacobian/Uses Forward/$$ below).

$head x$$
The argument $icode x$$ has prototype
$codei%
	const %VectorBase%& %x%
%$$
(see $cref/VectorBase/sparse_jacobian/VectorBase/$$ below)
and its size 
must be equal to $icode n$$, the dimension of the
$cref/domain/seq_property/Domain/$$ space for $icode f$$.
It specifies
that point at which to evaluate the Jacobian.

$head p$$
The argument $icode p$$ is optional and has prototype
$codei%
	const %VectorSet%& %p%
%$$
(see $cref/VectorSet/sparse_jacobian/VectorSet/$$ below).
If it has elements of type $code bool$$,
its size is $latex m * n$$.
If it has elements of type $code std::set<size_t>$$,
its size is $latex m$$ and all its set elements are between
zero and $latex n - 1$$.
It specifies a 
$cref/sparsity pattern/glossary/Sparsity Pattern/$$ 
for the Jacobian $latex F^{(1)} (x)$$.
$pre

$$
If this sparsity pattern does not change between calls to 
$codei SparseJacobian$$, it should be faster to calculate $icode p$$ once 
(using $cref ForSparseJac$$ or $cref/RevSparseJac/$$)
and then pass $icode p$$ to $codei SparseJacobian$$.
In addition,
if you specify $icode p$$, CppAD will use the same
type of sparsity representation 
(vectors of $code bool$$ or vectors of $code std::set<size_t>$$)
for its internal calculations.
Otherwise, the representation
for the internal calculations is unspecified.

$head r, c$$
The arguments $icode r$$ and $icode c$$ are optional and have prototype
$codei%
	const %VectorSize%& %r%
	const %VectorSize%& %c%
%$$
(see $cref/VectorSize/sparse_jacobian/VectorSize/$$ below).
They specify which rows and columns of $latex F^{(1)} (x)$$ are
returned and in which order.
We use $latex K$$ to denote the value $icode%jac%.size()%$$.
For $latex k = 0 , \ldots , K-1$$, it must hold that
$latex r[k] < m$$ and $latex c[k] < n$$.
Furthermore,
the pair $latex (r,c)$$  must be in either row major or column major order:

$subhead Row Major Order$$
Row major order is defined by the following conditions:
$icode%r%.size() == %K%$$,
$icode%c%.size() == %K%+1$$,
$latex c[K] = n$$,
for $latex k = 1 , \ldots , K-1$$,
$latex c[k-1] \leq c[k]$$ and if $latex c[k-1] = c[k]$$ then
$latex r[k-1] < r[k]$$.
In this case forward mode will be used to compute the Jacobian
with multiple columns computed for each forward sweep.

$subhead Column Major Order$$
Column major order is defined by the following condition:
$icode%r%.size() == %K%+1$$,
$icode%c%.size() == %K%$$,
$latex r[K] = m$$,
for $latex k = 1 , \ldots , K-1$$,
$latex r[k-1] \leq r[k]$$ and if $latex r[k-1] = r[k]$$ then
$latex c[k-1] < c[k]$$.
In this case reverse mode will be used to compute the Jacobian
with multiple rows computed for each reverse sweep.

$head jac$$
The result $icode jac$$ has prototype
$codei%
	%VectorBase%& %jac%
%$$
In the case where the arguments $icode r$$ and $icode c$$ are not present,
the size of $icode jac$$ is $latex m * n$$ and
$latex \[
	jac [ i * n + j ] = \D{ F_i }{ x_j } (x)
\] $$
$pre

$$
In the case where the arguments $icode r$$ and $icode c$$ are present,
we use $latex K$$ to denote the size of $icode jac$$. 
The input value of its elements does not matter.
Upon return, for $latex k = 0 , \ldots , K - 1$$,
$latex \[
	jac [ k ] = \D{ F_i }{ x_j } (x)
	\; , \;
	\; {\rm where} \;
	i = r[k]
	\; {\rm and } \;
	j = c[k]
\] $$

$head n_sweep$$
The return value $icode n_sweep$$ has prototype
$codei%
	size_t %n_sweep%
%$$
If row major (column major) order is used for $codei%(%r%, %c%)%$$,
$icode n_sweep$$ is the number of forward (reverse) sweeps used to compute the
requested Jacobian values. 
This is proportional to the total work that $code SparseJacobian$$ does, 
not counting the zero order forward sweep, 
or the work to combine multiple columns (rows) into a single sweep.

$head VectorBase$$
The type $icode VectorBase$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$icode Base$$.
The routine $cref CheckSimpleVector$$ will generate an error message
if this is not the case.

$head VectorSet$$
The type $icode VectorSet$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$code bool$$ or $code std::set<size_t>$$;
see $cref/sparsity pattern/glossary/Sparsity Pattern/$$ for a discussion
of the difference.
The routine $cref CheckSimpleVector$$ will generate an error message
if this is not the case.

$head VectorSize$$
The type $icode VectorSize$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$code size_t$$.
The routine $cref CheckSimpleVector$$ will generate an error message
if this is not the case.

$subhead Restrictions$$
If $icode VectorSet$$ has elements of $code std::set<size_t>$$,
then $icode%p%[%i%]%$$ must return a reference (not a copy) to the 
corresponding set.
According to section 26.3.2.3 of the 1998 C++ standard,
$code std::valarray< std::set<size_t> >$$ does not satisfy
this condition. 

$head Uses Forward$$
After each call to $cref Forward$$,
the object $icode f$$ contains the corresponding 
$cref/Taylor coefficients/glossary/Taylor Coefficient/$$.
After $code SparseJacobian$$,
the previous calls to $cref Forward$$ are undefined.

$head Example$$
$children%
	example/sparse_jacobian.cpp
%$$
The routine
$cref sparse_jacobian.cpp$$
is examples and tests of $code sparse_jacobian$$.
It return $code true$$, if it succeeds and $code false$$ otherwise.

$end
==============================================================================
*/
# include <cppad/local/std_set.hpp>

CPPAD_BEGIN_NAMESPACE
/*!
\file sparse_jacobian.hpp
Sparse Jacobian driver routine and helper functions.
*/
// ===========================================================================
/*!
Nom member helper function that deteremine if row or column major order.

\tparam VectorSize_t
is a simple vector class with elements of type \c size_t

\param major
is the vector that varies first in the ordering being checked for
(size K).

\param minor
is the vector that varies second in the ordering being checked for
(size K+1).

\return
The return value is true if for all k
\code
	minor[k] <= minor[k+1]
	if( minor[k] == minor[k+1] )
		major[k] < major[k+1]
\endcode
*/
template <class VectorSize>
bool is_major_minor_order(const VectorSize& major, const VectorSize& minor)
{	bool match = true;

	size_t k, K = major.size();
	CPPAD_ASSERT_UNKNOWN( K+1 == minor.size() ); 

	for(k = 1; k < K; k++)
	{	match &= minor[k-1] <= minor[k];
		if( minor[k-1] == minor[k] )
			match &= major[k-1] < major[k];
	} 
	return match;
}
// ===========================================================================
/*!
Private helper function SparseJacobianForward(x, p_transpose, r, c, jac).

All descriptions in the public function SparseJacobian(x, p, r, c, jac) apply.

\param x
See \c SparseJacobian(x, p, r, c, jac).

\param p_transpose
Sparsity pattern for the transpose of the Jacobian;
see \c SparseJacobian(x, p, r, c, jac).
Note that we do not change the values in \c p,
but is not \c const because we use its iterator facility.

\param r
See \c SparseJacobian(x, p, r, c, jac)
with the additional restriction that \c r , \c c are in row major order.

\param c
See \c SparseJacobian(x, p, r, c, jac).
with the additional restriction that \c r , \c c are in row major order.

\param jac
See \c SparseJacobian(x, p, r, c, jac).

\return
See \c SparseJacobian(x, p, r, c, jac).
*/
template<class Base>
template <class VectorBase, class VectorSet, class VectorSize>
size_t ADFun<Base>::SparseJacobianForward(
	const VectorBase&  x               ,
	VectorSet&         p_transpose     ,
	const VectorSize&  r               ,
	const VectorSize&  c               ,
	VectorBase&        jac             )
{
	using   CppAD::vectorBool;
	size_t i, j, k, ell;

	size_t m = Range();
	size_t n = Domain();

	// some values
	const Base zero(0);
	const Base one(1);

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CPPAD_ASSERT_UNKNOWN( x.size() == n );
	CPPAD_ASSERT_UNKNOWN( p_transpose.n_set() ==  n );
	CPPAD_ASSERT_UNKNOWN( p_transpose.end() ==  m );

	// number of components of Jacobian that are required
	size_t K = jac.size();
	CPPAD_ASSERT_UNKNOWN( r.size() == K );
	CPPAD_ASSERT_UNKNOWN( c.size() == K+1 );
	CPPAD_ASSERT_UNKNOWN( c[K] == n );
	CPPAD_ASSERT_UNKNOWN( is_major_minor_order(r, c) );

	// Point at which we are evaluating the Jacobian
	Forward(0, x);

	// mapping from row number to set of columns required
	VectorSet require;
	require.resize(m, n);
	k = 0;
	while( k < K )
	{	require.add_element(r[k], c[k]);
		k++;
	}

	// initial coloring
	VectorSize color(n);
	for(j = 0; j < n; j++)
		color[j] = j;

	// See GreedyPartialD2Coloring Algorithm Section 3.6.2 of
	// Graph Coloring in Optimization Revisited by
	// Assefaw Gebremedhin, Fredrik Maane, Alex Pothen
	vectorBool forbidden(n);
	for(j = 1; j < n; j++)
	{
		// initial all colors as ok for this column
		// (value of forbidden for ell > j does not matter)
		for(ell = 0; ell <= j; ell++)
			forbidden[ell] = false;

		// for each row that is non-zero for this column
		p_transpose.begin(j);
		i = p_transpose.next_element();
		while( i != p_transpose.end() )
		{	// for each column that we require a value for this row
			require.begin(i);
			ell = require.next_element();
			while( ell != require.end() )
			{	// if this is not the same column, forbid its color
				if( ell < j )
					forbidden[ color[ell] ] = true;
				ell = require.next_element();
			}
			i = p_transpose.next_element();
		}
		ell = 0;
		while( forbidden[ell] )
		{	ell++;
			CPPAD_ASSERT_UNKNOWN( ell <= j );
		}
		color[j] = ell;
	}
	size_t n_color = 1;
	for(ell = 0; ell < n; ell++) 
		n_color = std::max(n_color, color[ell] + 1);

	// direction vector for calls to forward
	VectorBase dx(n);

	// location for return values from forward
	VectorBase dy(m);

	// initialize the return value
	for(k = 0; k < K; k++)
		jac[k] = zero;

	// loop over colors
	size_t n_sweep = 0;
	for(ell = 0; ell < n_color; ell++)
	{	bool any = false;
		k = 0;
		for(j = 0; j < n; j++) if( color[j] == ell )
		{	// find first k such that c[k] has color ell
			if( ! any )
			{	while( c[k] < j )
					k++;
				any = c[k] == j;
			}
		}
		if( any )
		{	n_sweep++;
			// combine all columns with this color
			for(j = 0; j < n; j++)
				dx[j] = zero;
			for(j = 0; j < n; j++)
			{	if( color[j] == ell )
					dx[j] = one;
			}
			// call forward mode for all these columns at once
			dy = Forward(1, dx);

			// set the corresponding components of the result
			for(j = 0; j < n; j++) if( color[j] == ell )
			{	// find first index in c for this column
				while( c[k] < j )
					k++;
				// extract the row results for this column
				while( c[k] == j ) 
				{	jac[k] = dy[r[k]];
					k++;
				}
			}
		}
	}
	return n_sweep;
}
/*!
Private helper function SparseJacobianReverse(x, p, r, c, jac).

All descriptions in the public function SparseJacobian(x, p, r, c, jac) apply.

\param x
See \c SparseJacobian(x, p, r, c, jac).

\param p_transpose
See \c SparseJacobian(x, p, r, c, jac).
Note that we do not change the values in \c p,
but is is not \c const because we use its iterator facility.

\param r
See \c SparseJacobian(x, p, r, c, jac)
with the additional restriction that \c r , \c c are in column major order.

\param c
See \c SparseJacobian(x, p, r, c, jac).
with the additional restriction that \c r , \c c are in column major order.

\param jac
See \c SparseJacobian(x, p, r, c, jac).

\return
See \c SparseJacobian(x, p, r, c, jac).
*/
template<class Base>
template <class VectorBase, class VectorSet, class VectorSize>
size_t ADFun<Base>::SparseJacobianReverse(
	const VectorBase&  x               ,
	VectorSet&         p               ,
	const VectorSize&  r               ,
	const VectorSize&  c               ,
	VectorBase&        jac             )
{
	using   CppAD::vectorBool;
	size_t i, j, k, ell;

	size_t m = Range();
	size_t n = Domain();

	// some values
	const Base zero(0);
	const Base one(1);

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CPPAD_ASSERT_UNKNOWN( x.size() == n );
	CPPAD_ASSERT_UNKNOWN( p.n_set() ==  m );
	CPPAD_ASSERT_UNKNOWN( p.end() ==  n );

	// number of components of Jacobian that are required
	size_t K = jac.size();
	CPPAD_ASSERT_UNKNOWN( r.size() == K+1 );
	CPPAD_ASSERT_UNKNOWN( c.size() == K );
	CPPAD_ASSERT_UNKNOWN( r[K] == m );
	CPPAD_ASSERT_UNKNOWN( is_major_minor_order(c, r) );

	// Point at which we are evaluating the Jacobian
	Forward(0, x);

	// mapping from column number to set of rows required
	VectorSet require;
	require.resize(n, m);
	k = 0;
	while( k < K )
	{	require.add_element(c[k], r[k]);
		k++;
	}

	// initial coloring
	VectorSize color(m);
	for(i = 0; i < m; i++)
		color[i] = i;

	// See GreedyPartialD2Coloring Algorithm Section 3.6.2 of
	// Graph Coloring in Optimization Revisited by
	// Assefaw Gebremedhin, Fredrik Maane, Alex Pothen
	vectorBool forbidden(m);
	for(i = 1; i < m; i++)
	{
		// initial all colors as ok for this row
		// (value of forbidden for ell > i does not matter)
		for(ell = 0; ell <= i; ell++)
			forbidden[ell] = false;

		// for each column that is non-zero for this row
		p.begin(i);
		j = p.next_element();
		while( j != p.end() )
		{	// for each row that we require a value for this column
			require.begin(j);
			ell = require.next_element();
			while( ell != require.end() )
			{	// if this is not the same row, forbid its color 
				if( ell < i )
					forbidden[ color[ell] ] = true;
				ell = require.next_element();
			}
			j = p.next_element();
		}
		ell = 0;
		while( forbidden[ell] )
		{	ell++;
			CPPAD_ASSERT_UNKNOWN( ell <= i );
		}
		color[i] = ell;
	}
	size_t n_color = 1;
	for(ell = 0; ell < m; ell++) 
		n_color = std::max(n_color, color[ell] + 1);

	// weighting vector for calls to reverse
	VectorBase w(m);

	// location for return values from Reverse
	VectorBase dw(n);

	// initialize the return value
	for(k = 0; k < K; k++)
		jac[k] = zero;

	// loop over colors
	size_t n_sweep = 0;
	for(ell = 0; ell < n_color; ell++)
	{	bool any = false;
		k = 0;
		for(i = 0; i < m; i++) if( color[i] == ell )
		{	// find first k such that r[k] has color ell
			if( ! any )
			{	while( r[k] < i )
					k++;
				any = r[k] == i;
			}
		}
		if( any )
		{	n_sweep++;
			// combine all the rows with this color
			for(i = 0; i < m; i++)
				w[i] = zero;
			for(i = 0; i < m; i++)
			{	if( color[i] == ell )
					w[i] = one;
			}
			// call reverse mode for all these rows at once
			dw = Reverse(1, w);

			// set the corresponding components of the result
			for(i = 0; i < m; i++) if( color[i] == ell )
			{	// find first index in r for this row
				while( r[k] < i )
					k++;
				// extract the row results for this row
				while( r[k] == i ) 
				{	jac[k] = dw[c[k]];
					k++;
				}
			}
		}
	}
	return n_sweep;
}
// ===========================================================================
/*!
Private helper function SparseJacobianCase(set_type, x, p, r, c, jac).

All of the description in the public member function 
SparseJacobian(x, p, r, c, jac) applies.

\param set_type
is a \c bool value. This argument is used to dispatch to the proper source
code depending on the value of \c VectorSet::value_type.

\param x
See \c SparseJacobian(x, p, r, c, jac).

\param p
See \c SparseJacobian(x, p, r, c, jac).

\param r
See \c SparseJacobian(x, p, r, c, jac).

\param c
See \c SparseJacobian(x, p, r, c, jac).

\param jac
See \c SparseJacobian(x, p, r, c, jac).

\return
See \c SparseJacobian(x, p, r, c, jac).
*/
template <class Base>
template <class VectorBase, class VectorSet, class VectorSize>
size_t ADFun<Base>::SparseJacobianCase(
	bool               set_type        ,
	const VectorBase&  x               , 
	const VectorSet&   p               ,
	const VectorSize&  r               ,
	const VectorSize&  c               ,
	VectorBase&        jac             )
{	size_t n = Domain();
	size_t m = Range();
	size_t n_sweep;

	// check VectorSet is Simple Vector class with bool elements
	CheckSimpleVector<bool, VectorSet>();

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CPPAD_ASSERT_KNOWN(
		p.size() == m * n ,
		"SparseJacobian: using bool values and size of p "
		" not equal range dimension times domain dimension for f"
	); 
	CPPAD_ASSERT_UNKNOWN( x.size() == n );
	size_t K = jac.size();

	if( c.size() == K+1 )
	{	// row major, use forward mode ----------------------------------
		CPPAD_ASSERT_UNKNOWN( r.size() == K );
		CPPAD_ASSERT_UNKNOWN( c[K] == n );

		// convert the sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_pack sparsity;
		bool transpose = true;
		vec_bool_to_sparse_pack(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		n_sweep = SparseJacobianForward(x, sparsity, r, c, jac);
	}
	else
	{	// column major, use reverse mode --------------------------------
		CPPAD_ASSERT_UNKNOWN( r.size() == K+1 );
		CPPAD_ASSERT_UNKNOWN( c.size() == K   );
		CPPAD_ASSERT_UNKNOWN( r[K] == m );

		// convert the sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_pack sparsity;
		bool transpose = false;
		vec_bool_to_sparse_pack(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		n_sweep = SparseJacobianReverse(x, sparsity, r, c, jac);
	}
	return n_sweep;
}
/*!
Private helper function SparseJacobianCase(set_type, x, p, r, c, jac).

All of the description in the public member function 
SparseJacobian(x, p, r, c, jac) applies.

\param set_type
is a \c std::set<size_t> value. 
This argument is used to dispatch to the proper source
code depending on the value of \c VectorSet::value_type.

\param x
See \c SparseJacobian(x, p, r, c, jac).

\param p
See \c SparseJacobian(x, p, r, c, jac).

\param r
See \c SparseJacobian(x, p, r, c, jac).

\param c
See \c SparseJacobian(x, p, r, c, jac).

\param jac
See \c SparseJacobian(x, p, r, c, jac).

\return
See \c SparseJacobian(x, p, r, c, jac).
*/
template <class Base>
template <class VectorBase, class VectorSet, class VectorSize>
size_t ADFun<Base>::SparseJacobianCase(
	const std::set<size_t>&  set_type        ,
	const VectorBase&        x               , 
	const VectorSet&         p               ,
	const VectorSize&        r               ,
	const VectorSize&        c               ,
	VectorBase&              jac             )
{	size_t n = Domain();
	size_t m = Range();
	size_t n_sweep;

	// check VectorSet is Simple Vector class with sets for elements
	CheckSimpleVector<std::set<size_t>, VectorSet>(
		one_element_std_set<size_t>(), two_element_std_set<size_t>()
	);

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CPPAD_ASSERT_KNOWN(
		p.size() == Range() ,
		"SparseJacobian: using size of set sparsity pattern p "
		"not equal range dimension for f"
	); 
	CPPAD_ASSERT_UNKNOWN( x.size() == n );
	size_t K = jac.size();

	if( c.size() == K+1 )
	{	// row major, use forward mode ---------------------------------
		CPPAD_ASSERT_UNKNOWN( r.size() == K );
		CPPAD_ASSERT_UNKNOWN( c[K] == n );

		// convert the sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_set sparsity;
		bool transpose = true;
		vec_set_to_sparse_set(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		n_sweep = SparseJacobianForward(x, sparsity, r, c, jac);
	}
	else
	{	// column major, use reverse mode -----------------------------------
		CPPAD_ASSERT_UNKNOWN( c.size() == K );
		CPPAD_ASSERT_UNKNOWN( r.size() == K+1 );
		CPPAD_ASSERT_UNKNOWN( r[K] == m );

		// convert the sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_set sparsity;
		bool transpose = false;
		vec_set_to_sparse_set(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		n_sweep = SparseJacobianReverse(x, sparsity, r, c, jac);
	}
	return n_sweep;
}
// ===========================================================================
/*!
Private helper function SparseJacobianCase(set_type, x, p, jac).

All of the description in the public member function SparseJacobian(x, p)
applies.

\param set_type
is a \c bool value. This argument is used to dispatch to the proper source
code depending on the value of \c VectorSet::value_type.

\param x
See \c SparseJacobian(x, p).

\param p
See \c SparseJacobian(x, p).

\param jac
is the return value for the corresponding call to \c SparseJacobian(x, p).
On input, it must have size equal to the domain times range dimension
for this ADFun<Base> object.
On return, it will contain the Jacobian.
*/
template <class Base>
template <class VectorBase, class VectorSet>
void ADFun<Base>::SparseJacobianCase(
	bool               set_type        ,
	const VectorBase&  x               , 
	const VectorSet&   p               ,
	VectorBase&        jac             )
{
	typedef CppAD::vector<size_t> SizeVector;
	typedef CppAD::vectorBool     VectorBool;
	size_t i, j, k;

	size_t m = Range();
	size_t n = Domain();

	// the value zero
	const Base zero(0);

	// check VectorSet is Simple Vector class with bool elements
	CheckSimpleVector<bool, VectorSet>();

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CPPAD_ASSERT_KNOWN(
		p.size() == m * n,
		"SparseJacobian: using bool values and size of p "
		" not equal range dimension times domain dimension for f"
	); 
	CPPAD_ASSERT_UNKNOWN( x.size() == n );
	CPPAD_ASSERT_UNKNOWN(jac.size() == m * n); 

	// row index, column index, value representation
	// (used to fold problem into user interface case).
	size_t K = 0;
	for(j = 0; j < n; j++)
	{	for(i = 0; i < m; i++)
			if( p[ i * n + j ] )
				K++;
	} 
	VectorBase J(K);
	CppAD::vector<size_t> r, c;

	if( n <= m )
	{	// use row major, forward mode -----------------------------------
		r.resize(K); c.resize(K+1);
		k = 0;
		for(j = 0; j < n; j++)
		{	for(i = 0; i < m; i++)
			{	if( p[ i * n + j ] )
				{	r[k] = i;
					c[k] = j;
					k++;
				}
			}
		} 
		c[K] = n;

		// convert transposed sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_pack sparsity;
		bool transpose = true;
		vec_bool_to_sparse_pack(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		SparseJacobianForward(x, sparsity, r, c, J);
	}
	else
	{	// use column major, reverse mode ----------------------------------
		r.resize(K+1); c.resize(K);
		k = 0;
		for(i = 0; i < m; i++)
		{	for(j = 0; j < n; j++)
			{	if( p[ i * n + j ] )
				{	r[k] = i;
					c[k] = j;
					k++;
				}
			}
		} 
		r[K] = m;

		// convert the sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_pack sparsity;
		bool transpose = false;
		vec_bool_to_sparse_pack(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		SparseJacobianReverse(x, sparsity, r, c, J);
	}
	// initialize the return value
	for(i = 0; i < m; i++)
		for(j = 0; j < n; j++)
			jac[i * n + j] = zero;
	// now set the non-zero return values
	for(k = 0; k < K; k++)
		jac[r[k] * n + c[k]] = J[k];
}

/*!
Private helper function SparseJacobianCase(set_type, x, p, jac).

All of the description in the public member function SparseJacobian(x, p)
applies.

\param set_type
is a \c std::set<size_t> value.
This argument is used to dispatch to the proper source
code depending on the vlaue of \c VectorSet::value_type.

\param x
See \c SparseJacobian(x, p).

\param p
See \c SparseJacobian(x, p).

\param jac
is the return value for the corresponding call to \c SparseJacobian(x, p).
On input it must have size equalt to the domain times range dimension
for this ADFun<Base> object.
On return, it will contain the Jacobian.
*/
template <class Base>
template <class VectorBase, class VectorSet>
void ADFun<Base>::SparseJacobianCase(
	const std::set<size_t>&  set_type        ,
	const VectorBase&        x               , 
	const VectorSet&         p               ,
	VectorBase&              jac             )
{
	typedef CppAD::vector<size_t> SizeVector;
	typedef CppAD::vectorBool     VectorBool;
	size_t i, j, k;

	size_t m = Range();
	size_t n = Domain();

	// the value zero
	const Base zero(0);

	// check VectorSet is Simple Vector class with sets for elements
	CheckSimpleVector<std::set<size_t>, VectorSet>(
		one_element_std_set<size_t>(), two_element_std_set<size_t>()
	);

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CPPAD_ASSERT_KNOWN(
		p.size() == m,
		"SparseJacobian: using size of set sparsity pattern p "
		"not equal range dimension for f"
	); 
	CPPAD_ASSERT_UNKNOWN( x.size() == n );
	CPPAD_ASSERT_UNKNOWN(jac.size() == m * n); 

	// row index, column index, value representation
	// (used to fold problem into user interface case).
	size_t K = 0;
	std::set<size_t>::const_iterator itr;
	for(i = 0; i < m; i++)
	{	itr = p[i].begin();
		while( itr != p[i].end() )
		{	itr++;
			K++;
		}
	}	
	VectorBase J(K);
	CppAD::vector<size_t> r, c;

	if( n <= m )
	{	// use row major, forward mode -----------------------------------
		r.resize(K); c.resize(K+1);

		// need a pack_set transposed copy of the sparsity pattern.
		sparse_set sparsity;
		bool transpose = true;
		vec_set_to_sparse_set(sparsity, p, m, n, transpose);

		k = 0;
		for(j = 0; j < n; j++)
		{	sparsity.begin(j);
			i = sparsity.next_element();
			while( i != sparsity.end() )
			{	r[k] = i;
				c[k] = j;
				k++;
				i    = sparsity.next_element();
			}
		} 
		c[K] = n;
	
		// now we have folded this into the following case
		SparseJacobianForward(x, sparsity, r, c, J);
	}
	else
	{	// use column major, reverse mode ---------------------------------
		r.resize(K+1); c.resize(K);
		k = 0;
		for(i = 0; i < m; i++)
		{	itr = p[i].begin();
			while( itr != p[i].end() )
			{	j = *itr++;
				r[k] = i;
				c[k] = j;
				k++;
			}
		} 
		r[K] = m;

		// need a pack_set copy of sparsity pattern
		sparse_set sparsity;
		bool transpose = false;
		vec_set_to_sparse_set(sparsity, p, m, n, transpose);

		// now we have folded this into the following case
		SparseJacobianReverse(x, sparsity, r, c, J);
	}
	// initialize the return value
	for(i = 0; i < m; i++)
		for(j = 0; j < n; j++)
			jac[i * n + j] = zero;

	// now set the non-zero return values
	for(k = 0; k < K; k++)
		jac[r[k] * n + c[k]] = J[k];
}
// ==========================================================================
// Public Member functions
// ==========================================================================
/*!
Compute user specified subset of a sparse Jacobian.

The C++ source code corresponding to this operation is
\verbatim
	SparceJacobian(x, p, r, c, jac)
\endverbatim

\tparam Base
is the base type for the recording that is stored in this
ADFun<Base object.

\tparam VectorBase
is a simple vector class with elements of type \a Base.

\tparam VectorSet
is a simple vector class with elements of type 
\c bool or \c std::set<size_t>.

\param x
is a vector specifing the point at which to compute the Jacobian.

\param p
is the sparsity pattern for the Jacobian that we are calculating.

\param r
is the vector of row indices for the returned Jacobian values.

\param c
is the vector of columns indices for the returned Jacobian values.
It must have the same size are r.

\param jac
is the vector of Jacobian values.
It must have the same size are r. 
The return value <code>jac[k]</code> is the partial of the
<code>r[k]</code> component of the function with respect
the the <code>c[k]</component> of its argument.

\return
Is the number of first order forward (or reverse) sweeps used to compute the
requested Jacobian values. The total work, not counting the zero order
forward sweep, or the time to combine computations, is proportional to this
return value.
*/
template<class Base>
template <class VectorBase, class VectorSet, class VectorSize>
size_t ADFun<Base>::SparseJacobian(
	const VectorBase&  x               ,
	const VectorSet&   p               ,
	const VectorSize&  r               ,
	const VectorSize&  c               ,
	VectorBase&        jac             )
{
# ifndef NDEBUG
	size_t m = Range();
	size_t n = Domain();
	size_t K = jac.size();
	CPPAD_ASSERT_KNOWN(
		x.size() == n ,
		"SparseJacobian: size of x not equal domain dimension for f."
	); 
	CPPAD_ASSERT_KNOWN(
		r.size() == K+1 || c.size() == K+1 ,
		"SparseJacobian: neither r nor c has size K+1."
	); 
	if( r.size() == K+1 )
	{	// column major case
		CPPAD_ASSERT_KNOWN(
			is_major_minor_order(c, r),
			"SparseJacobian: r.size() == K+1 but not column major order."
		);
		CPPAD_ASSERT_KNOWN(
			c.size() == K,
			"SparseJacobian: r.size() == K+1 but c.size() != K."
		);
		CPPAD_ASSERT_KNOWN(
			r[K] == m,
			"SparseJacobian: r.size() == K+1 but r[K] != m."
		);
	}
	else if( c.size() == K+1 )
	{	// row major case
		CPPAD_ASSERT_KNOWN(
			is_major_minor_order(r, c),
			"SparseJacobian: c.size() == K+1 but not row major order."
		);
		CPPAD_ASSERT_KNOWN(
			r.size() == K,
			"SparseJacobian: c.size() == K+1 but r.size() != K."
		);
		CPPAD_ASSERT_KNOWN(
			c[K] == n,
			"SparseJacobian: c.size() == K+1 but c[K] != n."
		);
	}
	for(size_t k = 0; k < K; k++)
	{	CPPAD_ASSERT_KNOWN(
			r[k] < m,
			"SparseJacobian: a row index r[k] is too large."
		);
		CPPAD_ASSERT_KNOWN(
			c[k] < n,
			"SparseJacobian: a column index c[k] is too large."
		);
	}
# endif
	typedef typename VectorSet::value_type Set_type;
	size_t n_sweep = SparseJacobianCase(Set_type(), x, p, r, c, jac);
	return n_sweep;
}
/*!
Compute a sparse Jacobian.

The C++ source code corresponding to this operation is
\verbatim
	jac = SparseJacobian(x, p)
\endverbatim

\tparam Base
is the base type for the recording that is stored in this
ADFun<Base object.

\tparam VectorBase
is a simple vector class with elements of type \a Base.

\tparam VectorSet
is a simple vector class with elements of type 
\c bool or \c std::set<size_t>.

\param x
is a vector specifing the point at which to compute the Jacobian.

\param p
is the sparsity pattern for the Jacobian that we are calculating.

\return
Will be a vector if size \c m * n containing the Jacobian at the
specified point (in row major order).
*/
template <class Base>
template <class VectorBase, class VectorSet>
VectorBase ADFun<Base>::SparseJacobian(
	const VectorBase& x, const VectorSet& p
)
{	size_t m = Range();
	size_t n = Domain();
	VectorBase jac(m * n);

	CPPAD_ASSERT_KNOWN(
		x.size() == n,
		"SparseJacobian: size of x not equal domain size for f."
	);

	typedef typename VectorSet::value_type Set_type;
	SparseJacobianCase( Set_type(), x, p, jac);

	return jac;
} 

/*!
Compute a sparse Jacobian.

The C++ source code corresponding to this operation is
\verbatim
	jac = SparseJacobian(x)
\endverbatim

\tparam Base
is the base type for the recording that is stored in this
ADFun<Base object.

\tparam VectorBase
is a simple vector class with elements of the \a Base.

\param x
is a vector specifing the point at which to compute the Jacobian.

\return
Will be a vector of size \c m * n containing the Jacobian at the
specified point (in row major order).
*/
template <class Base>
template <class VectorBase>
VectorBase ADFun<Base>::SparseJacobian( const VectorBase& x )
{	typedef CppAD::vectorBool   VectorBool;

	size_t m = Range();
	size_t n = Domain();

	// sparsity pattern for Jacobian
	VectorBool p(m * n);

	if( n <= m )
	{	size_t j, k;

		// use forward mode 
		VectorBool r(n * n);
		for(j = 0; j < n; j++)
		{	for(k = 0; k < n; k++)
				r[j * n + k] = false;
			r[j * n + j] = true;
		}
		p = ForSparseJac(n, r);
	}
	else
	{	size_t i, k;

		// use reverse mode 
		VectorBool s(m * m);
		for(i = 0; i < m; i++)
		{	for(k = 0; k < m; k++)
				s[i * m + k] = false;
			s[i * m + i] = true;
		}
		p = RevSparseJac(m, s);
	}
	return SparseJacobian(x, p);
}


CPPAD_END_NAMESPACE
# endif
