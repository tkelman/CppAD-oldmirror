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
// 2DO: add and implement choice between sets and bools for sparsity patterns.
//      overide operator(), and check that id is zero there.

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

		// check for special case
		if( vx.size() > 0 )
		{	//Compute r, a Jacobian sparsity pattern.
			// 2DO: use forward mode when m < n
			vector< std::set<size_t> > s(m), r(m);
			for(i = 0; i < m; i++)
				s[i].insert(i);
			r = f_.RevSparseJac(m, s);
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
};

/*! \} */
CPPAD_END_NAMESPACE
# endif
