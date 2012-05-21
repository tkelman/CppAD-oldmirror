/* $Id$ */
# ifndef CPPAD_SPARSE_HESSIAN_INCLUDED
# define CPPAD_SPARSE_HESSIAN_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sparse_hessian$$
$spell
	CppAD
	valarray
	std
	Bool
	hes
	const
	Taylor
$$

$section Sparse Hessian: Easy Driver$$
$index SparseHessian$$
$index hessian, sparse$$

$head Syntax$$
$icode%hes% = %f%.SparseHessian(%x%, %w%)
%hes% = %f%.SparseHessian(%x%, %w%, %p%)
%n_sweep% = %f%.SparseHessian(%x%, %w%, %p%, %r%, %c%, %hes%, %work%)
%$$

$head Purpose$$
We use $latex n$$ for the $cref/domain/seq_property/Domain/$$ size,
and $latex m$$ for the $cref/range/seq_property/Range/$$ size of $icode f$$.
We use $latex F : \R^n \rightarrow \R^m$$ do denote the
$cref/AD function/glossary/AD Function/$$
corresponding to $icode f$$. 
The syntax above sets $icode hes$$ to the Hessian 
$latex \[
	H(x) = \dpow{2}{x} \sum_{i=1}^m w_i F_i (x) 
\] $$
This routine takes advantage of the sparsity of the Hessian
in order to reduce the amount of computation necessary.
If $icode r$$ and $icode c$$ are present, it also takes
advantage of the reduced set of elements of the Hessian that
need to be computed.
One can use speed tests (e.g. $cref speed_test$$)
to verify that results are computed faster
than when using the routine $cref Hessian$$.

$head f$$
The object $icode f$$ has prototype
$codei%
	ADFun<%Base%> %f%
%$$
Note that the $cref ADFun$$ object $icode f$$ is not $code const$$
(see $cref/Uses Forward/sparse_hessian/Uses Forward/$$ below).

$head x$$
The argument $icode x$$ has prototype
$codei%
	const %VectorBase%& %x%
%$$
(see $cref/VectorBase/sparse_hessian/VectorBase/$$ below)
and its size 
must be equal to $icode n$$, the dimension of the
$cref/domain/seq_property/Domain/$$ space for $icode f$$.
It specifies
that point at which to evaluate the Hessian.

$head w$$
The argument $icode w$$ has prototype
$codei%
	const %VectorBase%& %w%
%$$
and size $latex m$$.
It specifies the value of $latex w_i$$ in the expression 
for $icode hes$$.
The more components of $latex w$$ that are identically zero,
the more sparse the resulting Hessian may be (and hence the more efficient
the calculation of $icode hes$$ may be).

$head p$$
The argument $icode p$$ is optional and has prototype
$codei%
	const %VectorSet%& %p%
%$$
(see $cref/VectorSet/sparse_hessian/VectorSet/$$ below)
If it has elements of type $code bool$$,
its size is $latex n * n$$.
If it has elements of type $code std::set<size_t>$$,
its size is $latex n$$ and all its set elements are between
zero and $latex n - 1$$.
It specifies a
$cref/sparsity pattern/glossary/Sparsity Pattern/$$ 
for the Hessian $latex H(x)$$.
$pre

$$
If this sparsity pattern does not change between calls to 
$codei SparseHessian$$, it should be faster to calculate $icode p$$ once and
pass this argument to $codei SparseHessian$$.
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
(see $cref/VectorSize/sparse_hessian/VectorSize/$$ below).
They specify which rows and columns of $latex H (x)$$ are
returned and in what order.
We use $latex K$$ to denote the value $icode%hes%.size()%$$
which must also equal the size of $icode r$$ and $icode c$$.
Furthermore,
for $latex k = 0 , \ldots , K-1$$, it must hold that
$latex r[k] < n$$ and $latex c[k] < n$$.


$head hes$$
The result $icode hes$$ has prototype
$codei%
	%VectorBase% %hes%
%$$
In the case where $icode r$$ and $icode c$$ are not presetn,
the size of $icode hess$$ is $latex n * n$$ and
and its size is $latex n * n$$.
In this case, for $latex i = 0 , \ldots , n - 1 $$ 
and $latex ell = 0 , \ldots , n - 1$$
$latex \[
	hes [ j * n + \ell ] = \DD{ w^{\rm T} F }{ x_j }{ x_\ell } ( x )
\] $$
$pre

$$
In the case where the arguments $icode r$$ and $icode c$$ are present,
we use $latex K$$ to denote the size of $icode hes$$. 
The input value of its elements does not matter.
Upon return, for $latex k = 0 , \ldots , K - 1$$,
$latex \[
	hes [ k ] = \DD{ w^{\rm T} F }{ x_j }{ x_\ell } (x)
	\; , \;
	\; {\rm where} \;
	j = r[k]
	\; {\rm and } \;
	\ell = c[k]
\] $$

$head work$$
If this argument is present, it has prototype
$icode%
	sparse_hessian_work& %work%
%$$
This object can only be used with the routines $code SparseHessian$$.
During its the first use, information is stored in $icode work$$.
This is used to reduce the work done by future calls to $code SparseHessian$$ 
with the same $icode f$$, $icode p$$, $icode r$$, and $icode c$$.
If a future call is make where any of these values have changed,
you must first call $icode%work%.clear()%$$
to inform CppAD that this information needs to be recomputed.

$head n_sweep$$
The return value $icode n_sweep$$ has prototype
$codei%
	size_t %n_sweep%
%$$
It is the number of first order forward sweeps 
used to compute the requested Hessian values.
Each first forward sweep is followed by a second order reverse sweep 
so it is also the number of reverse sweeps.
This is proportional to the total work that $code SparseHessian$$ does, 
not counting the zero order forward sweep, 
or the work to combine multiple columns into a single 
forward-reverse sweep pair.

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
After $code SparseHessian$$,
the previous calls to $cref Forward$$ are undefined.

$head Example$$
$children%
	example/sparse_hessian.cpp
%$$
The routine
$cref sparse_hessian.cpp$$ 
is examples and tests of $code sparse_hessian$$.
It return $code true$$, if it succeeds and $code false$$ otherwise.


$end
-----------------------------------------------------------------------------
*/
# include <cppad/local/std_set.hpp>

CPPAD_BEGIN_NAMESPACE
/*!
\file sparse_hessian.hpp
Sparse Hessian driver routine and helper functions.
*/
// ===========================================================================
/*!
class used by SparseHessian to hold information 
so it does not need to be recomputed.
*/
class sparse_hessian_work {
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
Private helper function that does computation for all Sparse Hessian cases.

\tparam Base
See \c SparseHessian(x, w, p, r, c, hes, work).

\tparam VectorBase
See \c SparseHessian(x, w, p, r, c, hes, work).

\tparam VectorSet
is either \c sparse_pack or \c sparse_set.

\param x
See \c SparseHessian(x, w, p, r, c, hes, work).

\param sparsity
If <code>work.color.size() != 0</code>, then \c sparsity is not used.
Otherwise, it is a 
sparsity pattern for the Hessian of this ADFun<Base> object.

\param hes
See \c SparseHessian(x, w, p, r, c, hes, work).

\param work
See \c SparseHessian(x, w, p, r, c, hes, work).

\return
See \c SparseHessian(x, w, p, r, c, hes, work).
*/
template<class Base>
template <class VectorBase, class VectorSet>
size_t ADFun<Base>::SparseHessianCompute(
	const VectorBase&     x           ,
	const VectorBase&     w           ,
	VectorSet&            sparsity    ,
	VectorBase&           hes         ,
	sparse_hessian_work& work         )
{
	using   CppAD::vectorBool;
	size_t i, j, k, ell;

	CppAD::vector<size_t>& r(work.r_sort);
	CppAD::vector<size_t>& c(work.c_sort);
	CppAD::vector<size_t>& user_k(work.k_sort);
	CppAD::vector<size_t>& color(work.color);

	size_t n = Domain();

	// some values
	const Base zero(0);
	const Base one(1);

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CPPAD_ASSERT_UNKNOWN( x.size() == n );
	CPPAD_ASSERT_UNKNOWN( color.size() == 0 || color.size() == n );

	// number of components of Hessian that are required
	size_t K = hes.size();
	CPPAD_ASSERT_UNKNOWN( r.size() == K+1 );
	CPPAD_ASSERT_UNKNOWN( c.size() == K );
	CPPAD_ASSERT_UNKNOWN( r[K] == n );

	// Point at which we are evaluating the Hessian
	Forward(0, x);

	// Rows of the Hessian (i below) correspond to the forward mode index
	// and columns (j below) correspond to the reverse mode index.
	if( color.size() == 0 )
	{
		CPPAD_ASSERT_UNKNOWN( sparsity.n_set() ==  n );
		CPPAD_ASSERT_UNKNOWN( sparsity.end() ==  n );

		// rows and columns that are in the returned hessian
		VectorSet r_used, c_used;
		r_used.resize(n, n);
		c_used.resize(n, n);
		k = 0;
		while( k < K )
		{	CPPAD_ASSERT_UNKNOWN( r[k] < n && c[k] < n );
			CPPAD_ASSERT_UNKNOWN( k == 0 || r[k-1] <= r[k] );
			r_used.add_element(c[k], r[k]);
			c_used.add_element(r[k], c[k]);
			k++;
		}
	
		// given a column index, which rows are non-zero and not used
		VectorSet not_used;
		not_used.resize(n, n);
		for(i = 0; i < n; i++)
		{	sparsity.begin(i);
			j = sparsity.next_element();
			while( j != sparsity.end() )
			{	if( ! r_used.is_element(j , i) )
					not_used.add_element(j, i);
				j = sparsity.next_element();
			}
		}

		// initial coloring
		color.resize(n);
		for(i = 0; i < n; i++)
			color[i] = i;
	
		// See GreedyPartialD2Coloring Algorithm Section 3.6.2 of
		// Graph Coloring in Optimization Revisited by
		// Assefaw Gebremedhin, Fredrik Maane, Alex Pothen
		vectorBool forbidden(n);
		for(i = 1; i < n; i++)
		{
			// initial all colors as ok for this row
			// (value of forbidden for ell > i does not matter)
			for(ell = 0; ell <= i; ell++)
				forbidden[ell] = false;
	
			// -----------------------------------------------------
			// Forbid colors that this row would destroy results for.
			// for each column that is non-zero for this row
			sparsity.begin(i);
			j = sparsity.next_element();
			while( j != sparsity.end() )
			{	// for each row that this column uses
				r_used.begin(j);
				ell = r_used.next_element();
				while( ell != r_used.end() )
				{	// if this is not the same row, forbid its color
					if( ell < i )
						forbidden[ color[ell] ] = true;
					ell = r_used.next_element();
				}
				j = sparsity.next_element();
			}
	
			// -------------------------------------------------------
			// Forbid colors that would destroy the results for this row.
			// for each column that this row used
			c_used.begin(i);
			j = c_used.next_element();
			while( j != c_used.end() )
			{	// For each row that is non-zero for this column
				// (the used rows have already been checked above).
				not_used.begin(j);
				ell = not_used.next_element();
				while( ell != not_used.end() )
				{	// if this is not the same row, forbid its color
					if( ell < i )
						forbidden[ color[ell] ] = true;
					ell = not_used.next_element();
				}
				j = c_used.next_element();
			}

			// pick the color with the smallest index
			ell = 0;
			while( forbidden[ell] )
			{	ell++;
				CPPAD_ASSERT_UNKNOWN( ell <= i );
			}
			color[i] = ell;
		}
	}
	size_t n_color = 1;
	for(ell = 0; ell < n; ell++) 
		n_color = std::max(n_color, color[ell] + 1);

	// direction vector for calls to forward (rows of the Hessian)
	VectorBase u(n);

	// location for return values from reverse (columns of the Hessian)
	VectorBase ddw(2 * n);

	// initialize the return value
	for(k = 0; k < K; k++)
		hes[k] = zero;

	// loop over colors
	size_t n_sweep = 0;
	for(ell = 0; ell < n_color; ell++)
	{	bool any = false;
		k = 0;
		for(i = 0; i < n; i++) if( color[i] == ell )
		{	// find first k such that r[k] has color ell
			if( ! any )
			{	while( r[k] < i )
					k++;
				any = r[k] == i;
			}
		}
		if( any )
		{	n_sweep++;
			// combine all rows with this color
			for(i = 0; i < n; i++)
			{	u[i] = zero;
				if( color[i] == ell )
					u[i] = one;
			}
			// call forward mode for all these rows at once
			Forward(1, u);

			// evaluate derivative of w^T * F'(x) * u
			ddw = Reverse(2, w);

			// set the corresponding components of the result
			for(i = 0; i < n; i++) if( color[i] == ell )
			{	// find first index in c for this column
				while( r[k] < i )
					k++;
				// extract the results for this row
				while( r[k] == i ) 
				{	hes[ user_k[k] ] = ddw[ c[k] * 2 + 1 ];
					k++;
				}
			}
		}
	}
	return n_sweep;
}
// ===========================================================================
/*!
Private helper function for vector of \c bool sparsity pattern cases.

\tparam Base
See \c SparseHessian(x, w, p, r, c, hes, work).

\tparam VectorBase
See \c SparseHessian(x, w, p, r, c, hes, work).

\tparam VectorSet
is a simple vector with elements of type \c bool.

\param set_type
has element type for vector representing the sparsity sets.

\param x
See \c SparseHessian(x, w, p, r, c, hes, work).

\param p
Sparsity pattern for the Hessian of this ADFun<Base> object.

\param hes
See \c SparseHessian(x, w, p, r, c, hes, work).

\param work
See \c SparseHessian(x, w, p, r, c, hes, work).

\return
See \c SparseHessian(x, w, p, r, c, hes, work).
*/
template <class Base>
template <class VectorBase, class VectorSet>
size_t ADFun<Base>::SparseHessianCase(
	bool                  set_type  ,
	const VectorBase&     x         ,
	const VectorBase&     w         ,
	const VectorSet&      p         ,
	VectorBase&           hes       ,
	sparse_hessian_work&  work      )
{
	size_t n = Domain();

	// check VectorSet is Simple Vector class with bool elements
	CheckSimpleVector<bool, VectorSet>();

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CPPAD_ASSERT_UNKNOWN( x.size() == n );
	CPPAD_ASSERT_UNKNOWN( w.size() == Range() );
	CPPAD_ASSERT_KNOWN(
		p.size() == n * n,
		"SparseHessian: using bool values for sparsity and p.size() "
		"not equal square of domain dimension for f"
	);
 
	sparse_pack sparsity;
	bool transpose = false;
	vec_bool_to_sparse_pack(sparsity, p, n, n, transpose);
	
	// compute the Hessian
	size_t n_sweep = SparseHessianCompute(x, w, sparsity, hes, work);

	return n_sweep;
}
/*!
Private helper function for vector of std::set<size_t> sparsity pattern cases.

\tparam Base
See \c SparseHessian(x, w, p, r, c, hes, work).

\tparam VectorBase
See \c SparseHessian(x, w, p, r, c, hes, work).

\tparam VectorSet
is a simple vector with elements of type <code>std::set<size_t></code>.

\param set_type
has element type for vector representing the sparsity sets.

\param x
See \c SparseHessian(x, w, p, r, c, hes, work).

\param p
Sparsity pattern for the Hessian of this ADFun<Base> object.

\param hes
See \c SparseHessian(x, w, p, r, c, hes, work).

\param work
See \c SparseHessian(x, w, p, r, c, hes, work).

\return
See \c SparseHessian(x, w, p, r, c, hes, work).
*/
template <class Base>
template <class VectorBase, class VectorSet>
size_t ADFun<Base>::SparseHessianCase(
	const std::set<size_t>&     set_type  ,
	const VectorBase&           x         ,
	const VectorBase&           w         ,
	const VectorSet&            p         ,
	VectorBase&                 hes       ,
	sparse_hessian_work&        work      )
{
	size_t n = Domain();

	// check VectorSet is Simple Vector class with std::set<size_t> elements
	CheckSimpleVector<std::set<size_t>, VectorSet>(
		one_element_std_set<size_t>(), two_element_std_set<size_t>()
	);

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CPPAD_ASSERT_UNKNOWN( x.size() == n );
	CPPAD_ASSERT_UNKNOWN( w.size() == Range() );
	CPPAD_ASSERT_KNOWN(
		p.size() == n,
		"SparseHessian: using std::set<size_t> for sparsity and p.size() "
		"not equal domain dimension for f"
	);
 
	sparse_set sparsity;
	bool transpose = false;
	vec_set_to_sparse_set(sparsity, p, n, n, transpose);
	
	// compute the Hessian
	size_t n_sweep = SparseHessianCompute(x, w, sparsity, hes, work);

	return n_sweep;
}
// ===========================================================================
/*!
Private helper function for SparseHessian(x, w, p).

All of the description in the public member function SparseHessian(x, w, p)
applies.

\param set_type
is a \c bool value. This argument is used to dispatch to the proper source
code depending on the value of \c VectorSet::value_type.

\param x
See \c SparseHessian(x, w, p).

\param w
See \c SparseHessian(x, w, p).

\param p
See \c SparseHessian(x, w, p).

\param hes
is the return value for the corresponding call to \c SparseHessian(x, w, p).
On input, it must have size equal to the domain times range dimension
for this ADFun<Base> object.
On return, it will contain the Hessian.
*/

template <class Base>
template <class VectorBase, class VectorSet>
void ADFun<Base>::SparseHessianCase(
	bool               set_type         ,
	const VectorBase&  x                ,
	const VectorBase&  w                ,
	const VectorSet&   p                ,
	VectorBase&        hes              )
{
	size_t n = Domain();
	size_t i, j, k;

	// check Vector is Simple Vector class with bool elements
	CheckSimpleVector<bool, VectorSet>();

	// check Vector is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CPPAD_ASSERT_KNOWN(
		x.size() == n,
		"SparseHessian: size of x not equal domain dimension for f"
	); 
	CPPAD_ASSERT_KNOWN(
		w.size() == Range(),
		"SparseHessian: size of w not equal range dimension for f"
	);
	CPPAD_ASSERT_KNOWN(
		p.size() == n * n,
		"SparseHessian: using bool values and size of p "
		"not equal square of domain dimension for f"
	);
	sparse_hessian_work work;
	CppAD::vector<size_t>& r(work.r_sort);
	CppAD::vector<size_t>& c(work.c_sort);
	CppAD::vector<size_t>& user_k(work.k_sort);

	k = 0;
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
			if( p[i * n + j] )
				k++;
	}
	size_t K = k;
	VectorBase H(K);
	r.resize(K+1);
	c.resize(K);
	user_k.resize(K);
	k = 0;
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
			if( p[i * n + j] )
			{	r[k]      = i;
				c[k]      = j;
				user_k[k] = k;  
				k++;
			}
	}
	r[K] = n;
 
# if 0
	sparse_pack sparsity;
	bool transpose = false;
	vec_bool_to_sparse_pack(sparsity, p, n, n, transpose);
	
	// compute the Hessian
	SparseHessianCompute(x, w, sparsity, H, work);
# else
	SparseHessianCase(set_type, x, w, p, H, work);
# endif

	Base zero(0);
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
			hes[i * n + j] = zero; 
	}
	for(k = 0; k < K; k++)
		hes[r[k] * n + c[k]] = H[k]; 
}
/*!
Private helper function for SparseHessian(x, w, p).

All of the description in the public member function SparseHessian(x, w, p)
applies.

\param set_type
is a \c std::set<size_t> value.
This argument is used to dispatch to the proper source
code depending on the value of \c VectorSet::value_type.

\param x
See \c SparseHessian(x, w, p).

\param w
See \c SparseHessian(x, w, p).

\param p
See \c SparseHessian(x, w, p).

\param hes
is the return value for the corresponding call to \c SparseHessian(x, w, p).
On input, it must have size equal to the domain times range dimension
for this ADFun<Base> object.
On return, it will contain the Hessian.
*/

template <class Base>
template <class VectorBase, class VectorSet>
void ADFun<Base>::SparseHessianCase(
	const std::set<size_t>&  set_type         ,
	const VectorBase&        x                ,
	const VectorBase&        w                ,
	const VectorSet&         p                ,
	VectorBase&              hes              )
{
	size_t n = Domain();
	size_t i, j, k;

	// check VectorSet is Simple Vector class with sets for elements
	CheckSimpleVector<std::set<size_t>, VectorSet>(
		one_element_std_set<size_t>(), two_element_std_set<size_t>()
	);

	// check Vector is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CPPAD_ASSERT_KNOWN(
		x.size() == n,
		"SparseHessian: size of x not equal domain dimension for f"
	); 
	CPPAD_ASSERT_KNOWN(
		w.size() == Range(),
		"SparseHessian: size of w not equal range dimension for f"
	);
	CPPAD_ASSERT_KNOWN(
		p.size() == n,
		"SparseHessian: using size_t sets for sparsity pattern and p.size() "
		"not equal domain dimension for f"
	);
	sparse_hessian_work work;
	CppAD::vector<size_t>& r(work.r_sort);
	CppAD::vector<size_t>& c(work.c_sort);
	CppAD::vector<size_t>& user_k(work.k_sort);

	k = 0;
	std::set<size_t>::const_iterator itr;
	for(i = 0; i < n; i++)
	{	itr = p[i].begin();
		while( itr != p[i].end() )
		{	itr++;
			k++;
		}
	}
	size_t K = k;
	VectorBase H(K);
	r.resize(K+1);
	c.resize(K);
	user_k.resize(K);
	k = 0;
	for(i = 0; i < n; i++)
	{ 	itr = p[i].begin();
		while( itr != p[i].end() )
		{	r[k]      = i;
			c[k]      = *itr;
			user_k[k] = k;  
			itr++;
			k++;
		}
	}
	r[K] = n;
 
# if 0
	sparse_set sparsity;
	bool transpose = false;
	vec_set_to_sparse_set(sparsity, p, n, n, transpose);
	
	// compute the Hessian
	SparseHessianCompute(x, w, sparsity, H, work);
# else
	SparseHessianCase(set_type, x, w, p, H, work);
# endif

	Base zero(0);
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
			hes[i * n + j] = zero; 
	}
	for(k = 0; k < K; k++)
		hes[r[k] * n + c[k]] = H[k]; 
}
// ===========================================================================
// Public Member Functions
// ===========================================================================
/*!
Compute user specified subset of a sparse Hessian.

The C++ source code corresponding to this operation is
\verbatim
	SparceHessian(x, w, p, r, c, hes, work)
\endverbatim

\tparam Base
is the base type for the recording that is stored in this ADFun<Base object.

\tparam VectorBase
is a simple vector class with elements of type \a Base.

\tparam VectorSet
is a simple vector class with elements of type 
\c bool or \c std::set<size_t>.

\tparam VectorSize
is a simple vector class with elements of type \c size_t.

\param x
is a vector specifing the point at which to compute the Hessian.

\param w
is the weighting vector that defines a scalar valued function by
a weighted sum of the components of the vector valued function
$latex F(x)$$.

\param p
is the sparsity pattern for the Hessian that we are calculating.

\param r
is the vector of row indices for the returned Hessian values.

\param c
is the vector of columns indices for the returned Hessian values.
It must have the same size are r.

\param hes
is the vector of Hessian values.
It must have the same size are r. 
The return value <code>hes[k]</code> is the second partial of 
$latex w^{\rm T} F(x)$$ with respect to the
<code>r[k]</code> and <code>c[k]</code> component of $latex x$$.

\param work
contains information that depends on the function object, sparsity pattern,
\c r, and \c c vector.
If these values are the same, \c work does not need to be recomputed.
To be more specific, 
\c r_sort is sorted copy of \c r ,
\c c_sort is sorted copy of \c c ,
<code>k_sort[k]</code> is the original index corresponding to the
values <code>r_sort[k]</code> and <code>c_sort[k]</code>.
The order for the sort is by columns.
Let \c n the domain dimension,
and \c K the size of \c r , \c c , and \c hes.
There is one extra entry 
in the sorted row array and it has value <code>r_sort[K]=n</code>.
The \c color vector is set and used by \c SparseHessianCompute.

\return
Is the number of first order forward sweeps used to compute the
requested Hessian values.
(This is also equal to the number of second order reverse sweeps.)
The total work, not counting the zero order
forward sweep, or the time to combine computations, is proportional to this
return value.
*/
template<class Base>
template <class VectorBase, class VectorSet, class VectorSize>
size_t ADFun<Base>::SparseHessian(
	const VectorBase&     x    ,
	const VectorBase&     w    ,
	const VectorSet&      p    ,
	const VectorSize&     r    ,
	const VectorSize&     c    ,
	VectorBase&           hes  ,
	sparse_hessian_work&  work )
{
	size_t n = Domain();
	size_t k, K = hes.size();
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
		work.r_sort[K] = n;
		thread_alloc::return_memory(v_ptr);
	}
# ifndef NDEBUG
	CPPAD_ASSERT_KNOWN(
		x.size() == n ,
		"SparseHessian: size of x not equal domain dimension for f."
	); 
	CPPAD_ASSERT_KNOWN(
		r.size() == K && c.size() == K ,
		"SparseHessian: either r or c does not have the same size as ehs."
	); 
	CPPAD_ASSERT_KNOWN(
		work.r_sort.size() == K+1 &&
		work.c_sort.size() == K   &&
		work.k_sort.size() == K   ,
		"SparseHessian: invalid value in work."
	);
	CPPAD_ASSERT_KNOWN(
		work.color.size() == 0 || work.color.size() == n,
		"SparseHessian: invalid value in work."
	);
	for(k = 0; k < K; k++)
	{	CPPAD_ASSERT_KNOWN(
			r[k] < n,
			"SparseHessian: invalid value in r."
		);
		CPPAD_ASSERT_KNOWN(
			c[k] < n,
			"SparseHessian: invalid value in c."
		);
		CPPAD_ASSERT_KNOWN(
			work.k_sort[k] < K,
			"SparseHessian: invalid value in work."
		);
		CPPAD_ASSERT_KNOWN(
			work.r_sort[k] == r[ work.k_sort[k] ] ,
			"SparseHessian: invalid value in work."
		);
		CPPAD_ASSERT_KNOWN(
			work.c_sort[k] == c[ work.k_sort[k] ],
			"SparseHessian: invalid value in work."
		);
	}
	if( work.color.size() != 0 )
		for(size_t j = 0; j < n; j++) CPPAD_ASSERT_KNOWN(
			work.color[j] < n,
			"SparseHessian: invalid value in work."
	);
# endif
 
	typedef typename VectorSet::value_type Set_type;
	size_t n_sweep = SparseHessianCase(Set_type(), x, w, p, hes, work);
	return n_sweep;
}
/*!
Compute a sparse Hessian.

The C++ source code coresponding to this operation is
\verbatim
	hes = SparseHessian(x, w, p)
\endverbatim


\tparam Base
is the base type for the recording that is stored in this
ADFun<Base object.

\tparam VectorBase
is a simple vector class with elements of the \a Base.

\tparam VectorSet
is a simple vector class with elements of type
\c bool or \c std::set<size_t>.

\param x
is a vector specifing the point at which to compute the Hessian.

\param w
The Hessian is computed for a weighted sum of the components
of the function corresponding to this ADFun<Base> object.
The argument \a w specifies the weights for each component.
It must have size equal to the range dimension for this ADFun<Base> object.

\param p
is a sparsity pattern for the Hessian.

\return
Will be a vector of size \c n * n containing the Hessian of 
at the point specified by \a x
(where \c n is the domain dimension for this ADFun<Base> object).
*/
template <class Base>
template <class VectorBase, class VectorSet>
VectorBase ADFun<Base>::SparseHessian(
	const VectorBase& x, const VectorBase& w, const VectorSet& p
)
{	size_t n = Domain();
	VectorBase hes(n * n);

	typedef typename VectorSet::value_type Set_type;
	SparseHessianCase(Set_type(), x, w, p, hes);

	return hes;
}
/*!
Compute a sparse Hessian

The C++ source code coresponding to this operation is
\verbatim
	hes = SparseHessian(x, w)
\endverbatim


\tparam Base
is the base type for the recording that is stored in this
ADFun<Base object.

\tparam VectorBase
is a simple vector class with elements of the \a Base.

\param x
is a vector specifing the point at which to compute the Hessian.

\param w
The Hessian is computed for a weighted sum of the components
of the function corresponding to this ADFun<Base> object.
The argument \a w specifies the weights for each component.
It must have size equal to the range dimension for this ADFun<Base> object.

\return
Will be a vector of size \c n * n containing the Hessian of 
at the point specified by \a x
(where \c n is the domain dimension for this ADFun<Base> object).
*/
template <class Base>
template <class VectorBase>
VectorBase ADFun<Base>::SparseHessian(const VectorBase &x, const VectorBase &w)
{	size_t i, j, k;
	typedef CppAD::vectorBool VectorBool;

	size_t m = Range();
	size_t n = Domain();

	// determine the sparsity pattern p for Hessian of w^T F
	VectorBool r(n * n);
	for(j = 0; j < n; j++)
	{	for(k = 0; k < n; k++)
			r[j * n + k] = false;
		r[j * n + j] = true;
	}
	ForSparseJac(n, r);
	//
	VectorBool s(m);
	for(i = 0; i < m; i++)
		s[i] = w[i] != 0;
	VectorBool p = RevSparseHes(n, s);

	// compute sparse Hessian
	VectorBase hes(n * n);
	bool set_type = true; // only used to dispatch compiler to proper case
	SparseHessianCase(set_type, x, w, p, hes);

	return hes;
}

CPPAD_END_NAMESPACE
# endif
