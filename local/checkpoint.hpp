/* $Id$ */
# ifndef CPPAD_CHECKPOINT_INCLUDED
# define CPPAD_CHECKPOINT_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
// 2DO: Add and implement choice between sets and bools for sparsity patterns.
//      Overide operator(), and check that id==0, ax.size()==n, ay.size()==m.
//      Use f_.Domain() and f_.Range() to set n and m.

CPPAD_BEGIN_NAMESPACE
/*!
\defgroup checkpoint checkpoint.hpp
\{
\file checkpoint.hpp
defining checkpoint functions.
*/

/*
$begin checkpoint$$
$spell
	checkpoint
	checkpointing
	algo
	afun
	const
$$

$section Checkpointing Functions$$
$index function, checkpoint$$
$index checkpoint, function$$

$head Syntax$$
$icode%checkpoint<%Base%> %afun%(%name%, %algo%, %ax%, %ay%)
%ok% = %afun%(%ax%, %ay%)
%ok% = %algo%(%ax%, %ay%)
checkpoint<%Base%>::clear()%$$

$head Purpose$$
You can reduce the size of the tape and memory required for AD by 
checkpointing functions of the form $latex y = f(x)$$ where
$latex f : B^n \rightarrow B^m$$.

$head constructor$$
The constructor 
$codei%
	checkpoint<%Base%> %afun%(%name%, %algo%, %ax%, %ay%)
%$$
cannot be called in $cref/parallel/ta_in_parallel/$$ mode.
In addition, you cannot currently be recording 
$codei%AD<%Base%>%$$ operations when the constructor is called.
This class is implemented as a derived class of
$cref/atomic_base/atomic_ctor/atomic_base/$$ and hence 
some of its error message will refer to $code atomic_base$$.

$head Base$$
The type $icode Base$$ specifies the base type for AD operations.

$head ADVector$$
The type $icode ADVector$$ must be a
$cref/simple vector class/SimpleVector/$$ with elements of type
$codei%AD<%Base%>%$$.

$head name$$
This $icode checkpoint$$ constructor argument has prototype
$codei%
	const char* %name%
%$$
It is the name used for error reporting.
The suggested value for $icode name$$ is $icode afun$$; i.e.,
the same name as used for the function.

$head ax$$
This argument has prototype
$codei%
	const %ADVector%& %ax%
%$$
and size must be equal to $icode n$$.
It specifies vector $latex x \in B^n$$ 
at which an $codei%AD<%Base%>%$$ version of 
$latex y = f(x)$$ is to be evaluated.

$head ay$$
This argument has prototype
$codei%
	%ADVector%& %ay%
%$$
Its input size must be equal to $icode m$$ and does not change.
The input values of its elements do not matter.
Upon return, it is an $codei%AD<%Base%>%$$ version of 
$latex y = f(x)$$.

$head algo$$
The type of $icode algo$$ is arbitrary, except for the fact that
the syntax
$codei%
	%ok% = %algo%(%ax%, %ay%)
%$$ 
must evaluate the function $latex y = f(x)$$ using
$codei%AD%<%Base%>%$$ operations.
In addition, we assume that the 
$cref/operation sequence/glossary/Operation/Sequence/$$
does not depend on the value of $icode ax$$.

$head afun$$
Given $icode ax$$ it computes the corresponding value of $icode ay$$
using the operation sequence corresponding to $icode algo$$. 
If $codei%AD<%Base%>%$$ operations are being recorded,
it enters the computation as single operation in the recording;
see $cref/start recording/Independent/Start Recording/$$.

$head clear$$
The $code atomic_base$$ class holds onto static work space in order to
increase speed by avoiding system memory allocation calls.
This call makes to work space $cref/available/ta_available/$$ to
for other uses by the same thread.
This should be called when you are done using the 
user atomic functions for a specific value of $icode Base$$.

$subhead Restriction$$
The $code clear$$ routine cannot be called
while in $cref/parallel/ta_in_parallel/$$ execution mode.

$children%
	example/chkpt_simple.cpp
%$$
$head Example$$
The file $cref chkpt_simple.cpp$$ contains an example and test
of these operations.
It returns true if it succeeds and false if it fails.

$end
*/
template <class Base>
class checkpoint : public atomic_base<Base> {
private:
	ADFun<Base> f_;
public:
	/*!
 	Constructor of a checkpoint object

	\param name_in [in]
	is the user's name for the AD version of this atomic operation.

	\param algo [in/out]
	user routine that compute AD function values
	(not const because state may change during evaluation).

	\param ax [in]
	argument value where algo operation sequence is taped.

	\param ay [out]
	function value at specified argument value.
	*/
	template <class Algo, class ADVector>
	checkpoint(const char* name_in, 
		Algo& algo, const ADVector& ax, ADVector& ay)
	: atomic_base<Base>(name_in, true) // name = afun, use_set = true
	{	CheckSimpleVector< CppAD::AD<Base> , ADVector>();

		// make a copy of ax because Independent modifies AD information
		ADVector x_tmp(ax);
	 	Independent(x_tmp);
# ifndef NDEBUG
		bool ok = algo(x_tmp, ay); 
		CPPAD_ASSERT_KNOWN(
			ok,
			"checkpoint constructor: algo returned ok equal to false"
		);
# else
		algo(ax, ay); 
# endif
		f_.Dependent(ay);
	}
	/*!
	Implement the user call to <tt>afun(ax, ay)</tt>.
	
	\tparam ADVector
	A simple vector class with elements of type <code>AD<Base></code>.
	
	\param id
	optional parameter which must be zero if present.
	
	\param ax
	is the argument vector for this call,
	<tt>ax.size()</tt> determines the number of arguments.
	
	\param ay
	is the result vector for this call,
	<tt>ay.size()</tt> determines the number of results.
	*/
	template <class ADVector>
	void operator()(const ADVector& ax, ADVector& ay, size_t id = 0)
	{	CPPAD_ASSERT_KNOWN(
			id == 0,
			"checkpoint: id is non-zero in afun(ax, ay, id)"
		);
		this->atomic_base<Base>::operator()(ax, ay, id);
	}
	/*!
 	Link from user_atomic to forward mode 

	\copydetails atomic_base::forward
 	*/
	virtual bool forward(
		size_t                   id , 
		size_t                    q ,
		size_t                    p ,
		const vector<bool>&      vx , 
		      vector<bool>&      vy , 
		const vector<Base>&      tx ,
		      vector<Base>&      ty )
	{	CPPAD_ASSERT_UNKNOWN( id == 0 );
		CPPAD_ASSERT_UNKNOWN( f_.size_var() > 0 );
		CPPAD_ASSERT_UNKNOWN( tx.size() % (p+1) == 0 );
		CPPAD_ASSERT_UNKNOWN( ty.size() % (p+1) == 0 );
		size_t n = tx.size() / (p+1);
		size_t m = ty.size() / (p+1);
		bool ok  = true;	
		size_t i, j;

		// 2DO: test both forward and reverse vy information
		if( vx.size() > 0 )
		{	//Compute Jacobian sparsity pattern.
			vector< std::set<size_t> > s(m);
			if( n <= m )
			{	vector< std::set<size_t> > r(n);
				for(j = 0; j < n; j++)
					r[j].insert(j);
				s = f_.ForSparseJac(n, r);
			}
			else
			{	vector< std::set<size_t> > r(m);
				for(i = 0; i < m; i++)
					r[i].insert(i);
				s = f_.RevSparseJac(m, r);
			}
			std::set<size_t>::const_iterator itr;
			for(i = 0; i < m; i++)
			{	vy[i] = false;
				for(itr = s[i].begin(); itr != s[i].end(); itr++)
				{	j = *itr;
					assert( j < n );
					// y[i] depends on the value of x[j]
					vy[i] |= vx[j];
				}
			}
		}
		ty = f_.Forward(p, tx);

		// no longer need the Taylor coefficients in f_
		// (have to reconstruct them every time)
		f_.capacity_taylor(0);
		return ok;
	}
	/*!
 	Link from user_atomic to reverse mode 

	\copydetails atomic_base::reverse
 	*/
	virtual bool reverse(
		size_t                    id ,
		size_t                    p  ,
		const vector<Base>&       tx ,
		const vector<Base>&       ty ,
		      vector<Base>&       px ,
		const vector<Base>&       py )
	{	CPPAD_ASSERT_UNKNOWN( id == 0 );
		CPPAD_ASSERT_UNKNOWN( f_.size_var() > 0 );
		CPPAD_ASSERT_UNKNOWN( tx.size() % (p+1) == 0 );
		CPPAD_ASSERT_UNKNOWN( ty.size() % (p+1) == 0 );
		bool ok  = true;	

		// put proper forward mode coefficients in f_
# ifdef NDEBUG
		f_.Forward(p, tx);
# else
		size_t n = tx.size() / (p+1);
		size_t m = ty.size() / (p+1);
		CPPAD_ASSERT_UNKNOWN( px.size() == n * (p+1) );
		CPPAD_ASSERT_UNKNOWN( py.size() == m * (p+1) );
		size_t i, j, k;
		//
		vector<Base> check_ty = f_.Forward(p, tx);
		for(i = 0; i < m; i++)
		{	for(k = 0; k <= p; k++)
			{	j = i * (p+1) + k;
				CPPAD_ASSERT_UNKNOWN( check_ty[j] == ty[j] );
			}
		}
# endif
		// now can run reverse mode
		px = f_.Reverse(p+1, py);

		// no longer need the Taylor coefficients in f_
		// (have to reconstruct them every time)
		f_.capacity_taylor(0);
		return ok;
	}
	/*!
 	Link from user_atomic to forward sparse Jacobian 

	\copydetails atomic_base::for_sparse_jac
 	*/
	virtual bool for_sparse_jac(
		size_t                                  id ,
		size_t                                  q  ,
		const vector< std::set<size_t> >&       r  ,
		      vector< std::set<size_t> >&       s  )
	{	CPPAD_ASSERT_UNKNOWN( id == 0 );
		bool ok = true;
		s = f_.ForSparseJac(q, r);

		// no longer need the forward mode sparsity pattern
		// (have to reconstruct them every time)
		f_.size_forward_set(0);
		
		return ok; 
	}
	virtual bool rev_sparse_jac(
		size_t                                  id ,
		size_t                                  q  ,
		      vector< std::set<size_t> >&       rt ,
		const vector< std::set<size_t> >&       st )
	{	CPPAD_ASSERT_UNKNOWN( id == 0 );
		bool ok  = true;

		// compute rt
		bool transpose = true;
		rt = f_.RevSparseJac(q, st, transpose);

		return ok; 
	}
	virtual bool rev_sparse_hes(
		size_t                                  id ,
		size_t                                  q  ,
		const vector< std::set<size_t> >&       r  ,
		const vector<bool>&                     s  ,
		      vector<bool>&                     t  ,
		const vector< std::set<size_t> >&       u  ,
		      vector< std::set<size_t> >&       v  )
	{	// 2DO: prehaps we can get rid of transposes in a manner
		// similar to how it was done for rev_sparse_jac above.
		CPPAD_ASSERT_UNKNOWN( id == 0 );
		size_t m = s.size();
		size_t n = t.size();
		bool ok  = true;
		std::set<size_t>::const_iterator itr;

		// compute sparsity pattern for T(x) = S(x) * f'(x)
		vector< std::set<size_t> > S(1);
		size_t i, j;
		S[0].clear();
		for(i = 0; i < m; i++)
			if( s[i] )
				S[0].insert(i);
		t = f_.RevSparseJac(1, s);

		// compute sparsity pattern for A(x)^T = U(x)^T * f'(x)
		vector< std::set<size_t> > ut(q), at(q);
		for(i = 0; i < m; i++)
		{	for(itr = u[i].begin(); itr != u[i].end(); itr++)
				ut[*itr].insert(i);
		}
		at = f_.RevSparseJac(q, ut);

		// compute sparsity pattern for H(x)^T = R^T * (S * F)''(x)
		vector< std::set<size_t> > rt(n), ht(q);
		for(j = 0; j < n; j++)
		{	for(itr = rt[j].begin(); itr != rt[j].end(); itr++)
				rt[ *itr ].insert(j);
		}
		f_.ForSparseJac(q, rt);
		ht = f_.RevSparseHes(q, S);

		// compute sparsity pattern for V(x) = A(x) + H(x)^T
		for(j = 0; j < n; j++)
			v[j].clear();
		for(i = 0; i < q; i++)
		{	for(itr = at[i].begin(); itr != at[i].end(); itr++)
				v[*itr].insert(i);
			for(itr = ht[i].begin(); itr != ht[i].end(); itr++)
				v[*itr].insert(i);
		}

		// no longer need the forward mode sparsity pattern
		// (have to reconstruct them every time)
		f_.size_forward_set(0);

		return ok;
	}
};

/*! \} */
CPPAD_END_NAMESPACE
# endif
