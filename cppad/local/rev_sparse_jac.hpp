/* $Id$ */
# ifndef CPPAD_REV_SPARSE_JAC_INCLUDED
# define CPPAD_REV_SPARSE_JAC_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin RevSparseJac$$
$spell
	optimizer
	nz
	CondExpRel
	std
	VecAD
	var
	Jacobian
	Jac
	const
	Bool
	Dep
	proportional
$$

$section Jacobian Sparsity Pattern: Reverse Mode$$ 

$index RevSparseJac$$
$index reverse, sparse Jacobian$$
$index sparse, reverse Jacobian$$
$index pattern, reverse Jacobian$$

$head Syntax$$
$icode%s% = %f%.RevSparseJac(%q%, %r%)
%$$
$icode%s% = %f%.RevSparseJac(%q%, %r%, %transpose%)%$$

$head Purpose$$
We use $latex F : B^n \rightarrow B^m$$ to denote the
$cref/AD function/glossary/AD Function/$$ corresponding to $icode f$$.
For a fixed matrix $latex R \in B^{q \times m}$$,
the Jacobian of $latex R * F( x )$$
with respect to $latex x$$ is
$latex \[
	S(x) = R * F^{(1)} ( x )
\] $$
Given a
$cref/sparsity pattern/glossary/Sparsity Pattern/$$ 
for $latex R$$,
$code RevSparseJac$$ returns a sparsity pattern for the $latex S(x)$$.

$head f$$
The object $icode f$$ has prototype
$codei%
	ADFun<%Base%> %f%
%$$

$head x$$
the sparsity pattern is valid for all values of the independent 
variables in $latex x \in B^n$$
(even if it has $cref CondExp$$ or $cref VecAD$$ operations).

$head q$$
The argument $icode q$$ has prototype
$codei%
	size_t %q%
%$$
It specifies the number of rows in
$latex R \in B^{q \times m}$$ and the 
Jacobian $latex S(x) \in B^{q \times n}$$. 

$head transpose$$
The argument $icode transpose$$ has prototype
$codei%
	bool %transpose%
%$$
The default value $code false$$ is used when $icode transpose$$ is not present.

$head nz_compare$$
The argument $icode nz_compare$$ has prototype
$codei%
	bool %nz_compare%
%$$
If $icode nz_compare$$ is true,
the derivatives with respect to left and right of the 
$cref CondExp$$ below are considered to be non-zero:
$codei%
	%CondExp%Rel%(%left%, %right%, %if_true%, %if_false%)
%$$
This is used by the 
$cref/optimizer/optimize/$$ with $cref checkpoint$$ functions
to obtain the correct dependency relations.
The default value $icode%nz_compare% = false%$$ is used when 
$icode nz_compare$$ is not present.

$head r$$
The argument $icode s$$ has prototype
$codei%
	const %VectorSet%& %r%
%$$
see $cref/VectorSet/RevSparseJac/VectorSet/$$ below.

$subhead transpose false$$
If $icode r$$ has elements of type $code bool$$,
its size is $latex q * m$$.
If it has elements of type $code std::set<size_t>$$,
its size is $icode q$$ and all its set elements are between
zero and $latex m - 1$$.
It specifies a 
$cref/sparsity pattern/glossary/Sparsity Pattern/$$ 
for the matrix $latex R \in B^{q \times m}$$.

$subhead transpose true$$
If $icode r$$ has elements of type $code bool$$,
its size is $latex m * q$$.
If it has elements of type $code std::set<size_t>$$,
its size is $icode m$$ and all its set elements are between
zero and $latex q - 1$$.
It specifies a 
$cref/sparsity pattern/glossary/Sparsity Pattern/$$ 
for the matrix $latex R^\R{T} \in B^{m \times q}$$.

$head s$$
The return value $icode s$$ has prototype
$codei%
	%VectorSet% %s%
%$$
see $cref/VectorSet/RevSparseJac/VectorSet/$$ below.

$subhead transpose false$$
If it has elements of type $code bool$$,
its size is $latex q * n$$.
If it has elements of type $code std::set<size_t>$$,
its size is $icode q$$ and all its set elements are between
zero and $latex n - 1$$.
It specifies a 
$cref/sparsity pattern/glossary/Sparsity Pattern/$$ 
for the matrix $latex S(x) \in {q \times n}$$.

$subhead transpose true$$
If it has elements of type $code bool$$,
its size is $latex n * q$$.
If it has elements of type $code std::set<size_t>$$,
its size is $icode n$$ and all its set elements are between
zero and $latex q - 1$$.
It specifies a 
$cref/sparsity pattern/glossary/Sparsity Pattern/$$ 
for the matrix $latex S(x)^\R{T} \in {n \times q}$$.

$head VectorSet$$
The type $icode VectorSet$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$code bool$$ or $code std::set<size_t>$$;
see $cref/sparsity pattern/glossary/Sparsity Pattern/$$ for a discussion
of the difference.

$head Entire Sparsity Pattern$$
Suppose that $latex q = m$$ and
$latex R$$ is the $latex m \times m$$ identity matrix.
In this case, 
the corresponding value for $icode s$$ is a 
sparsity pattern for the Jacobian $latex S(x) = F^{(1)} ( x )$$.

$head Example$$
$children%
	example/rev_sparse_jac.cpp
%$$
The file
$cref rev_sparse_jac.cpp$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

# include <cppad/local/std_set.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\defgroup rev_sparse_jac_hpp rev_sparse_jac.hpp
\{
\file rev_sparse_jac.hpp
Reverse mode Jacobian sparsity patterns.
*/

// -------------------------------------------------------------------------
/*!
Calculate Jacobian vector of bools sparsity patterns using reverse mode.

The C++ source code corresponding to this operation is
\verbatim
	s = f.RevSparseJac(q, r)
\endverbatim

\tparam Base
is the base type for this recording.

\tparam VectorSet
is a simple vector class with elements of type \c bool.

\param transpose
are the sparsity patterns transposed.

\param nz_compare
Are the derivatives with respect to left and right of the expression below
considered to be non-zero:
\code
	CondExpRel(left, right, if_true, if_false)
\endcode
This is used by the optimizer to obtain the correct dependency relations.

\param q
is the number of rows in the matrix \f$ R \f$.

\param r
is a sparsity pattern for the matrix \f$ R \f$.

\param s
the input value of \a s must be a vector with size <tt>p*n</tt>
where \c n is the number of independent variables
corresponding to the operation sequence stored in \a play. 
The input value of the components of \c s does not matter.
On output, \a s is the sparsity pattern for the matrix
\f[
	S(x) = R * F^{(1)} (x)
\f]
where \f$ F \f$ is the function corresponding to the operation sequence
and \a x is any argument value.

\param total_num_var
is the total number of variable in this recording.

\param dep_taddr
maps dependendent variable index
to the corresponding variable in the tape.

\param ind_taddr
maps independent variable index
to the corresponding variable in the tape.

\param play
is the recording that defines the function we are computing the sparsity 
pattern for.
*/

template <class Base, class VectorSet> 
void RevSparseJacBool(
	bool                   transpose        ,
	bool                   nz_compare       ,
	size_t                 q                , 
	const VectorSet&       r                ,
	VectorSet&             s                ,
	size_t                 total_num_var    ,
	CppAD::vector<size_t>& dep_taddr        ,
	CppAD::vector<size_t>& ind_taddr        ,
	CppAD::player<Base>&   play             )
{
	// temporary indices
	size_t i, j;

	// check VectorSet is Simple Vector class with bool elements
	CheckSimpleVector<bool, VectorSet>();

	// range and domain dimensions for F
	size_t m = dep_taddr.size();
	size_t n = ind_taddr.size();

	CPPAD_ASSERT_KNOWN(
		q > 0,
		"RevSparseJac: q is not greater than zero"
	);
	CPPAD_ASSERT_KNOWN(
		size_t(r.size()) == q * m,
		"RevSparseJac: size of r is not equal to\n"
		"q times range dimension for ADFun object."
	);

	// vector of sets that will hold the results
	sparse_pack    var_sparsity;
	var_sparsity.resize(total_num_var, q);

	// The sparsity pattern corresponding to the dependent variables
	for(i = 0; i < m; i++)
	{	CPPAD_ASSERT_UNKNOWN( dep_taddr[i] < total_num_var );
		if( transpose )
		{	for(j = 0; j < q; j++) if( r[ j * m + i ] )
				var_sparsity.add_element( dep_taddr[i], j );
		}
		else
		{	for(j = 0; j < q; j++) if( r[ i * q + j ] )
				var_sparsity.add_element( dep_taddr[i], j );
		}
	}

	// evaluate the sparsity patterns
	RevJacSweep(
		nz_compare,
		n,
		total_num_var,
		&play,
		var_sparsity
	);

	// return values corresponding to dependent variables
	CPPAD_ASSERT_UNKNOWN( size_t(s.size()) == q * n );
	for(j = 0; j < n; j++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr[j] == (j+1) );

		// ind_taddr[j] is operator taddr for j-th independent variable
		CPPAD_ASSERT_UNKNOWN( play.GetOp( ind_taddr[j] ) == InvOp );

		// extract the result from var_sparsity
		if( transpose )
		{	for(i = 0; i < q; i++)
				s[ j * q + i ] = false;
		}
		else
		{	for(i = 0; i < q; i++) 
				s[ i * n + j ] = false;
		}
		CPPAD_ASSERT_UNKNOWN( var_sparsity.end() == q );
		var_sparsity.begin(j+1);
		i = var_sparsity.next_element();
		while( i < q )
		{	if( transpose )
				s[ j * q + i ] = true;
			else	s[ i * n + j ] = true;
			i  = var_sparsity.next_element();
		}
	}
}
/*!
Calculate Jacobian vector of sets sparsity patterns using reverse mode.

The C++ source code corresponding to this operation is
\verbatim
	s = f.RevSparseJac(q, r)
\endverbatim

\tparam Base
see \c RevSparseJacBool.

\tparam VectorSet
is a simple vector class with elements of type \c std::set<size_t>.

\param transpose
see \c RevSparseJacBool.

\param nz_compare
see \c RevSparseJacBool.

\param q
see \c RevSparseJacBool.

\param r
see \c RevSparseJacBool.

\param s
see \c RevSparseJacBool.

\param total_num_var
see \c RevSparseJacBool.

\param dep_taddr
see \c RevSparseJacBool.

\param ind_taddr
see \c RevSparseJacBool.

\param play
see \c RevSparseJacBool.
*/
template <class Base, class VectorSet> 
void RevSparseJacSet(
	bool                   transpose        ,
	bool                   nz_compare       ,
	size_t                 q                , 
	const VectorSet&       r                ,
	VectorSet&             s                ,
	size_t                 total_num_var    ,
	CppAD::vector<size_t>& dep_taddr        ,
	CppAD::vector<size_t>& ind_taddr        ,
	CppAD::player<Base>&   play             )
{
	// temporary indices
	size_t i, j;
	std::set<size_t>::const_iterator itr;

	// check VectorSet is Simple Vector class with sets for elements
	CheckSimpleVector<std::set<size_t>, VectorSet>(
		one_element_std_set<size_t>(), two_element_std_set<size_t>()
	);

	// domain dimensions for F
	size_t n = ind_taddr.size();
	size_t m = dep_taddr.size();

	CPPAD_ASSERT_KNOWN(
		q > 0,
		"RevSparseJac: q is not greater than zero"
	);
	CPPAD_ASSERT_KNOWN(
		size_t(r.size()) == q || transpose,
		"RevSparseJac: size of r is not equal to q and transpose is false."
	);
	CPPAD_ASSERT_KNOWN(
		size_t(r.size()) == m || ! transpose,
		"RevSparseJac: size of r is not equal to m and transpose is true."
	);

	// vector of lists that will hold the results
	CPPAD_INTERNAL_SPARSE_SET    var_sparsity;
	var_sparsity.resize(total_num_var, q);

	// The sparsity pattern corresponding to the dependent variables
	if( transpose )
	{	for(i = 0; i < m; i++)
		{	itr = r[i].begin();
			while(itr != r[i].end())
			{	j = *itr++; 
				CPPAD_ASSERT_KNOWN(
				j < q,
				"RevSparseJac: transpose is true and element of the set\n"
				"r[i] has value greater than or equal q."
				);
				CPPAD_ASSERT_UNKNOWN( dep_taddr[i] < total_num_var );
				var_sparsity.add_element( dep_taddr[i], j );
			}
		}
	}
	else
	{	for(i = 0; i < q; i++)
		{	itr = r[i].begin();
			while(itr != r[i].end())
			{	j = *itr++; 
				CPPAD_ASSERT_KNOWN(
				j < m,
				"RevSparseJac: transpose is false and element of the set\n"
				"r[i] has value greater than or equal range dimension."
				);
				CPPAD_ASSERT_UNKNOWN( dep_taddr[j] < total_num_var );
				var_sparsity.add_element( dep_taddr[j], i );
			}
		}
	}
	// evaluate the sparsity patterns
	RevJacSweep(
		nz_compare,
		n,
		total_num_var,
		&play,
		var_sparsity
	);

	// return values corresponding to dependent variables
	CPPAD_ASSERT_UNKNOWN( size_t(s.size()) == q || transpose );
	CPPAD_ASSERT_UNKNOWN( size_t(s.size()) == n || ! transpose );
	for(j = 0; j < n; j++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr[j] == (j+1) );

		// ind_taddr[j] is operator taddr for j-th independent variable
		CPPAD_ASSERT_UNKNOWN( play.GetOp( ind_taddr[j] ) == InvOp );

		CPPAD_ASSERT_UNKNOWN( var_sparsity.end() == q );
		var_sparsity.begin(j+1);
		i = var_sparsity.next_element();
		while( i < q )
		{	if( transpose )
				s[j].insert(i);
			else	s[i].insert(j);
			i = var_sparsity.next_element();
		}
	}
}
// --------------------------------------------------------------------------

/*!
Private helper function for \c RevSparseJac(q, r, transpose).

All of the description in the public member function 
\c RevSparseJac(q, r, transpose) apply.

\param set_type
is a \c bool value.
This argument is used to dispatch to the proper source code
depending on the value of \c VectorSet::value_type.

\param transpose
See \c RevSparseJac(q, r, transpose, nz_compare)

\param nz_compare
See \c RevSparseJac(q, r, transpose, nz_compare)

\param q
See \c RevSparseJac(q, r, transpose, nz_compare)

\param r
See \c RevSparseJac(q, r, transpose, nz_compare)

\param s
is the return value for the corresponding call to 
RevSparseJac(q, r, transpose).
*/

template <class Base>
template <class VectorSet>
void ADFun<Base>::RevSparseJacCase(
	bool                set_type          ,
	bool                transpose         ,
	bool                nz_compare        ,
	size_t              q                 ,
	const VectorSet&    r                 ,
	VectorSet&          s                 )
{	size_t n = Domain();

	// dimension of the result vector
	s.resize( q * n );

	// store results in s
	RevSparseJacBool(
		transpose      ,
		nz_compare     ,
		q              ,
		r              ,
		s              ,
		total_num_var_ ,
		dep_taddr_     ,
		ind_taddr_     ,
		play_
	);
}

/*!
Private helper function for \c RevSparseJac(q, r, transpose).

All of the description in the public member function 
\c RevSparseJac(q, r, transpose) apply.

\param set_type
is a \c std::set<size_t> object.
This argument is used to dispatch to the proper source code
depending on the value of \c VectorSet::value_type.

\param transpose
See \c RevSparseJac(q, r, transpose, nz_compare)

\param nz_compare
See \c RevSparseJac(q, r, transpose, nz_compare)

\param q
See \c RevSparseJac(q, r, transpose, nz_compare)

\param r
See \c RevSparseJac(q, r, transpose, nz_compare)

\param s
is the return value for the corresponding call to RevSparseJac(q, r, transpose)
*/

template <class Base>
template <class VectorSet>
void ADFun<Base>::RevSparseJacCase(
	const std::set<size_t>&      set_type          ,
	bool                         transpose         ,
	bool                         nz_compare        ,
	size_t                       q                 ,
	const VectorSet&             r                 ,
	VectorSet&                   s                 )
{	// dimension of the result vector
	if( transpose )
		s.resize( Domain() );
	else	s.resize( q );

	// store results in r
	RevSparseJacSet(
		transpose      ,
		nz_compare     ,
		q              ,
		r              ,
		s              ,
		total_num_var_ ,
		dep_taddr_     ,
		ind_taddr_     ,
		play_
	);
}
// --------------------------------------------------------------------------
/*!
User API for Jacobian sparsity patterns using reverse mode.

The C++ source code corresponding to this operation is
\verbatim
	s = f.RevSparseJac(q, r, transpose, nz_compare)
\endverbatim

\tparam Base
is the base type for this recording.

\tparam VectorSet
is a simple vector with elements of type \c bool.
or \c std::set<size_t>.

\param q
is the number of rows in the matrix \f$ R \f$.

\param r
is a sparsity pattern for the matrix \f$ R \f$.

\param transpose
are the sparsity patterns for \f$ R \f$ and \f$ S(x) \f$ transposed.

\param nz_compare
Are the derivatives with respect to left and right of the expression below
considered to be non-zero:
\code
	CondExpRel(left, right, if_true, if_false)
\endcode
This is used by the optimizer to obtain the correct dependency relations.


\return
If \c transpose is false (true), the return value is a sparsity pattern
for \f$ S(x) \f$ (\f$ S(x)^T \f$) where
\f[
	S(x) = R * F^{(1)} (x)
\f]
and \f$ F \f$ is the function corresponding to the operation sequence
and \a x is any argument value.
If \c VectorSet::value_type is \c bool,
the return value has size \f$ q * n \f$ ( \f$ n * q \f$).
If \c VectorSet::value_type is \c std::set<size_t>,
the return value has size \f$ q \f$ ( \f$ n \f$)
and with all its elements between zero and \f$ n - 1 \f$ (\f$ q - 1 \f$).
*/
template <class Base>
template <class VectorSet>
VectorSet ADFun<Base>::RevSparseJac(
	size_t              q          , 
	const VectorSet&    r          ,
	bool                transpose  ,
	bool                nz_compare )
{
	VectorSet s;
	typedef typename VectorSet::value_type Set_type;

	RevSparseJacCase(
		Set_type()    ,
		transpose     ,
		nz_compare    ,
		q             ,
		r             ,
		s
	);
	return s;
}

/*! \} */
} // END_CPPAD_NAMESPACE
# endif
