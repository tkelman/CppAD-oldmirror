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
$icode%jac% = %f%.SparseJacobian(%x%, %p%)%$$

$head Purpose$$
We use $latex F : \R^n \rightarrow \R^m$$ do denote the
$cref/AD function/glossary/AD Function/$$
corresponding to $icode f$$. 
The syntax above sets $icode jac$$ to the Jacobian 
$latex \[
	jac = F^{(1)} (x) 
\] $$
This routine assumes
that the matrix $latex F^{(1)} (x) \in \R^{m \times n}$$ is sparse
and uses this assumption to reduce the amount of computation necessary.
One should use speed tests (e.g. $cref speed_test$$)
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

$head jac$$
The result $icode jac$$ has prototype
$codei%
	%VectorBase% %jac%
%$$
and its size is $latex m * n$$.
For $latex i = 0 , \ldots , m - 1$$,
and $latex j = 0 , \ldots , n - 1 $$ 
$latex \[
	jac [ i * n + j ] = \D{ F_i }{ x_j } (x)
\] $$

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
-----------------------------------------------------------------------------
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
is the vector that varies first in the ordering being checked for.

\param minor
is the vector that varies second in the ordering being checked for.

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

	CPPAD_ASSERT_UNKNOWN( major.size() == minor.size() ); 
	size_t k, K = major.size();
	size_t major_previous = major[0];
	size_t minor_previous = minor[0];
	for(k = 1; k < K; k++)
	{	match &= minor_previous <= minor[k];
		if( minor_previous == minor[k] )
			match &= major_previous < major[k];
		major_previous = major[k];
		minor_previous = minor[k];
	} 
	return match;
}
// ===========================================================================
/*!
Private helper function SparseJacobianForward(x, p, r, c, jac).

All descriptions in the public function SparseJacobian(x, p, r, c, jac) apply.

\param x
See \c SparseJacobian(x, p, r, c, jac).

\param p
See \c SparseJacobian(x, p, r, c, jac).
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
*/
template<class Base>
template <class VectorBase, class VectorSet, class VectorSize>
void ADFun<Base>::SparseJacobianForward(
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
	CPPAD_ASSERT_UNKNOWN( r.size() == c.size() && r.size() == jac.size() ); 
	CPPAD_ASSERT_UNKNOWN( p.n_set() ==  m );
	CPPAD_ASSERT_UNKNOWN( p.end() ==  n );

	// Assume row major order
	CPPAD_ASSERT_UNKNOWN( is_major_minor_order(r, c) );

	// Point at which we are evaluating the Jacobian
	Forward(0, x);

	// initialize the return value
	size_t K = jac.size();
	for(k = 0; k < K; k++)
		jac[k] = zero;

	// mapping from column number to index used for coloring,
	// where the value n means that this column is not used.
	VectorSize used_index(n);
	for(j = 0; j < n; j++)
		used_index[j] = n;
	size_t n_used = 0;
	for(k = 0; k < K; k++)
	{	if( used_index[c[k]] == n )
		{	used_index[c[k]] = n_used++;
		}
	}
	CPPAD_ASSERT_UNKNOWN( n_used <= n );

	// mapping from used index to original column index
	VectorSize column(n_used);
	size_t j_used = 0;
	for(j = 0; j < n; j++)
	{	if( used_index[j] != n )
		{	column[j_used++] = j;
			CPPAD_ASSERT_UNKNOWN( used_index[j] == j_used-1 );
		}
	}
	CPPAD_ASSERT_UNKNOWN( j_used == n_used );

	// initial coloring
	VectorSize color(n_used);
	for(j_used = 0; j_used < n_used; j_used++)
		color[j_used] = j_used;

	// See GreedyPartialD2Coloring Algorithm Section 3.6.2 of
	// Graph Coloring in Optimization Revisited by
	// Assefaw Gebremedhin, Fredrik Maane, Alex Pothen
	vectorBool forbidden(n_used);
	k = 0;
	for(j_used = 0; j_used < n_used; j_used++)
	{
		// initial all colors as ok for this column
		for(ell = 0; ell < n_used; ell++)
			forbidden[ell] = false;

		// advance k to first index for this column
		CPPAD_ASSERT_UNKNOWN( c[k] <= column[j_used] )
		while( c[k] < column[j_used] )
		{	CPPAD_ASSERT_UNKNOWN( k < K-1 );
			k++;
		}
		CPPAD_ASSERT_UNKNOWN( c[k] == column[j_used] );

		// for each row that we will use a value from this column 
		// (note that the set in use may be smaller than sparsity pattern).
		i = r[k];
		while( k < K && c[k] == column[j_used] )
		{	// for each column that is connected to this row
			p.begin(i);
			j = p.next_element();
			while( j != p.end() )
			{	ell = used_index[j];	
				// if this is not the same column and we want values for it
				if( (column[j_used] != j) & (ell != n) )
				{	CPPAD_ASSERT_UNKNOWN( ell < n_used );
					forbidden[ color[ell] ] = true;
				}
				j = p.next_element();
			}
			k++;
		}
		CPPAD_ASSERT_UNKNOWN( k < K || j_used == (n_used - 1) );

		ell = 0;
		while( forbidden[ell] && ell < n_used )
		{	ell++;
			CPPAD_ASSERT_UNKNOWN( ell < j_used );
		}
		color[j_used] = ell;
	}
	size_t n_color = 1;
	for(ell = 0; ell < n_used; ell++) 
		n_color = std::max(n_color, color[ell] + 1);

	// direction vector for calls to forward
	VectorBase dx(n);

	// location for return values from forward
	VectorBase dy(m);

	// loop over colors
	size_t i_color;
	for(i_color = 0; i_color < n_color; i_color++)
	{	for(j = 0; j < n; j++)
			dx[j] = zero;
		// determine all the colums with this color
		for(ell = 0; ell < n_used; ell++)
		{	if( color[ell] == i_color )
				dx[column[ell]] = one;
		}
		// call forward mode for all these columns at once
		dy = Forward(1, dx);

		// set the corresponding components of the result
		for(ell = 0; ell < n_used; ell++) if( color[ell] == i_color )
		{	// find first index in c for this column
			k = 0;
			while( c[k] != column[ell] )
			{	CPPAD_ASSERT_UNKNOWN( k < K );
				k++;
			}
			// extract the row results for this column
			while( k < K && c[k] == column[ell] ) 
			{	jac[k] = dy[r[k]];
				k++;
			}
		}
	}
}
/*!
Private helper function SparseJacobianReverse(x, p_transpose, r, c, jac).

All descriptions in the public function SparseJacobian(x, p, r, c, jac) apply.

\param x
See \c SparseJacobian(x, p, r, c, jac).

\param p_transpose
Is a sparsity pattern for the transpose of p in 
\c SparseJacobian(x, p, r, c, jac).
Note that we do not change the values in \c p_transpose,
but is not \c const because we use its iterator facility.

\param r
See \c SparseJacobian(x, p, r, c, jac)
with the additional restriction that \c r , \c c are in column major order.

\param c
See \c SparseJacobian(x, p, r, c, jac).
with the additional restriction that \c r , \c c are in column major order.

\param jac
See \c SparseJacobian(x, p, r, c, jac).
*/
template<class Base>
template <class VectorBase, class VectorSet, class VectorSize>
void ADFun<Base>::SparseJacobianReverse(
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
	CPPAD_ASSERT_UNKNOWN( r.size() == c.size() && r.size() == jac.size() ); 
	CPPAD_ASSERT_UNKNOWN( p_transpose.n_set() ==  n );
	CPPAD_ASSERT_UNKNOWN( p_transpose.end() ==  m );

	// Assume row column order
	CPPAD_ASSERT_UNKNOWN( is_major_minor_order(c, r) );

	// Point at which we are evaluating the Jacobian
	Forward(0, x);

	// initialize the return value
	size_t K = jac.size();
	for(k = 0; k < K; k++)
		jac[k] = zero;

	// mapping from row number to index used for coloring,
	// where the value m means that this column is not used.
	VectorSize used_index(n);
	for(i = 0; i < m; i++)
		used_index[i] = m;
	size_t m_used = 0;
	for(k = 0; k < K; k++)
	{	if( used_index[r[k]] == m )
		{	used_index[r[k]] = m_used++;
		}
	}
	CPPAD_ASSERT_UNKNOWN( m_used <= m );

	// mapping from used index to original row index
	VectorSize row(m_used);
	size_t i_used = 0;
	for(i = 0; i < m; i++)
	{	if( used_index[i] != m )
		{	row[i_used++] = i;
			CPPAD_ASSERT_UNKNOWN( used_index[i] == i_used-1 );
		}
	}
	CPPAD_ASSERT_UNKNOWN( i_used == m_used );

	// initial coloring
	VectorSize color(m_used);
	for(i_used = 0; i_used < m_used; i_used++)
		color[i_used] = i_used;

	// See GreedyPartialD2Coloring Algorithm Section 3.6.2 of
	// Graph Coloring in Optimization Revisited by
	// Assefaw Gebremedhin, Fredrik Maane, Alex Pothen
	vectorBool forbidden(m_used);
	k = 0;
	for(i_used = 0; i_used < m_used; i_used++)
	{
		// initial all colors as ok for this row
		for(ell = 0; ell < m_used; ell++)
			forbidden[ell] = false;

		// advance k to first index for this row
		CPPAD_ASSERT_UNKNOWN( r[k] <= row[i_used] )
		while( r[k] < row[i_used] )
		{	CPPAD_ASSERT_UNKNOWN( k < K-1 );
			k++;
		}
		CPPAD_ASSERT_UNKNOWN( r[k] == row[i_used] );

		// for each column that we will use a value from this row 
		// (note that the set in use may be smaller than sparsity pattern).
		j = c[k];
		while( k < K && r[k] == row[i_used] )
		{	// for each row that is connected to this column
			p_transpose.begin(j);
			i = p_transpose.next_element();
			while( i != p_transpose.end() )
			{	ell = used_index[i];	
				// if this is not the same row and we want values for it
				if( (row[i_used] != i) & (ell != m) )
				{	CPPAD_ASSERT_UNKNOWN( ell < m_used );
					forbidden[ color[ell] ] = true;
				}
				i = p_transpose.next_element();
			}
			k++;
		}
		CPPAD_ASSERT_UNKNOWN( k < K || i_used == (m_used - 1) );

		ell = 0;
		while( forbidden[ell] && ell < m_used )
		{	ell++;
			CPPAD_ASSERT_UNKNOWN( ell < i_used );
		}
		color[i_used] = ell;
	}
	size_t n_color = 1;
	for(ell = 0; ell < m_used; ell++) 
		n_color = std::max(n_color, color[ell] + 1);

	// weighting vector for calls to reverse
	VectorBase w(m);

	// location for return values from Reverse
	VectorBase dw(n);

	// loop over colors
	size_t i_color;
	for(i_color = 0; i_color < n_color; i_color++)
	{	for(i = 0; i < m; i++)
			w[i] = zero;
		// determine all the rows with this color
		for(ell = 0; ell < m_used; ell++)
		{	if( color[ell] == i_color )
				w[row[ell]] = one;
		}
		// call reverse mode for all these rows at once
		dw = Reverse(1, w);

		// set the corresponding components of the result
		for(ell = 0; ell < m_used; ell++) if( color[ell] == i_color )
		{	// find first index in r for this row
			k = 0;
			while( r[k] != row[ell] )
			{	CPPAD_ASSERT_UNKNOWN( k < K );
				k++;
			}
			// extract the row results for this row
			while( k < K && r[k] == row[ell] ) 
			{	jac[k] = dw[c[k]];
				k++;
			}
		}
	}
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
*/
template <class Base>
template <class VectorBase, class VectorSet, class VectorSize>
void ADFun<Base>::SparseJacobianCase(
	bool               set_type        ,
	const VectorBase&  x               , 
	const VectorSet&   p               ,
	const VectorSize&  r               ,
	const VectorSize&  c               ,
	VectorBase&        jac             )
{	size_t n = Domain();
	size_t m = Range();

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
	CPPAD_ASSERT_UNKNOWN( r.size() == c.size() ); 
	CPPAD_ASSERT_UNKNOWN( r.size() == jac.size() ); 

	if( is_major_minor_order(r, c) )
	{	// use forward mode ----------------------------------------

		// convert the sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_pack sparsity;
		bool transpose = false;
		vec_bool_to_sparse_pack(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		SparseJacobianForward(x, sparsity, r, c, jac);
	}
	else
	{	// use reverse mode ----------------------------------------

		// convert the sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_pack sparsity;
		bool transpose = true;
		vec_bool_to_sparse_pack(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		SparseJacobianReverse(x, sparsity, r, c, jac);
	}
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
*/
template <class Base>
template <class VectorBase, class VectorSet, class VectorSize>
void ADFun<Base>::SparseJacobianCase(
	const std::set<size_t>&  set_type        ,
	const VectorBase&        x               , 
	const VectorSet&         p               ,
	const VectorSize&        r               ,
	const VectorSize&        c               ,
	VectorBase&              jac             )
{	size_t n = Domain();
	size_t m = Range();

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
	CPPAD_ASSERT_UNKNOWN( r.size() == c.size() ); 
	CPPAD_ASSERT_UNKNOWN( r.size() == jac.size() ); 

	if( is_major_minor_order(r, c) )
	{	// use forward mode ----------------------------------------

		// convert the sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_pack sparsity;
		bool transpose = false;
		vec_bool_to_sparse_set(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		SparseJacobianForward(x, sparsity, r, c, jac);
	}
	else
	{	// use reverse mode ----------------------------------------

		// convert the sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_pack sparsity;
		bool transpose = true;
		vec_bool_to_sparse_set(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		SparseJacobianReverse(x, sparsity, r, c, jac);
	}
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
	CppAD::vector<size_t> r(K), c(K);
	VectorBase J(K);

	if( n <= m )
	{	// use forward mode ----------------------------------------
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

		// convert the sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_pack sparsity;
		bool transpose = false;
		vec_bool_to_sparse_pack(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		SparseJacobianForward(x, sparsity, r, c, J);
	}
	else
	{	// use reverse mode ----------------------------------------
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

		// convert the sparsity pattern to a sparse_pack object
		// so can fold vector of bools and vector of sets into same function
		sparse_pack sparsity;
		bool transpose = true;
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
	CppAD::vector<size_t> c(K), r(K);
	VectorBase J(K);

	// transposed sparsity pattern
	sparse_set p_transpose;
	bool transpose = true;
	vec_set_to_sparse_set(p_transpose, p, m, n, transpose);

	if( n <= m )
	{	// use forward mode ----------------------------------------
		k = 0;
		for(j = 0; j < n; j++)
		{	p_transpose.begin(j);
			i = p_transpose.next_element();
			while( i != p_transpose.end() )
			{	r[k] = i;
				c[k] = j;
				k++;
				i    = p_transpose.next_element();
			}
		} 

		// also need a non-transposed copy so can fold vector of sets
		// into the r, c, J representation.
		sparse_set sparsity;
		transpose = false;
		vec_set_to_sparse_set(sparsity, p, m, n, transpose);
	
		// now we have folded this into the following case
		SparseJacobianForward(x, sparsity, r, c, J);
	}
	else
	{	// use reverse mode ----------------------------------------
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

		// now we have folded this into the following case
		SparseJacobianReverse(x, p_transpose, r, c, J);
	}
	// initialize the return value
	for(i = 0; i < m; i++)
		for(j = 0; j < n; j++)
			jac[i * n + j] = zero;

	// now set the non-zero return values
	for(k = 0; k < K; k++)
		jac[r[k] * n + c[k]] = J[k];
}
// ============================================================================
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
