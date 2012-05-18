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
	recomputed
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
%jac% = %f%.SparseJacobian(%x%, %p%)
%n_sweep% = %f%.SparseJacobianForward(%x%, %p%, %r%, %c%, %jac%, %work%)
%n_sweep% = %f%.SparseJacobianReverse(%x%, %p%, %r%, %c%, %jac%, %work%)
%$$

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
We use $latex K$$ to denote the value $icode%jac%.size()%$$
which must also equal the size of $icode r$$ and $icode c$$.
Furthermore,
for $latex k = 0 , \ldots , K-1$$, it must hold that
$latex r[k] < m$$ and $latex c[k] < n$$.

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

$head work$$
If this argument is present, it has prototype
$icode%
	sparse_jacobian_work& %work%
%$$
(see $cref/VectorSize/sparse_jacobian/VectorSize/$$ below).
This object can only be used with the routines $code SparseJacobianForward$$
or $code SparseJacobianReverse$$.
During its the first use, information is stored in $icode work$$.
This is used to reduce the work done by future calls to the 
same routine, with the same $icode f$$, $icode p$$, $icode r$$, and $icode c$$.
If a future call is make where any of these values have changed,
you must first call $icode%work%.clear()%$$
to inform CppAD that this information needs to be recomputed.

$head n_sweep$$
The return value $icode n_sweep$$ has prototype
$codei%
	size_t %n_sweep%
%$$
If $code SparseJacobianForward$$ ($code SparseJacobianReverse$$) is used, 
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
class used by SparseJacobian to hold information 
so it does not need to be recomputed.
*/
class sparse_jacobian_work {
	public:
		/// version of user r array sorted by row or column
		CppAD::vector<size_t> r_sort;
		/// version of user c array sorted by row or column
		CppAD::vector<size_t> c_sort;
		/// mapping from sorted array indices to user array indices
		CppAD::vector<size_t> k_sort;
		/// results of the coloring algorithm
		CppAD::vector<size_t> color;
		/// inform CppAD that this information needs to be recomputed
		void clear(void)
		{	r_sort.resize(0);
			c_sort.resize(0);
			k_sort.resize(0);
			color.resize(0);
		}
};
// ===========================================================================
/*!
std::qsort comparision function

\tparam Key
Is the type used for the key for the comparision operation.
There must be a conversion operation from the type 
\c Key to the type \c int. 

\param a_vptr
is a pointer to first \c Key value to be compared
(this must be the beginning of the corresponding record being sorted).

\param b_vptr
is a pointer to the second \c Key value to be compared
(this must be the beginning of the corresponding record being sorted).

\return
is negative, zero, position if the first value is less than,
equal, or greater than the second (respectively). 
*/
template <class Key>
int std_qsort_compare(const void* a_vptr, const void* b_vptr)
{	const Key* a_ptr = reinterpret_cast<const Key*>(a_vptr);
	const Key* b_ptr = reinterpret_cast<const Key*>(b_vptr);
	int a = static_cast<int>(*a_ptr);
	int b = static_cast<int>(*b_ptr);
	return a - b;
}
// ===========================================================================
/*!
Private helper function forward mode cases

\tparam Base
See \c SparseJacobianForward(x, p, r, c, jac, work).

\tparam VectorBase
See \c SparseJacobianForward(x, p, r, c, jac, work).

\tparam VectorSet
is either \c sparse_pack or \c sparse_set.

\param x
See \c SparseJacobianForward(x, p, r, c, jac, work).

\param p_transpose
Sparsity pattern for the transpose of the Jacobian of this ADFun<Base> object.
Note that we do not change the values in \c p_transpose,
but is not \c const because we use its iterator facility.

\param jac
See \c SparseJacobianForward(x, p, r, c, jac, work).

\param work
See \c SparseJacobianForward(x, p, r, c, jac, work).

\return
See \c SparseJacobianForward(x, p, r, c, jac, work).
*/
template<class Base>
template <class VectorBase, class VectorSet>
size_t ADFun<Base>::SparseJacobianFor(
	const VectorBase&     x               ,
	VectorSet&            p_transpose     ,
	VectorBase&           jac             ,
	sparse_jacobian_work& work            )
{
	using   CppAD::vectorBool;
	size_t i, j, k, ell;

	CppAD::vector<size_t>& r(work.r_sort);
	CppAD::vector<size_t>& c(work.c_sort);
	CppAD::vector<size_t>& user_k(work.k_sort);
	CppAD::vector<size_t>& color(work.color);

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

	// Point at which we are evaluating the Jacobian
	Forward(0, x);

	// mapping from row number to set of columns required
	VectorSet require;
	require.resize(m, n);
	k = 0;
	while( k < K )
	{	require.add_element(r[k], c[k]);
		CPPAD_ASSERT_UNKNOWN( r[k] < m && c[k] < n );
		k++;
		CPPAD_ASSERT_UNKNOWN( k == K || c[k-1] <= c[k] );
	}

	if( color.size() == 0 )
	{	// initial coloring
		color.resize(n);
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
			{	dx[j] = zero;
				if( color[j] == ell )
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
				{	jac[ user_k[k] ] = dy[r[k]];
					k++;
				}
			}
		}
	}
	return n_sweep;
}
/*!
Private helper function for reverse mode cases.

\tparam Base
See \c SparseJacobianForward(x, p, r, c, jac, work).

\tparam VectorBase
See \c SparseJacobianForward(x, p, r, c, jac, work).

\tparam VectorSet
is either \c sparse_pack or \c sparse_set.

\param x
See \c SparseJacobianForward(x, p, r, c, jac, work).

\param p
Sparsity pattern for the Jacobian of this ADFun<Base> object.
Note that we do not change the values in \c p_transpose,
but is not \c const because we use its iterator facility.

\param jac
See \c SparseJacobianForward(x, p, r, c, jac, work).

\param work
See \c SparseJacobianForward(x, p, r, c, jac, work).

\return
See \c SparseJacobianForward(x, p, r, c, jac, work).
*/
template<class Base>
template <class VectorBase, class VectorSet>
size_t ADFun<Base>::SparseJacobianRev(
	const VectorBase&     x        ,
	VectorSet&            p        ,
	VectorBase&           jac      ,
	sparse_jacobian_work& work     )
{
	using   CppAD::vectorBool;
	size_t i, j, k, ell;

	CppAD::vector<size_t>& r(work.r_sort);
	CppAD::vector<size_t>& c(work.c_sort);
	CppAD::vector<size_t>& user_k(work.k_sort);
	CppAD::vector<size_t>& color(work.color);

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

	// Point at which we are evaluating the Jacobian
	Forward(0, x);

	// mapping from column number to set of rows required
	VectorSet require;
	require.resize(n, m);
	k = 0;
	while( k < K )
	{	require.add_element(c[k], r[k]);
		CPPAD_ASSERT_UNKNOWN( r[k] < m && c[k] < n );
		k++;
		CPPAD_ASSERT_UNKNOWN( k == K || r[k-1] <= r[k] );
	}

	if( color.size() == 0 )
	{	// initial coloring
		color.resize(m);
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
			{	w[i] = zero;
				if( color[i] == ell )
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
				{	jac[ user_k[k] ] = dw[c[k]];
					k++;
				}
			}
		}
	}
	return n_sweep;
}
// ===========================================================================
/*!
Private helper function for vector of bool sparsity pattern cases.

\tparam Base
See \c SparseJacobianForward(x, p, r, c, jac, work).

\tparam VectorBase
See \c SparseJacobianForward(x, p, r, c, jac, work).

\tparam VectorSet
is a simple vector with elements of type \c bool.

\param set_type
has element type for vector representing the sparsity sets.

\param x
See \c SparseJacobianForward(x, p, r, c, jac, work).

\param p
Sparsity pattern for the Jacobian of this ADFun<Base> object.

\param jac
See \c SparseJacobianForward(x, p, r, c, jac, work).

\param work
See \c SparseJacobianForward(x, p, r, c, jac, work).

\return
See \c SparseJacobianForward(x, p, r, c, jac, work).
*/
template <class Base>
template <class VectorBase, class VectorSet>
size_t ADFun<Base>::SparseJacobianCase(
	bool                  set_type    ,
	const VectorBase&     x           , 
	const VectorSet&      p           ,
	VectorBase&           jac         ,
	sparse_jacobian_work& work        )
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

	if( work.c_sort.size() == K+1 )
	{	// row major, use forward mode ----------------------------------
		CPPAD_ASSERT_UNKNOWN( work.r_sort.size() == K );
		CPPAD_ASSERT_UNKNOWN( work.c_sort[K] == n );

		// convert the sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_pack sparsity;
		bool transpose = true;
		vec_bool_to_sparse_pack(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		n_sweep = SparseJacobianFor(x, sparsity, jac, work);
	}
	else
	{	// column major, use reverse mode --------------------------------
		CPPAD_ASSERT_UNKNOWN( work.r_sort.size() == K+1 );
		CPPAD_ASSERT_UNKNOWN( work.c_sort.size() == K   );
		CPPAD_ASSERT_UNKNOWN( work.r_sort[K] == m );

		// convert the sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_pack sparsity;
		bool transpose = false;
		vec_bool_to_sparse_pack(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		n_sweep = SparseJacobianRev(x, sparsity, jac, work);
	}
	return n_sweep;
}
/*!
Private helper function for vector of std::set<size_t> sparsity pattern cases.

\tparam Base
See \c SparseJacobianForward(x, p, r, c, jac, work).

\tparam VectorBase
See \c SparseJacobianForward(x, p, r, c, jac, work).

\tparam VectorSet
is a simple vector with elements of type <code>std::set<size_t></code>.

\param set_type
has element type for vector representing the sparsity sets.

\param x
See \c SparseJacobianForward(x, p, r, c, jac, work).

\param p
Sparsity pattern for the Jacobian of this ADFun<Base> object.

\param jac
See \c SparseJacobianForward(x, p, r, c, jac, work).

\param work
See \c SparseJacobianForward(x, p, r, c, jac, work).

\return
See \c SparseJacobianForward(x, p, r, c, jac, work).
*/
template <class Base>
template <class VectorBase, class VectorSet>
size_t ADFun<Base>::SparseJacobianCase(
	const std::set<size_t>&  set_type        ,
	const VectorBase&        x               , 
	const VectorSet&         p               ,
	VectorBase&              jac             ,
	sparse_jacobian_work&    work            )
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

	if( work.c_sort.size() == K+1 )
	{	// row major, use forward mode ---------------------------------
		CPPAD_ASSERT_UNKNOWN( work.r_sort.size() == K );
		CPPAD_ASSERT_UNKNOWN( work.c_sort[K] == n );

		// convert the sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_set sparsity;
		bool transpose = true;
		vec_set_to_sparse_set(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		n_sweep = SparseJacobianFor(x, sparsity, jac, work);
	}
	else
	{	// column major, use reverse mode -----------------------------------
		CPPAD_ASSERT_UNKNOWN( work.c_sort.size() == K );
		CPPAD_ASSERT_UNKNOWN( work.r_sort.size() == K+1 );
		CPPAD_ASSERT_UNKNOWN( work.r_sort[K] == m );

		// convert the sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_set sparsity;
		bool transpose = false;
		vec_set_to_sparse_set(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		n_sweep = SparseJacobianRev(x, sparsity, jac, work);
	}
	return n_sweep;
}
// ===========================================================================
/*!
Private helper function for vector of bool sparsity pattern cases

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
	sparse_jacobian_work work;
	CppAD::vector<size_t>& r(work.r_sort);
	CppAD::vector<size_t>& c(work.c_sort);
	CppAD::vector<size_t>& user_k(work.k_sort);

	if( n <= m )
	{	// forward mode, columns are sorted
		r.resize(K); c.resize(K+1); user_k.resize(K);
		k = 0;
		for(j = 0; j < n; j++)
		{	for(i = 0; i < m; i++)
			{	if( p[ i * n + j ] )
				{	r[k]      = i;
					c[k]      = j;
					user_k[k] = k;
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
		SparseJacobianFor(x, sparsity, J, work);
	}
	else
	{	// reverse mode, rows are sorted
		r.resize(K+1); c.resize(K); user_k.resize(K);
		k = 0;
		for(i = 0; i < m; i++)
		{	for(j = 0; j < n; j++)
			{	if( p[ i * n + j ] )
				{	r[k]      = i;
					c[k]      = j;
					user_k[k] = k;
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
		SparseJacobianRev(x, sparsity, J, work);
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
Private helper function for vector of std::set<size_t> sparsity pattern cases.

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
	sparse_jacobian_work work;
	CppAD::vector<size_t>& r(work.r_sort);
	CppAD::vector<size_t>& c(work.c_sort);
	CppAD::vector<size_t>& user_k(work.k_sort);

	if( n <= m )
	{	// forward mode, columns are sorted
		r.resize(K); c.resize(K+1); user_k.resize(K);

		// need a pack_set transposed copy of the sparsity pattern.
		sparse_set sparsity;
		bool transpose = true;
		vec_set_to_sparse_set(sparsity, p, m, n, transpose);

		k = 0;
		for(j = 0; j < n; j++)
		{	sparsity.begin(j);
			i = sparsity.next_element();
			while( i != sparsity.end() )
			{	r[k]      = i;
				c[k]      = j;
				user_k[k] = k;
				k++;
				i    = sparsity.next_element();
			}
		} 
		c[K] = n;
	
		// now we have folded this into the following case
		SparseJacobianFor(x, sparsity, J, work);
	}
	else
	{	// reverse mode, rows are sorted
		r.resize(K+1); c.resize(K); user_k.resize(K);
		k = 0;
		for(i = 0; i < m; i++)
		{	itr = p[i].begin();
			while( itr != p[i].end() )
			{	j = *itr++;
				r[k]      = i;
				c[k]      = j;
				user_k[k] = k;
				k++;
			}
		} 
		r[K] = m;

		// need a pack_set copy of sparsity pattern
		sparse_set sparsity;
		bool transpose = false;
		vec_set_to_sparse_set(sparsity, p, m, n, transpose);

		// now we have folded this into the following case
		SparseJacobianRev(x, sparsity, J, work);
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
Compute user specified subset of a sparse Jacobian using forward mode.

The C++ source code corresponding to this operation is
\verbatim
	SparceJacobianForward(x, p, r, c, jac, work)
\endverbatim

\tparam Base
is the base type for the recording that is stored in this
ADFun<Base object.

\tparam VectorBase
is a simple vector class with elements of type \a Base.

\tparam VectorSet
is a simple vector class with elements of type 
\c bool or \c std::set<size_t>.

\tparam VectorSize
is a simple vector class with elements of type \c size_t.

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
the the <code>c[k]</code> of its argument.

\param work
contains information that depends on the function object, sparsity pattern,
\c r, and \c c vector.
If these values are the same, \c work does not need to be recomputed.
To be more specific, 
\c r_sort is sorted copy of \c r ,
\c c_sort is sorted copy of \c c ,
<code>k_sort[k]</code> is the original index corresponding to the
values <code>r_sort[k]</code> and <code>c_sort[k]</code>.
The order for the sort is either by rows, for reverse mode calculations,
and by columns, for forward mode calculations.
Let \c m be the range dimension, n the domain dimension,
and K the size of \c r , \c c , and \c jac.
If sorted by rows, there is one extra entry 
in the sorted row array and it has value <code>r_sort[K]=m</code>.
If sorted by columns, there is one extra entry 
in the sorted column array and it has value <code>c_sort[K]=n</code>.
The \c work vector \c color is set and used by 
\c SparseJacobianFor, for the forward mode case,
and by SparseJacobianRev, for the reverse mode case. 

\return
Is the number of first order forward sweeps used to compute the
requested Jacobian values. The total work, not counting the zero order
forward sweep, or the time to combine computations, is proportional to this
return value.
*/
template<class Base>
template <class VectorBase, class VectorSet, class VectorSize>
size_t ADFun<Base>::SparseJacobianForward(
	const VectorBase&     x    ,
	const VectorSet&      p    ,
	const VectorSize&     r    ,
	const VectorSize&     c    ,
	VectorBase&           jac  ,
	sparse_jacobian_work& work )
{
	size_t n = Domain();
	size_t k, K = jac.size();
	if( work.r_sort.size() == 0 )
	{	// create version of (r,c,k) sorted by columns
		size_t min_bytes = 3 * K * sizeof(size_t);
		size_t cap_bytes;
		void*  v_ptr  = thread_alloc::get_memory(min_bytes, cap_bytes);
		size_t* crk  = reinterpret_cast<size_t*>(v_ptr);
		for(k = 0; k < K; k++)
		{	// must put column first so it is used for sorting
			crk[3 * k + 0] = c[k];
			crk[3 * k + 1] = r[k];
			crk[3 * k + 2] = k;
		}
		std::qsort(crk, K, 3 * sizeof(size_t), std_qsort_compare<size_t>);
		work.c_sort.resize(K+1);
		work.r_sort.resize(K);
		work.k_sort.resize(K);
		for(k = 0; k < K; k++)
		{	work.c_sort[k] = crk[3 * k + 0];  
			work.r_sort[k] = crk[3 * k + 1];  
			work.k_sort[k] = crk[3 * k + 2];  
		}
		work.c_sort[K] = n;
		thread_alloc::return_memory(v_ptr);
	}
# ifndef NDEBUG
	size_t m = Range();
	CPPAD_ASSERT_KNOWN(
		x.size() == n ,
		"SparseJacobianForward: size of x not equal domain dimension for f."
	); 
	CPPAD_ASSERT_KNOWN(
		r.size() == K && c.size() == K ,
		"SparseJacobianForward: either r or c does not have "
		"the same size as jac."
	); 
	CPPAD_ASSERT_KNOWN(
		work.r_sort.size() == K   &&
		work.c_sort.size() == K+1 &&
		work.k_sort.size() == K   ,
		"SparseJacobianForward: invalid value in work."
	);
	CPPAD_ASSERT_KNOWN(
		work.color.size() == 0 || work.color.size() == n,
		"SparseJacobianForward: invalid value in work."
	);
	for(k = 0; k < K; k++)
	{	CPPAD_ASSERT_KNOWN(
			r[k] < m,
			"SparseJacobianForward: invalid value in r."
		);
		CPPAD_ASSERT_KNOWN(
			c[k] < n,
			"SparseJacobianForward: invalid value in c."
		);
		CPPAD_ASSERT_KNOWN(
			work.r_sort[k] < m,
			"SparseJacobianForward: invalid value in work."
		);
		CPPAD_ASSERT_KNOWN(
			work.c_sort[k] < n,
			"SparseJacobianForward: invalid value in work."
		);
		CPPAD_ASSERT_KNOWN(
			work.k_sort[k] < K,
			"SparseJacobianForward: invalid value in work."
		);
	}
	if( work.color.size() != 0 )
		for(size_t j = 0; j < n; j++) CPPAD_ASSERT_KNOWN(
			work.color[j] < n,
			"SparseJacobianForward: invalid value in work."
	);
# endif
 
	typedef typename VectorSet::value_type Set_type;
	size_t n_sweep = SparseJacobianCase(Set_type(), x, p, jac, work);
	return n_sweep;
}
/*!
Compute user specified subset of a sparse Jacobian using reverse mode.

The C++ source code corresponding to this operation is
\verbatim
	SparceJacobianReverse(x, p, r, c, jac, work)
\endverbatim

\tparam Base
See \c SparseJacobianForward(x, p, r, c, jac, work).

\tparam VectorBase
See \c SparseJacobianForward(x, p, r, c, jac, work).

\tparam VectorSet
is either \c sparse_pack or \c sparse_set.

\param x
See \c SparseJacobianForward(x, p, r, c, jac, work).

\param p
Sparsity pattern for the Jacobian of this ADFun<Base> object.

\param r
See \c SparseJacobianForward(x, p, r, c, jac, work).

\param c
See \c SparseJacobianForward(x, p, r, c, jac, work).

\param jac
See \c SparseJacobianForward(x, p, r, c, jac, work).

\param work
See \c SparseJacobianForward(x, p, r, c, jac, work).

\return
See \c SparseJacobianForward(x, p, r, c, jac, work).
*/
template<class Base>
template <class VectorBase, class VectorSet, class VectorSize>
size_t ADFun<Base>::SparseJacobianReverse(
	const VectorBase&     x    ,
	const VectorSet&      p    ,
	const VectorSize&     r    ,
	const VectorSize&     c    ,
	VectorBase&           jac  ,
	sparse_jacobian_work& work )
{
	size_t m = Range();
	size_t k, K = jac.size();
	if( work.r_sort.size() == 0 )
	{	// create version of (r,c,k) sorted by rows
		size_t min_bytes = 3 * K * sizeof(size_t);
		size_t cap_bytes;
		void*  v_ptr  = thread_alloc::get_memory(min_bytes, cap_bytes);
		size_t* rck  = reinterpret_cast<size_t*>(v_ptr);
		for(k = 0; k < K; k++)
		{	// must put column first so it is used for sorting
			rck[3 * k + 0] = r[k];
			rck[3 * k + 1] = c[k];
			rck[3 * k + 2] = k;
		}
		std::qsort(rck, K, 3 * sizeof(size_t), std_qsort_compare<size_t>);
		work.c_sort.resize(K);
		work.r_sort.resize(K+1);
		work.k_sort.resize(K);
		for(k = 0; k < K; k++)
		{	work.r_sort[k] = rck[3 * k + 0];  
			work.c_sort[k] = rck[3 * k + 1];  
			work.k_sort[k] = rck[3 * k + 2];  
		}
		work.r_sort[K] = m;
		thread_alloc::return_memory(v_ptr);
	}
# ifndef NDEBUG
	size_t n = Domain();
	CPPAD_ASSERT_KNOWN(
		x.size() == n ,
		"SparseJacobianReverse: size of x not equal domain dimension for f."
	); 
	CPPAD_ASSERT_KNOWN(
		r.size() == K && c.size() == K ,
		"SparseJacobianReverse: either r or c does not have "
		"the same size as jac."
	); 
	CPPAD_ASSERT_KNOWN(
		work.r_sort.size() == K+1 &&
		work.c_sort.size() == K   &&
		work.k_sort.size() == K   ,
		"SparseJacobianReverse: invalid value in work."
	);
	CPPAD_ASSERT_KNOWN(
		work.color.size() == 0 || work.color.size() == m,
		"SparseJacobianReverse: invalid value in work."
	);
	for(k = 0; k < K; k++)
	{	CPPAD_ASSERT_KNOWN(
			r[k] < m,
			"SparseJacobianReverse: invalid value in r."
		);
		CPPAD_ASSERT_KNOWN(
			c[k] < n,
			"SparseJacobianReverse: invalid value in c."
		);
		CPPAD_ASSERT_KNOWN(
			work.r_sort[k] < m,
			"SparseJacobianReverse: invalid value in work."
		);
		CPPAD_ASSERT_KNOWN(
			work.c_sort[k] < n,
			"SparseJacobianReverse: invalid value in work."
		);
		CPPAD_ASSERT_KNOWN(
			work.k_sort[k] < K,
			"SparseJacobianReverse: invalid value in work."
		);
	}
	if( work.color.size() != 0 )
		for(size_t i = 0; i < m; i++) CPPAD_ASSERT_KNOWN(
			work.color[i] < m,
			"SparseJacobianReverse: invalid value in work."
	);
# endif
 
	typedef typename VectorSet::value_type Set_type;
	size_t n_sweep = SparseJacobianCase(Set_type(), x, p, jac, work);
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
