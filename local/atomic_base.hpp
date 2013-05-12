/* $Id$ */
# ifndef CPPAD_ATOMIC_BASE_INCLUDED
# define CPPAD_ATOMIC_BASE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <set>
# include <cppad/local/cppad_assert.hpp>
// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/thread_alloc.hpp>

CPPAD_BEGIN_NAMESPACE
/*!
\defgroup atomic_base.hpp atomic_base.hpp
\{
\file atomic_base.hpp
Base class for atomic user operations.
*/

/*
$begin atomic_ctor$$
$spell
	afun
	arg
	CppAD
	bool
	std
	ctor
	typedef
	const
$$

$section Atomic Function Constructor$$

$head Syntax$$
$icode%atomic_user afun%(%user_ctor_arg_list%)
%$$
$codei%atomic_base<%Base%>(%name%, %use_set%)
%$$

$head atomic_user$$

$subhead user_ctor_arg_list$$
Is a list of arguments for the $icode atomic_user$$ constructor.

$subhead afun$$
The object $icode afun$$ must stay in scope for as long
as the corresponding atomic function is used.
This includes use by any $cref/ADFun<Base>/ADFun/$$ that 
has this $icode atomic_user$$ operation in its 
$cref/operation sequence/glossary/Operation/Sequence/$$.

$subhead Implementation$$
The user defined $icode atomic_user$$ class is a publicly derived class of 
$codei%atomic_base<%Base%>%$$.
It should be declared as follows:
$codei%
	class %atomic_user% : public CppAD::atomic_base<%Base%> {
	public:
		%atomic_user%(%user_ctor_arg_list%) 
		: atomic_base<%Base%>(%name%, %use_set%)
	%...%
	};
%$$
where $icode ...$$  denotes the rest of the implementation of the derived class.
This includes completing the constructor and
all the virtual functions that have their 
$code atomic_base$$ implementations replaced by 
$icode atomic_user$$ implementations.

$head atomic_base$$

$subhead Restrictions$$
The $code atomic_base$$ constructor cannot be called in
$cref/parallel/ta_in_parallel/$$ mode.

$subhead Base$$
The template parameter determines the
$icode Base$$ type for this $codei%AD<%Base%>%$$ atomic operation.

$subhead name$$
This $icode atomic_base$$ constructor argument has prototype
$codei%
	const char* %name%
%$$
It is the name for this atomic function and is used for error reporting.
The suggested value $icode name$$ is $icode afun$$, i.e.,
the name of the corresponding $icode atomic_user$$ object.

$subhead use_set$$
This $icode atomic_base$$ constructor argument has prototype
$codei%
	bool %use_set%
%$$ 
specifies if $code std::set$$ ($code true$$)
or vector of $code bool$$  ($code false$$)
are used the sparsity patterns
calculated by this atomic operation.

$head atomic_sparsity$$
If $icode use_set$$ is true, we define the type $icode atomic_sparsity$$ 
and a $cref/sparsity patterns/glossary/Sparsity Pattern/$$ 
$icode r$$ for $latex R \in B^{p \times q}$$ by
$codei%
	typedef CppAD::vector< std::set<size_t> %atomic_sparsity%
	%atomic_sparsity% %r%
%$$
where $icode%r%.size() == %p%$$ and
for $latex i = 0 , \ldots , p-1$$,
all the elements of $icode%r%[%i%]%$$ are between zero and $latex q-1$$
inclusive. 
$pre

$$
If $icode use_set$$ is false, we define the type $icode atomic_sparsity$$ 
and a $cref/sparsity patterns/glossary/Sparsity Pattern/$$ 
$icode r$$ for $latex R \in B^{p \times q}$$ by
$codei%
	typedef CppAD::vector<bool> %atomic_sparsity%
	%atomic_sparsity% %r%
%$$
where $icode%r%.size() == %p%*%q%$$.

$end
*/
/*!
Base class for atomic_user functions.

\tparam Base
This class is used for defining an AD<Base> atomic operation y = f(x).
*/
template <class Base>
class atomic_base {
private:
	// -----------------------------------------------------
	// constants
	/// name for this atomic funciton (used for error reporting)
	const std::string name_;
	/// if true, use sets for sparsity patterns, otherwise use bools.
	const bool   use_set_;
	/// index of this object in the list of all objects (see list() below)
	const size_t index_;
	// -----------------------------------------------------
	/// temporary work space used eval, declared here to avoid memory 
	// allocation/deallocation for each call to eval
	vector<bool>  eval_vx_[CPPAD_MAX_NUM_THREADS];
	vector<bool>  eval_vy_[CPPAD_MAX_NUM_THREADS];
	vector<Base>  eval_tx_[CPPAD_MAX_NUM_THREADS];
	vector<Base>  eval_ty_[CPPAD_MAX_NUM_THREADS];

	// -----------------------------------------------------
	/// List of all the object in this class
	/// (null pointer used for objects that have been deleted)
	static std::vector<atomic_base *>& list(void)
	{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
		static std::vector<atomic_base *> list_;
		return list_;
	}

	/// Name corresponding to a base_atomic object
	static const char* name(size_t index)
	{	return list()[index]->name_.c_str(); }

public:
	/// make sure user does not invoke the default constructor
	atomic_base(void)
	{	CPPAD_ASSERT_KNOWN(false,
			"Attempt to use the atomic_base default constructor"
		);
	}
	/*!
	Constructor

	\param name_in
	name used for error reporting

	\param use_set
	should sets (or bools) be used for sparsity patterns
	*/
	atomic_base(const char* name_in, bool use_set) :
	name_(name_in),
	use_set_(use_set),
	index_( list().size() )
	{	CPPAD_ASSERT_KNOWN(
			! thread_alloc::in_parallel() ,
			"atomic_base: constructor cannot be called in parallel mode."
		);
		list().push_back(this);
	}
	/// destructor informs CppAD that this atomic function with this index
	/// has dropped out of scope by setting its pointer to null
	~atomic_base(void)
	{	CPPAD_ASSERT_UNKNOWN( list().size() > index_ );
		list()[index_] = CPPAD_NULL;
	}
	/// atomic_base function object corresponding to a certain index
	static atomic_base* list(size_t index)
	{	CPPAD_ASSERT_UNKNOWN( list().size() > index );
		return list()[index];
	}
	
/*
-----------------------------------------------------------------------------
$begin atomic_ad$$

$spell
	afun
	checkpointing
	eval
	const
	CppAD
	algo
$$

$section Atomic Function AD Calls: eval, tape, algo$$

$head Syntax$$
$icode%afun%(%ax%, %ay%, %id%)
%afun%.eval(%ax%, %ay%, %id%)
%ok% = %afun%.tape(%ax%, %ay%)
%ok% = %afun%.algo(%ax%, %ay%)%$$

$head Vector$$
The type $icode Vector$$ must be a
$cref/simple vector class/SimpleVector/$$ with elements of type
$codei%AD<%Base%>%$$; see $cref/Base/atomic_ctor/atomic_base/Base/$$.

$head ax$$
This argument has prototype
$codei%
	const %Vector%& %ax%
%$$
and size must be equal to $icode n$$.
It specifies vector $latex x \in B^n$$ 
at which an $codei%AD<%Base%>%$$ version of 
$latex y = f(x)$$ is to be evaluated; see 
$cref/Base/atomic_ctor/atomic_base/Base/$$.

$head ay$$
This argument has prototype
$codei%
	%Vector%& %ay%
%$$
and size must be equal to $icode m$$.
The input values of its elements do not matter.
Upon return, it is an $codei%AD<%Base%>%$$ version of 
$latex y = f(x)$$.

$head id$$
The $icode id$$ argument is optional and can only be used for
$cref/user defined derivatives/atomic_base/User Defined Derivatives/$$.
It is intended to pass extra information about this particular use of
$icode afun$$.
If the argument $icode id$$ is not present, the default value
zero is used.
This argument should not be present 
(or must have the value zero) if $icode afun$$ is used for
$cref/checkpointing/atomic_base/Checkpointing/$$.
In this case, extra information must be passed as part of the
$icode ax$$ vector.

$head eval$$
This function is defined by the
$cref/atomic_base/atomic_ctor/atomic_base/$$ class.
Given $icode ax$$ it computes the corresponding value of $icode ay$$. 
If $codei%AD<%Base%>%$$ operations are being recorded,
it enters the computation as an atomic operation in the recording;
see $cref/start recording/Independent/Start Recording/$$.
$codei%
	%afun%(%ax%, %ay%, %id%)
%$$
is an alternative syntax for the $code eval$$ function.

$head tape$$
This function is used for 
$cref/checkpointing/atomic_base/Checkpointing/$$ and
is defined by the $cref/atomic_base/atomic_ctor/atomic_base/$$ class.
A call to $code tape$$ instructs CppAD to 
$list number$$
$cref/start recording/Independent/Start Recording/$$
$codei%AD<%Base%>%$$ operations
$lnext
Use $icode%afun%.algo(%ax%, %ay%)%$$ compute the value of $icode ay$$
$lnext
$cref/stop the recording/Independent/Stop Recording/$$
$lend
The resulting $cref ADFun$$ object is used 
by $code atomic_base$$ to compute the necessary
functions values and derivatives corresponding to future calls to 
$code eval$$.

$head algo$$
If you are using $cref/checkpointing/atomic_base/Checkpointing/$$,
this virtual function must be defined by the 
$cref/atomic_user/atomic_ctor/atomic_user/$$ class.
It is used by the $code tape$$ function.
Given $icode ax$$ it computes the corresponding value of $icode ay$$
using $codei%AD<%Base%>%$$ operations.

$end
-----------------------------------------------------------------------------
*/
template <class Vector>
void eval(
	const Vector&  ax     ,
	      Vector&  ay     ,
	size_t         id = 0 )
{	size_t i, j;
	size_t n = ax.size();
	size_t m = ay.size();
# ifndef NDEBUG
	bool ok;
	std::string msg = "atomic_base: " + name_ + ".eval: ";
	if( (n == 0) | (m == 0) )
	{	msg += "ax.size() or ay.size() is zero";
		CPPAD_ASSERT_KNOWN(false, msg.c_str() );
	}
# endif
	size_t thread = thread_alloc::thread_num();
	vector <Base>& tx  = eval_tx_[thread];
	vector <Base>& ty  = eval_ty_[thread];
	vector <bool>& vx  = eval_vx_[thread];
	vector <bool>& vy  = eval_vy_[thread];
	//
	if( vx.size() != n )
	{	vx.resize(n);
		tx.resize(n);
	}
	if( vy.size() != m )
	{	vy.resize(m);
		ty.resize(m);
	}
	// 
	// Determine tape corresponding to variables in ax
	tape_id_t     tape_id  = 0;
	ADTape<Base>* tape     = CPPAD_NULL;
	for(j = 0; j < n; j++)
	{	tx[j]  = ax[j].value_;
		vx[j]  = Variable( ax[j] );
		if( vx[j] )
		{
			if( tape_id == 0 )
			{	tape    = ax[j].tape_this();
				tape_id = ax[j].tape_id_;
				CPPAD_ASSERT_UNKNOWN( tape != CPPAD_NULL );
			}
# ifndef NDEBUG
			if( tape_id != ax[j].tape_id_ )
			{	msg += name_ + 
				": ax contains variables from different threads.";
				CPPAD_ASSERT_KNOWN(false, msg.c_str());
			}
# endif
		}
	}
	// Use zero order forward mode to compute values
	size_t q = 0, p = 0;
# ifdef NDEBUG
	forward(id, q, p, vx, vy, tx, ty);  
# else
	ok = forward(id, q, p, vx, vy, tx, ty);  
	if( ! ok )
	{	msg += name_ + ": ok is false for "
			"zero order forward mode calculation.";
		CPPAD_ASSERT_KNOWN(false, msg.c_str());
	}
# endif
	bool record_operation = false;
	for(i = 0; i < m; i++)
	{
		// pass back values
		ay[i].value_ = ty[i];

		// initialize entire vector parameters (not in tape)
		ay[i].tape_id_ = 0;
		ay[i].taddr_   = 0;

		// we need to record this operation if
		// any of the eleemnts of ay are variables,
		record_operation |= vy[i];
	}
# ifndef NDEBUG
	if( record_operation & (tape == CPPAD_NULL) )
	{	msg += 
		"all elements of vx are false but vy contains a true element";
		CPPAD_ASSERT_KNOWN(false, msg.c_str() );
	}
# endif
	// if tape is not null, ay is on the tape
	if( record_operation )
	{
		// Operator that marks beginning of this atomic operation
		CPPAD_ASSERT_UNKNOWN( NumRes(UserOp) == 0 );
		CPPAD_ASSERT_UNKNOWN( NumArg(UserOp) == 4 );
		tape->Rec_.PutArg(index_, id, n, m);
		tape->Rec_.PutOp(UserOp);

		// Now put n operators, one for each element of arugment vector
		CPPAD_ASSERT_UNKNOWN( NumRes(UsravOp) == 0 );
		CPPAD_ASSERT_UNKNOWN( NumRes(UsrapOp) == 0 );
		CPPAD_ASSERT_UNKNOWN( NumArg(UsravOp) == 1 );
		CPPAD_ASSERT_UNKNOWN( NumArg(UsrapOp) == 1 );
		for(j = 0; j < n; j++)
		{	if( vx[j] )
			{	// information for an argument that is a variable
				tape->Rec_.PutArg(ax[j].taddr_);
				tape->Rec_.PutOp(UsravOp);
			}
			else
			{	// information for an arugment that is parameter
				addr_t par = tape->Rec_.PutPar(ax[j].value_);
				tape->Rec_.PutArg(par);
				tape->Rec_.PutOp(UsrapOp);
			}
		}

		// Now put m operators, one for each element of result vector
		CPPAD_ASSERT_UNKNOWN( NumArg(UsrrpOp) == 1 );
		CPPAD_ASSERT_UNKNOWN( NumRes(UsrrpOp) == 0 );
		CPPAD_ASSERT_UNKNOWN( NumArg(UsrrvOp) == 0 );
		CPPAD_ASSERT_UNKNOWN( NumRes(UsrrvOp) == 1 );
		for(i = 0; i < m; i++)
		{	if( vy[i] )
			{	ay[i].taddr_    = tape->Rec_.PutOp(UsrrvOp);
				ay[i].tape_id_  = tape_id;
			}
			else
			{	addr_t par = tape->Rec_.PutPar(ay[i].value_);
				tape->Rec_.PutArg(par);
				tape->Rec_.PutOp(UsrrpOp);
			}
		}

		// Put a duplicate UserOp at end of UserOp sequence
		tape->Rec_.PutArg(index_, id, n, m);
		tape->Rec_.PutOp(UserOp);
	} 
	return;
}
/*
-----------------------------------------------------------------------------
$begin atomic_forward$$
$spell
	afun
	checkpointing
	vx
	vy
	ty
	eval
	Taylor
	const
	CppAD
	bool
$$

$section Atomic Forward Mode$$

$head Syntax$$
$icode%ok% = %afun%.forward(%id%, %q%, %p%, %vx%, %vy%, %tx%, %ty%)%$$

$head Purpose$$
This virtual function is used by $cref/eval/atomic_ad/eval/$$
to evaluate function values.
It is also used buy $cref/forward/Forward/$$
to compute function vales and derivatives.

$head Implementation$$
If you are using
$cref/user defined derivatives/atomic_base/User Defined Derivatives/$$, 
this virtual function must be defined by the
$cref/atomic_user/atomic_ctor/atomic_user/$$ class.
It can just return $icode%ok% == false%$$ 
(and not compute anything) for values
of $icode%p% > 0%$$ that are greater than those used by your
$cref/forward/Forward/$$ mode calculations.

$head id$$
The argument $icode id$$ has prototype
$codei%
	size_t %id%
%$$
and is the value of
$cref/id/atomic_ad/id/$$ in the corresponding call to 
$cref/id/atomic_ad/eval/$$.

$head q$$
The argument $icode q$$ has prototype
$codei%
	size_t %q%
%$$
It specifies the lowest order Taylor coefficient that we are evaluating. 
During calls to $cref/eval/atomic_ad/eval/$$, $icode%q% == 0%$$.

$head p$$
The argument $icode p$$ has prototype
$codei%
	size_t %p%
%$$
It specifies the highest order Taylor coefficient that we are evaluating. 
During calls to $cref/eval/atomic_ad/eval/$$, $icode%p% == 0%$$.

$head vx$$
The $code forward$$ argument $icode vx$$ has prototype
$codei%
	const CppAD::vector<bool>& %vx%
%$$
The case $icode%vx%.size() > 0%$$ only occurs while evaluating a call to 
$cref/eval/atomic_ad/eval/$$.
In this case,
$icode%q% == %p% == 0%$$, 
$icode%vx%.size() == %n%$$, and
for $latex j = 0 , \ldots , n-1$$,
$icode%vx%[%j%]%$$ is true if and only if
$icode%ax%[%j%]%$$ is a $cref/variable/glossary/Variable/$$ 
in the corresponding call to 
$codei%
	%afun%.eval(%ax%, %ay%)
%$$
$pre

$$
If $icode%vx%.size() == 0%$$, 
then $icode%vy%.size() == 0%$$ and neither of these vectors
should be used.

$head vy$$
The $code forward$$ argument $icode vy$$ has prototype
$codei%
	CppAD::vector<bool>& %vy%
%$$
If $icode%vy%.size() == 0%$$, it should not be used.
Otherwise, 
$icode%p% == 0%$$ and $icode%vy%.size() == %m%$$.
The input values of the elements of $icode vy$$ do not matter.
Upon return, for $latex j = 0 , \ldots , m-1$$,
$icode%vy%[%i%]%$$ is true if and only if
$icode%ay%[%i%]%$$ is a variable
(CppAD uses $icode vy$$ to reduce the necessary computations).

$head tx$$
The argument $icode tx$$ has prototype
$codei%
	const CppAD::vector<%Base%>& %tx%
%$$
and $icode%tx%.size() == (%p%+1)*%n%$$.
For $latex j = 0 , \ldots , n-1$$ and $latex k = 0 , \ldots , p$$,
we use the Taylor coefficient notation
$latex \[
\begin{array}{rcl}
	x_j^k    & = & tx [ j * ( p + 1 ) + k ]
	\\
	X_j (t)  & = & x_j^0 + x_j^1 t^1 + \cdots + x_j^p t^p
\end{array}
\] $$
Note that superscripts represent an index for $latex x_j^k$$
and an exponent for $latex t^k$$.
Also note that the Taylor coefficients for $latex X(t)$$ correspond
to the derivatives of $latex X(t)$$ at $latex t = 0$$ in the following way:
$latex \[
	x_j^k = \frac{1}{ k ! } X_j^{(k)} (0)
\] $$

$head ty$$
The argument $icode ty$$ has prototype
$codei%
	CppAD::vector<%Base%>& %ty%
%$$
and $icode%tx%.size() == (%p%+1)*%m%$$.
Upon return,
For $latex i = 0 , \ldots , m-1$$ and $latex k = 0 , \ldots , p$$,
$latex \[
\begin{array}{rcl}
	Y_i (t)  & = & f_i [ X(t) ]
	\\
	Y_i (t)  & = & y_i^0 + y_i^1 t^1 + \cdots + y_i^p t^p + o ( t^p )
	\\
	ty [ i * ( p + 1 ) + k ] & = & y_i^k
\end{array}
\] $$
where $latex o( t^p ) / t^p \rightarrow 0$$ as $latex t \rightarrow 0$$.
Note that superscripts represent an index for $latex y_j^k$$
and an exponent for $latex t^k$$.
Also note that the Taylor coefficients for $latex Y(t)$$ correspond
to the derivatives of $latex Y(t)$$ at $latex t = 0$$ in the following way:
$latex \[
	y_j^k = \frac{1}{ k ! } Y_j^{(k)} (0)
\] $$
If $latex q > 0$$, 
for $latex i = 0 , \ldots , m-1$$ and $latex k = 0 , \ldots , q-1$$,
the input of $icode ty$$ satisfies
$latex \[
	ty [ i * ( p + 1 ) + k ] = y_i^k
\]$$
and hence the corresponding elements need not be recalculated.

$head ok$$
If the required results are calculated, $icode ok$$ should be true.
Otherwise, it should be false.

$head Example$$
For example, suppose that $icode%p% == 2%$$,
and you know how to compute the function $latex f(x)$$,
its first derivative $latex f^{(1)} (x)$$,
and it component wise Hessian $latex f_i^{(2)} (x)$$.
Then you can compute $icode ty$$ using the following formulas:
$latex \[
\begin{array}{rcl}
y_i^0 & = & Y(0) 
        = f_i ( x^0 )
\\
y_i^1 & = & Y^{(1)} ( 0 ) 
        = f_i^{(1)} ( x^0 ) X^{(1)} ( 0 ) 
        = f_i^{(1)} ( x^0 ) x^1 
\\
y_i^2 
& = & \frac{1}{2 !} Y^{(2)} (0)
\\
& = & \frac{1}{2} X^{(1)} (0)^\R{T} f_i^{(2)} ( x^0 ) X^{(1)} ( 0 )
  +   \frac{1}{2} f_i^{(1)} ( x^0 ) X^{(2)} ( 0 )
\\
& = & \frac{1}{2} (x^1)^\R{T} f_i^{(2)} ( x^0 ) x^1 
  +    f_i^{(1)} ( x^0 ) x^2
\end{array}
\] $$
For $latex i = 0 , \ldots , m-1$$, and $latex k = 0 , 1 , 2$$, 
$latex \[
	ty [ i * (p + 1) + k ] = y_i^k
\] $$
 
$end
-----------------------------------------------------------------------------
*/
virtual bool forward(
	size_t                    id ,
	size_t                    q  ,
	size_t                    p  ,
	const vector<bool>&       vx ,
	      vector<bool>&       vy ,
	const vector<Base>&       tx ,
	      vector<Base>&       ty )
{	return false; }
/*
-----------------------------------------------------------------------------
$begin atomic_reverse$$
$spell
	afun
	checkpointing
	ty
	px
	py
	Taylor
	const
	CppAD
$$

$section Atomic Reverse Mode$$
$spell
	bool
$$

$head Syntax$$
$icode%ok% = %afun%.reverse(%id%, %p%, %tx%, %ty%, %px%, %py%)%$$

$head Purpose$$
This function is used by $cref/reverse/Reverse/$$ 
to compute derivatives.

$head Implementation$$
If you are using
$cref/user defined derivatives/atomic_base/User Defined Derivatives/$$, 
and $cref/reverse/Reverse/$$ mode,
this virtual function must be defined by the
$cref/atomic_user/atomic_ctor/atomic_user/$$ class.
It can just return $icode%ok% == false%$$ 
(and not compute anything) for values
of $icode%p% > 0%$$ that are greater than those used by your
$cref/reverse/Reverse/$$ mode calculations.

$head id$$
The argument $icode id$$ has prototype
$codei%
	size_t %id%
%$$
and is the value of
$cref/id/atomic_ad/id/$$ in the corresponding call to 
$cref/id/atomic_ad/eval/$$.

$head p$$
The argument $icode p$$ has prototype
$codei%
	size_t %p%
%$$
It specifies the highest order Taylor coefficient that
computing the derivative of.

$head tx$$
The argument $icode tx$$ has prototype
$codei%
	const CppAD::vector<%Base%>& %tx%
%$$
and $icode%tx%.size() == (%p%+1)*%n%$$.
For $latex j = 0 , \ldots , n-1$$ and $latex k = 0 , \ldots , p$$,
we use the Taylor coefficient notation
$latex \[
\begin{array}{rcl}
	x_j^k    & = & tx [ j * ( p + 1 ) + k ]
	\\
	X_j (t)  & = & x_j^0 + x_j^1 t^1 + \cdots + x_j^p t^p
\end{array}
\] $$
Note that superscripts represent an index for $latex x_j^k$$
and an exponent for $latex t^k$$.
Also note that the Taylor coefficients for $latex X(t)$$ correspond
to the derivatives of $latex X(t)$$ at $latex t = 0$$ in the following way:
$latex \[
	x_j^k = \frac{1}{ k ! } X_j^{(k)} (0)
\] $$

$head ty$$
The argument $icode ty$$ has prototype
$codei%
	const CppAD::vector<%Base%>& %ty%
%$$
and $icode%tx%.size() == (%p%+1)*%m%$$.
For $latex i = 0 , \ldots , m-1$$ and $latex k = 0 , \ldots , p$$,
we use the Taylor coefficient notation
$latex \[
\begin{array}{rcl}
	Y_i (t)  & = & f_i [ X(t) ]
	\\
	Y_i (t)  & = & y_i^0 + y_i^1 t^1 + \cdots + y_i^p t^p + o ( t^p )
	\\
	y_i^k    & = & ty [ i * ( p + 1 ) + k ]
\end{array}
\] $$
where $latex o( t^p ) / t^p \rightarrow 0$$ as $latex t \rightarrow 0$$.
Note that superscripts represent an index for $latex y_j^k$$
and an exponent for $latex t^k$$.
Also note that the Taylor coefficients for $latex Y(t)$$ correspond
to the derivatives of $latex Y(t)$$ at $latex t = 0$$ in the following way:
$latex \[
	y_j^k = \frac{1}{ k ! } Y_j^{(k)} (0)
\] $$


$head F, G, H$$
We use the notation $latex \{ x_j^k \} \in B^{n \times (p+1)}$$ for 
$latex \[
	\{ x_j^k \W{:} j = 0 , \ldots , n-1, k = 0 , \ldots , p \}
\]$$ 
We use the notation $latex \{ y_i^k \} \in B^{m \times (p+1)}$$ for 
$latex \[
	\{ y_i^k \W{:} i = 0 , \ldots , m-1, k = 0 , \ldots , p \}
\]$$ 
We define the function
$latex F : B^{n \times (p+1)} \rightarrow B^{m \times (p+1)}$$ by
$latex \[
	y_i^k = F_i^k [ \{ x_j^k \} ]
\] $$
We use $latex G : B^{m \times (p+1)} \rightarrow B$$
to denote an arbitrary scalar valued function of $latex \{ y_i^k \}$$.
We use $latex H : B^{n \times (p+1)} \rightarrow B$$
defined by
$latex \[
	H ( \{ x_j^k \} ) = G[ F( \{ x_j^k \} ) ]
\] $$

$head py$$
The argument $icode py$$ has prototype
$codei%
	const CppAD::vector<%Base%>& %py%
%$$
and $icode%py%.size() == (%p%+1) * %m%$$.
For $latex i = 0 , \ldots , m-1$$, $latex k = 0 , \ldots , p$$,
$latex \[
	py[ i * (p + 1 ) + k ] = \partial G / \partial y_i^k
\] $$

$subhead px$$
The $icode px$$ has prototype
$codei%
	CppAD::vector<%Base%>& %px%
%$$
and $icode%px%.size() == (%p%+1) * %n%$$.
The input values of the elements of $icode px$$ do not matter.
Upon return,
for $latex j = 0 , \ldots , n-1$$ and $latex \ell = 0 , \ldots , p$$,
$latex \[
\begin{array}{rcl}
px [ j * (p + 1) + \ell ] & = & \partial H / \partial x_j^\ell
\\
& = & 
( \partial G / \partial \{ y_i^k \} ) 
	( \partial \{ y_i^k \} / \partial x_j^\ell )
\\
& = & 
\sum_{i=0}^{m-1} \sum_{k=0}^p
( \partial G / \partial y_i^k ) ( \partial y_i^k / \partial x_j^\ell )
\\
& = & 
\sum_{i=0}^{m-1} \sum_{k=\ell}^p
py[ i * (p + 1 ) + k ] ( \partial F_i^k / \partial x_j^\ell )
\end{array}
\] $$
Note that we have used the fact that for $latex k < \ell$$,
$latex \partial F_i^k / \partial x_j^\ell = 0$$.

$head ok$$
The return value $icode ok$$ has prototype
$codei%
	bool %ok%
%$$
If it is $code true$$, the corresponding evaluation succeeded,
otherwise it failed.

$end
-----------------------------------------------------------------------------
*/
virtual bool reverse(
	size_t                    id ,
	size_t                    p  ,
	const vector<Base>&       tx ,
	const vector<Base>&       ty ,
	      vector<Base>&       px ,
	const vector<Base>&       py )
{	return false; }
/*
-------------------------------------- ---------------------------------------
$begin atomic_for_sparse_jac$$
$spell
	afun
	checkpointing
	Jacobian
	jac
	const
	CppAD
	std
	bool
	std
$$

$section Atomic Forward Jacobian Sparsity Patterns$$

$head Syntax$$
$icode%ok% = %afun%.for_sparse_jac(%id%, %q%, %r%, %s%)%$$

$head Purpose$$
This function is used by $cref ForSparseJac$$ to compute
Jacobian sparsity patterns.
For a fixed matrix $latex R \in B^{n \times q}$$,
the Jacobian of $latex f( x + R * u)$$ with respect to $latex u \in B^q$$ is
$latex \[
	S(x) = f^{(1)} (x) * R
\] $$
Given a $cref/sparsity pattern/glossary/Sparsity Pattern/$$ for $latex R$$,
$code for_sparse_jac$$ computes a sparsity pattern for $latex S(x)$$.

$head Implementation$$
If you are using
$cref/user defined derivatives/atomic_base/User Defined Derivatives/$$, 
and $cref ForSparseJac$$,
this virtual function must be defined by the
$cref/atomic_user/atomic_ctor/atomic_user/$$ class.

$head id$$
The argument $icode id$$ has prototype
$codei%
	size_t %id%
%$$
and is the value of
$cref/id/atomic_ad/id/$$ in the corresponding call to 
$cref/id/atomic_ad/eval/$$.

$subhead q$$
The argument $icode q$$ has prototype
$codei%
     size_t %q%
%$$
It specifies the number of columns in 
$latex R \in B^{n \times q}$$ and the Jacobian 
$latex S(x) \in B^{m \times q}$$. 

$subhead r$$
This argument has prototype
$codei%
     const %atomic_sparsity%& %r%
%$$
and is a $cref/atomic_sparsity/atomic_ctor/atomic_sparsity/$$ pattern for 
$latex R \in B^{n \times q}$$.

$subhead s$$
This argument has prototype
$codei%
	%atomic_sparsity%& %s%
%$$
The input values of its elements do not matter.
Upon return, $icode s$$ is a 
$cref/atomic_sparsity/atomic_ctor/atomic_sparsity/$$ pattern for 
$latex S(x) \in B^{m \times q}$$.

$head ok$$
The return value $icode ok$$ has prototype
$codei%
	bool %ok%
%$$
If it is $code true$$, the corresponding evaluation succeeded,
otherwise it failed.

$end
-----------------------------------------------------------------------------
*/
virtual bool for_sparse_jac(
	size_t                                  id ,
	size_t                                  q  ,
	const vector< std::set<size_t> >&       r  ,
	      vector< std::set<size_t> >&       s  )
{	return false; }
/*
-------------------------------------- ---------------------------------------
$begin atomic_rev_sparse_jac$$
$spell
	afun
	checkpointing
	Jacobian
	jac
	CppAD
	std
	bool
	const
	hes
$$

$section Atomic Reverse Jacobian Sparsity Patterns$$

$head Syntax$$
$icode%ok% = %afun%.rev_sparse_jac(%id%, %q%, %r%, %s%)%$$

$head Purpose$$
This function is used by $cref RevSparseJac$$ to compute
Jacobian sparsity patterns.
For a fixed matrix $latex S \in B^{q \times m}$$,
the Jacobian of $latex S * f( x )$$ with respect to $latex x \in B^q$$ is
$latex \[
	R(x) = S * f^{(1)} (x)
\] $$
Given a $cref/sparsity pattern/glossary/Sparsity Pattern/$$ for $latex S$$,
$code rev_sparse_jac$$ computes a sparsity pattern for $latex R(x)$$.

$head Implementation$$
If you are using
$cref/user defined derivatives/atomic_base/User Defined Derivatives/$$, 
and $cref RevSparseJac$$,
this virtual function must be defined by the
$cref/atomic_user/atomic_ctor/atomic_user/$$ class.

$head id$$
The argument $icode id$$ has prototype
$codei%
	size_t %id%
%$$
and is the value of
$cref/id/atomic_ad/id/$$ in the corresponding call to 
$cref/id/atomic_ad/eval/$$.

$subhead q$$
The argument $icode q$$ has prototype
$codei%
     size_t %q%
%$$
It specifies the number of rows in 
$latex S \in B^{q \times m}$$ and the Jacobian 
$latex R(x) \in B^{q \times n}$$. 


$subhead r$$
This argument has prototype
$codei%
	%atomic_sparsity%& %r%
%$$
The input value of its elements do not matter.
Upon return, $icode r$$ is a 
$cref/atomic_sparsity/atomic_ctor/atomic_sparsity/$$ pattern for
$latex R(x) \in B^{q \times n}$$. 
 
$subhead s$$
This argument has prototype
$codei%
     const %atomic_sparsity%& %s%
%$$
and is a 
$cref/atomic_sparsity/atomic_ctor/atomic_sparsity/$$ pattern for
$latex S \in B^{q \times m}$$.

$head ok$$
The return value $icode ok$$ has prototype
$codei%
	bool %ok%
%$$
If it is $code true$$, the corresponding evaluation succeeded,
otherwise it failed.

$end
-----------------------------------------------------------------------------
*/
virtual bool rev_sparse_jac(
	size_t                                  id ,
	size_t                                  q  ,
	      vector< std::set<size_t> >&       r  ,
	const vector< std::set<size_t> >&       s  )
{	return false; }
/*
-------------------------------------- ---------------------------------------
$begin atomic_rev_sparse_hes$$
$spell
	afun
	checkpointing
	Jacobian
	jac
	CppAD
	std
	bool
	hes
	const
$$

$section Atomic Reverse Hessian Sparsity Patterns$$

$head Syntax$$
$icode%ok% = %afun%.rev_sparse_hes(%id%, %q%, %r%, %s%, %t%, %u%, %v%)%$$

$head Purpose$$
This function is used by $cref RevSparseHes$$ to compute
Hessian sparsity patterns.
There is an unspecified scalar valued function 
$latex g : B^m \rightarrow B$$.
Given a $cref/sparsity pattern/glossary/Sparsity Pattern/$$ for 
$latex R \in B^{n \times q}$$,
and information about the function $latex z = g(y)$$,
this routine computes the sparsity pattern for
$latex \[
	V(x) = (g \circ f)^{(2)}( x ) R
\] $$

$head Implementation$$
If you are using
$cref/user defined derivatives/atomic_base/User Defined Derivatives/$$, 
and $cref RevSparseHes$$,
this virtual function must be defined by the
$cref/atomic_user/atomic_ctor/atomic_user/$$ class.

$head id$$
The argument $icode id$$ has prototype
$codei%
	size_t %id%
%$$
and is the value of
$cref/id/atomic_ad/id/$$ in the corresponding call to 
$cref/id/atomic_ad/eval/$$.

$subhead q$$
The argument $icode q$$ has prototype
$codei%
     size_t %q%
%$$
It specifies the number of columns in 
$latex R \in B^{n \times q}$$ and 
$latex V(x) \in B^{n \times q}$$. 

$subhead r$$
This argument has prototype
$codei%
     const %atomic_sparsity%& %r%
%$$
and is a $cref/atomic_sparsity/atomic_ctor/atomic_sparsity/$$ pattern for 
$latex R \in B^{n \times q}$$.

$subhead s$$
This argument has prototype
$codei%
     const %atomic_sparsity%& %s%
%$$
and is a $cref/atomic_sparsity/atomic_ctor/atomic_sparsity/$$ pattern for 
$latex S(x) = g^{(1)} (y) \in B^{1 \times m}$$.

$subhead t$$
This argument has prototype
$codei%
     %atomic_sparsity%& %t%
%$$
The input values of its elements do not matter.
Upon return, $icode t$$ is a 
$cref/atomic_sparsity/atomic_ctor/atomic_sparsity/$$ pattern for 
$latex \[
	T(x) = (g \circ f)^{(1)} (x) = S(x) * f^{(1)} (x) \in B^{1 \times n}
\]$$

$head u$$
This argument has prototype
$codei%
     const %atomic_sparsity%& %u%
%$$
and is a $cref/atomic_sparsity/atomic_ctor/atomic_sparsity/$$ pattern for 
$latex U(x) \in B^{m \times q}$$ which is defined by
$latex \[
\begin{array}{rcl}
U(x) 
& = & 
\partial_u \{ \partial_y g[ y + f^{(1)} (x) R u ] \}_{u=0}
\\
& = & 
\partial_u \{ g^{(1)} [ y + f^{(1)} (x) R u ] \}_{u=0}
\\
& = &
g^{(2)} (y) f^{(1)} (x) R
\end{array}
\] $$

$subhead v$$
This argument has prototype
$codei%
     %atomic_sparsity%& %v%
%$$
The input value of its elements do not matter.
Upon return, $icode v$$ is a 
$cref/atomic_sparsity/atomic_ctor/atomic_sparsity/$$ pattern for 
$latex V(x) \in B^{n \times q}$$ which is defined by
$latex \[
\begin{array}{rcl}
V(x) 
& = & 
\partial_u [ \partial_x (g \circ f) ( x + R u )  ]_{u=0}
\\
& = &
\partial_u [ (g \circ f)^{(1)}( x + R u )  ]_{u=0}
\\
& = &
(g \circ f)^{(2)}( x ) R
\\
& = &
f^{(1)} (x)^\R{T} g^{(2)} ( y ) f^{(1)} (x)  R
+
\sum_{i=1}^m [ g^{(1)} (y) ]_i \; f_i^{(2)} (x) R
\\
& = &
f^{(1)} (x)^\R{T} U(x)
+
\sum_{i=1}^m S(x)_i \; f_i^{(2)} (x) R
\end{array}
\] $$

$end
-----------------------------------------------------------------------------
*/
virtual bool rev_sparse_hes(
	size_t                                  id ,
	size_t                                  q  ,
	const vector< std::set<size_t> >&       r  ,
	const vector< std::set<size_t> >&       s  ,
	      vector< std::set<size_t> >&       t  ,
	const vector< std::set<size_t> >&       u  ,
	      vector< std::set<size_t> >&       v  )
{	return false; }
};

/*! \} */
CPPAD_END_NAMESPACE
# endif
