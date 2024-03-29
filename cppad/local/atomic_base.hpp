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

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\defgroup atomic_base.hpp atomic_base.hpp
\{
\file atomic_base.hpp
Base class for atomic user operations.
*/

template <class Base>
class atomic_base {
// ===================================================================
public:
	enum option_enum { bool_sparsity_enum, set_sparsity_enum};
private:
	// ------------------------------------------------------
	// constants
	//
	/// index of this object in class_object
	const size_t index_;

	// -----------------------------------------------------
	// variables
	//
	/// sparsity pattern this object is currently using
	/// (set by constructor and option member functions)
	option_enum sparsity_;

	/// temporary work space used afun, declared here to avoid memory 
	/// allocation/deallocation for each call to afun
	vector<bool>  afun_vx_[CPPAD_MAX_NUM_THREADS];
	vector<bool>  afun_vy_[CPPAD_MAX_NUM_THREADS];
	vector<Base>  afun_tx_[CPPAD_MAX_NUM_THREADS];
	vector<Base>  afun_ty_[CPPAD_MAX_NUM_THREADS];

	// -----------------------------------------------------
	// static member functions
	//
	/// List of all the object in this class
	static std::vector<atomic_base *>& class_object(void)
	{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
		static std::vector<atomic_base *> list_;
		return list_;
	}
	/// List of names for each object in this class
	static std::vector<std::string>& class_name(void)
	{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
		static std::vector<std::string> list_;
		return list_;
	}
	// =====================================================================
public:
	// -----------------------------------------------------
	// member functions not in user API
	//
	/// current sparsity setting
	option_enum sparsity(void) const
	{	return sparsity_; }

	/// Name corresponding to a base_atomic object
	const std::string& afun_name(void) const
	{	return class_name()[index_]; }
/*
$begin atomic_ctor$$
$spell
	std
	afun
	arg
	CppAD
	bool
	ctor
	const
	matrix_mul.hpp
$$

$section Atomic Function Constructor$$
$index constructor, atomic function$$
$index atomic, function constructor$$
$index function, atomic constructor$$

$head Syntax$$
$icode%atomic_user afun%(%ctor_arg_list%)
%$$
$codei%atomic_base<%Base%>(%name%)
%$$

$head atomic_user$$

$subhead ctor_arg_list$$
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
		%atomic_user%(%ctor_arg_list%) : atomic_base<%Base%>(%name%)
	%...%
	};
%$$
where $icode ...$$  
denotes the rest of the implementation of the derived class.
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
This $icode atomic_base$$ constructor argument has either of the
following prototypes
$codei%
	const char*        %name%
	const std::string& %name%
%$$
It is the name for this atomic function and is used for error reporting.
The suggested value for $icode name$$ is $icode afun$$ or $icode atomic_user$$,
i.e., the name of the corresponding atomic object or class.

$head Examples$$

$subhead Define Constructor$$
The following are links to user atomic function constructor definitions:
$cref%get_started.cpp%atomic_get_started.cpp%Constructor%$$,
$cref%reciprocal.cpp%atomic_reciprocal.cpp%Constructor%$$,
$cref%tangent.cpp%atomic_tangent.cpp%Constructor%$$,
$cref%matrix_mul.hpp%atomic_matrix_mul.hpp%Constructor%$$.

$subhead Use Constructor$$
The following are links to user atomic function constructor uses:
$cref%get_started.cpp%atomic_get_started.cpp%Use Atomic Function%Constructor%$$,
$cref%reciprocal.cpp%atomic_reciprocal.cpp%Use Atomic Function%Constructor%$$,
$cref%tangent.cpp%atomic_tangent.cpp%Use Atomic Function%Constructor%$$,
$cref%mat_mul.cpp%atomic_mat_mul.cpp%Use Atomic Function%Constructor%$$.


$end
*/
/*!
Base class for atomic_user functions.

\tparam Base
This class is used for defining an AD<Base> atomic operation y = f(x).
*/
/// make sure user does not invoke the default constructor
atomic_base(void)
{	CPPAD_ASSERT_KNOWN(false,
		"Attempt to use the atomic_base default constructor"
	);
}
/*!
Constructor

\param name
name used for error reporting
*/
atomic_base( const std::string&  name) :
index_( class_object().size() )     ,
sparsity_( set_sparsity_enum )
{	CPPAD_ASSERT_KNOWN(
		! thread_alloc::in_parallel() ,
		"atomic_base: constructor cannot be called in parallel mode."
	);
	class_object().push_back(this);
	class_name().push_back(name);
	CPPAD_ASSERT_UNKNOWN( class_object().size() == class_name().size() );
}
/// destructor informs CppAD that this atomic function with this index
/// has dropped out of scope by setting its pointer to null
virtual ~atomic_base(void)
{	CPPAD_ASSERT_UNKNOWN( class_object().size() > index_ );
	// change object pointer to null, but leave name for error reporting
	class_object()[index_] = CPPAD_NULL;
}
/// atomic_base function object corresponding to a certain index
static atomic_base* class_object(size_t index)
{	CPPAD_ASSERT_UNKNOWN( class_object().size() > index );
	return class_object()[index];
}
/// atomic_base function name corresponding to a certain index
static const std::string& class_name(size_t index)
{	CPPAD_ASSERT_UNKNOWN( class_name().size() > index );
	return class_name()[index];
}
/*
$begin atomic_option$$
$spell
	enum
	afun
	bool
	CppAD
	std
	typedef
$$

$section Set Atomic Function Options$$
$index atomic, options$$
$index options, atomic$$

$head Syntax$$
$icode%afun%.option(%option_value%)%$$

$head atomic_sparsity$$
$index atomic_sparsity$$
$index sparsity, atomic$$
You can used this option to set to type used for 
$icode afun$$ sparsity patterns.
This does not apply individual calls to $icode afun$$,
but rather all its uses between when the sparsity pattern is set and when
it is changed.
If neither the $code set_sparsity_enum$$ or 
$code bool_sparsity_enum$$ option is set,
the type for $icode atomic_sparsity$$ is one of the two choices below
(and otherwise unspecified).

$subhead bool_sparsity_enum$$
$index bool_sparsity_enum$$
If $icode option_value$$ is $codei%atomic_base<%Base%>::bool_sparsity_enum%$$, 
then the type used by $icode afun$$ for
$cref/sparsity patterns/glossary/Sparsity Pattern/$$,
(after the option is set) will be
$codei%
	typedef CppAD::vector<bool> %atomic_sparsity%
%$$
If $icode r$$ is a sparsity pattern 
for a matrix $latex R \in B^{p \times q}$$:
$icode%r%.size() == %p% * %q%$$.

$subhead set_sparsity_enum$$
$index set_sparsity_enum$$
If $icode option_value$$ is $icode%atomic_base<%Base%>::set_sparsity_enum%$$, 
then the type used by $icode afun$$ for
$cref/sparsity patterns/glossary/Sparsity Pattern/$$,
(after the option is set) will be
$codei%
	typedef CppAD::vector< std::set<size_t> > %atomic_sparsity%
%$$
If $icode r$$ is a sparsity pattern 
for a matrix $latex R \in B^{p \times q}$$:
$icode%r%.size() == %p%$$, and for $latex i = 0 , \ldots , p-1$$,
the elements of $icode%r%[%i%]%$$ are between zero and $latex q-1$$ inclusive. 

$end
*/
void option(enum option_enum option_value)
{	switch( option_value )
	{	case bool_sparsity_enum:
		case set_sparsity_enum:
		sparsity_ = option_value;
		break;

		default:
		CPPAD_ASSERT_KNOWN(
			false,
			"atoic_base::option: option_value is not valid"
		);
	}
	return;
}
/*
-----------------------------------------------------------------------------
$begin atomic_afun$$

$spell
	mul
	afun
	const
	CppAD
$$

$section Using AD Version of Atomic Function$$
$index atomic, use function$$

$head Syntax$$
$icode%afun%(%ax%, %ay%)%$$

$head Purpose$$
Given $icode ax$$,
this call computes the corresponding value of $icode ay$$. 
If $codei%AD<%Base%>%$$ operations are being recorded,
it enters the computation as an atomic operation in the recording;
see $cref/start recording/Independent/Start Recording/$$.

$head ADVector$$
The type $icode ADVector$$ must be a
$cref/simple vector class/SimpleVector/$$ with elements of type
$codei%AD<%Base%>%$$; see $cref/Base/atomic_ctor/atomic_base/Base/$$.

$head afun$$
is a $cref/atomic_user/atomic_ctor/atomic_user/$$ object 
and this $icode afun$$ function call is implemented by the
$cref/atomic_base/atomic_ctor/atomic_base/$$ class. 

$head ax$$
This argument has prototype
$codei%
	const %ADVector%& %ax%
%$$
and size must be equal to $icode n$$.
It specifies vector $latex x \in B^n$$ 
at which an $codei%AD<%Base%>%$$ version of 
$latex y = f(x)$$ is to be evaluated; see 
$cref/Base/atomic_ctor/atomic_base/Base/$$.

$head ay$$
This argument has prototype
$codei%
	%ADVector%& %ay%
%$$
and size must be equal to $icode m$$.
The input values of its elements 
are not specified (must not matter).
Upon return, it is an $codei%AD<%Base%>%$$ version of 
$latex y = f(x)$$.

$head Examples$$
The following files contain example uses of 
the AD version of atomic functions during recording:
$cref%get_started.cpp%atomic_get_started.cpp%Use Atomic Function%Recording%$$,
$cref%reciprocal.cpp%atomic_reciprocal.cpp%Use Atomic Function%Recording%$$,
$cref%tangent.cpp%atomic_tangent.cpp%Use Atomic Function%Recording%$$,
$cref%matrix_mul.cpp%atomic_mat_mul.cpp%Use Atomic Function%Recording%$$.

$end
-----------------------------------------------------------------------------
*/
/*!
Implement the user call to <tt>afun(ax, ay)</tt> and old_atomic call to
<tt>afun(ax, ay, id)</tt>.

\tparam ADVector
A simple vector class with elements of type <code>AD<Base></code>.

\param id
optional extra information vector that is just passed through by CppAD,
and used by old_atomic derived class (not other derived classes).
This is an extra parameter to the virtual callbacks for old_atomic;
see the set_id member function.

\param ax
is the argument vector for this call,
<tt>ax.size()</tt> determines the number of arguments.

\param ay
is the result vector for this call,
<tt>ay.size()</tt> determines the number of results.
*/
template <class ADVector>
void operator()(
	const ADVector&  ax     ,
	      ADVector&  ay     ,
	size_t           id = 0 )
{	size_t i, j;
	size_t n = ax.size();
	size_t m = ay.size();
# ifndef NDEBUG
	bool ok;
	std::string msg = "atomic_base: " + afun_name() + ".eval: ";
	if( (n == 0) | (m == 0) )
	{	msg += "ax.size() or ay.size() is zero";
		CPPAD_ASSERT_KNOWN(false, msg.c_str() );
	}
# endif
	size_t thread = thread_alloc::thread_num();
	vector <Base>& tx  = afun_tx_[thread];
	vector <Base>& ty  = afun_ty_[thread];
	vector <bool>& vx  = afun_vx_[thread];
	vector <bool>& vy  = afun_vy_[thread];
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
			{	msg += afun_name() + 
				": ax contains variables from different threads.";
				CPPAD_ASSERT_KNOWN(false, msg.c_str());
			}
# endif
		}
	}
	// Use zero order forward mode to compute values
	size_t q = 0, p = 0;
	set_id(id);
# ifdef NDEBUG
	forward(q, p, vx, vy, tx, ty);  
# else
	ok = forward(q, p, vx, vy, tx, ty);  
	if( ! ok )
	{	msg += afun_name() + ": ok is false for "
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
	mul.hpp
	hes
	afun
	vx
	vy
	ty
	Taylor
	const
	CppAD
	bool
$$

$section Atomic Forward Mode$$
$index atomic, forward callback$$
$index forward, atomic callback$$
$index forward, atomic virtual$$


$head Syntax$$
$icode%ok% = %afun%.forward(%q%, %p%, %vx%, %vy%, %tx%, %ty%)%$$

$head Purpose$$
This virtual function is used by $cref atomic_afun$$
to evaluate function values.
It is also used buy $cref/forward/Forward/$$
to compute function vales and derivatives.

$head Implementation$$
This virtual function must be defined by the
$cref/atomic_user/atomic_ctor/atomic_user/$$ class.
It can just return $icode%ok% == false%$$ 
(and not compute anything) for values
of $icode%p% > 0%$$ that are greater than those used by your
$cref/forward/Forward/$$ mode calculations.

$head q$$
The argument $icode q$$ has prototype
$codei%
	size_t %q%
%$$
It specifies the lowest order Taylor coefficient that we are evaluating. 
During calls to $cref atomic_afun$$, $icode%q% == 0%$$.

$head p$$
The argument $icode p$$ has prototype
$codei%
	size_t %p%
%$$
It specifies the highest order Taylor coefficient that we are evaluating. 
During calls to $cref atomic_afun$$, $icode%p% == 0%$$.

$head vx$$
The $code forward$$ argument $icode vx$$ has prototype
$codei%
	const CppAD::vector<bool>& %vx%
%$$
The case $icode%vx%.size() > 0%$$ only occurs while evaluating a call to 
$cref atomic_afun$$.
In this case,
$icode%q% == %p% == 0%$$, 
$icode%vx%.size() == %n%$$, and
for $latex j = 0 , \ldots , n-1$$,
$icode%vx%[%j%]%$$ is true if and only if
$icode%ax%[%j%]%$$ is a $cref/variable/glossary/Variable/$$ 
in the corresponding call to 
$codei%
	%afun%(%ax%, %ay%, %id%)
%$$
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
The input values of the elements of $icode vy$$ 
are not specified (must not matter).
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

$head Discussion$$
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

$head Examples$$

$subhead Define forward$$
The following files contain example atomic $code forward$$ functions:
$cref%get_started.cpp%atomic_get_started.cpp%forward%$$,
$cref%reciprocal.cpp%atomic_reciprocal.cpp%forward%$$,
$cref%tangent.cpp%atomic_tangent.cpp%forward%$$,
$cref%matrix_mul.hpp%atomic_matrix_mul.hpp%forward%$$.

$subhead Use forward$$
The following are links to user atomic function forward uses:
$cref%get_started.cpp%atomic_get_started.cpp%Use Atomic Function%forward%$$,
$cref%reciprocal.cpp%atomic_reciprocal.cpp%Use Atomic Function%forward%$$,
$cref%tangent.cpp%atomic_tangent.cpp%Use Atomic Function%forward%$$,
$cref%mat_mul.cpp%atomic_mat_mul.cpp%Use Atomic Function%forward%$$.

 
$end
-----------------------------------------------------------------------------
*/
/*!
Link from atomic_base to forward mode 

\param q [in]
lowerest order for this forward mode calculation.

\param p [in]
highest order for this forward mode calculation.

\param vx [in]
if size not zero, which components of \c x are variables

\param vy [out]
if size not zero, which components of \c y are variables

\param tx [in]
Taylor coefficients corresponding to \c x for this calculation.

\param ty [out]
Taylor coefficient corresponding to \c y for this calculation

See the forward mode in user's documentation for base_atomic 
*/
virtual bool forward(
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
	mul.hpp
	afun
	ty
	px
	py
	Taylor
	const
	CppAD
$$

$section Atomic Reverse Mode$$
$index atomic, reverse callback$$
$index reverse, atomic callback$$
$index reverse, atomic virtual$$
$spell
	bool
$$

$head Syntax$$
$icode%ok% = %afun%.reverse(%p%, %tx%, %ty%, %px%, %py%)%$$

$head Purpose$$
This function is used by $cref/reverse/Reverse/$$ 
to compute derivatives.

$head Implementation$$
If you are using
$cref/reverse/Reverse/$$ mode,
this virtual function must be defined by the
$cref/atomic_user/atomic_ctor/atomic_user/$$ class.
It can just return $icode%ok% == false%$$ 
(and not compute anything) for values
of $icode p$$ that are greater than those used by your
$cref/reverse/Reverse/$$ mode calculations.

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
and $icode%py%.size() == m * (%p%+1)%$$.
For $latex i = 0 , \ldots , m-1$$, $latex k = 0 , \ldots , p$$,
$latex \[
	py[ i * (p + 1 ) + k ] = \partial G / \partial y_i^k
\] $$

$subhead px$$
The $icode px$$ has prototype
$codei%
	CppAD::vector<%Base%>& %px%
%$$
and $icode%px%.size() == n * (%p%+1)%$$.
The input values of the elements of $icode px$$ 
are not specified (must not matter).
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

$head Examples$$

$subhead Define reverse$$
The following files contain example atomic $code reverse$$ functions:
$cref%reciprocal.cpp%atomic_reciprocal.cpp%reverse%$$,
$cref%tangent.cpp%atomic_tangent.cpp%reverse%$$,
$cref%matrix_mul.hpp%atomic_matrix_mul.hpp%reverse%$$.

$subhead Use reverse$$
The following are links to user atomic function constructor uses:
$cref%reciprocal.cpp%atomic_reciprocal.cpp%Use Atomic Function%reverse%$$,
$cref%tangent.cpp%atomic_tangent.cpp%Use Atomic Function%reverse%$$,
$cref%mat_mul.cpp%atomic_mat_mul.cpp%Use Atomic Function%reverse%$$.

$end
-----------------------------------------------------------------------------
*/
/*!
Link from reverse mode sweep to users routine.

\param p [in]
highest order for this reverse mode calculation.

\param tx [in]
Taylor coefficients corresponding to \c x for this calculation.

\param ty [in]
Taylor coefficient corresponding to \c y for this calculation

\param px [out]
Partials w.r.t. the \c x Taylor coefficients.

\param py [in]
Partials w.r.t. the \c y Taylor coefficients.

See atomic_reverse mode use documentation 
*/
virtual bool reverse(
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
	mul.hpp
	afun
	Jacobian
	jac
	const
	CppAD
	std
	bool
	std
$$

$section Atomic Forward Jacobian Sparsity Patterns$$
$index atomic, for_sparse_jac callback$$
$index for_sparse_jac, atomic callback$$
$index for_sparse_jac, atomic virtual$$

$head Syntax$$
$icode%ok% = %afun%.for_sparse_jac(%q%, %r%, %s%)%$$

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
If you are using $cref ForSparseJac$$,
this virtual function must be defined by the
$cref/atomic_user/atomic_ctor/atomic_user/$$ class.

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
and is a $cref/atomic_sparsity/atomic_option/atomic_sparsity/$$ pattern for 
$latex R \in B^{n \times q}$$.

$subhead s$$
This argument has prototype
$codei%
	%atomic_sparsity%& %s%
%$$
The input values of its elements 
are not specified (must not matter).
Upon return, $icode s$$ is a 
$cref/atomic_sparsity/atomic_option/atomic_sparsity/$$ pattern for 
$latex S(x) \in B^{m \times q}$$.

$head ok$$
The return value $icode ok$$ has prototype
$codei%
	bool %ok%
%$$
If it is $code true$$, the corresponding evaluation succeeded,
otherwise it failed.

$head Examples$$

$subhead Define for_sparse_jac$$
The following files contain example atomic $code for_sparse_jac$$ functions:
$cref%reciprocal.cpp%atomic_reciprocal.cpp%for_sparse_jac%$$,
$cref%tangent.cpp%atomic_tangent.cpp%for_sparse_jac%$$,
$cref%matrix_mul.hpp%atomic_matrix_mul.hpp%for_sparse_jac%$$.

$subhead Use for_sparse_jac$$
The following are links to user atomic function constructor uses:
$cref%reciprocal.cpp%
	atomic_reciprocal.cpp%Use Atomic Function%for_sparse_jac%$$,
$cref%tangent.cpp%atomic_tangent.cpp%Use Atomic Function%for_sparse_jac%$$,
$cref%mat_mul.cpp%atomic_mat_mul.cpp%Use Atomic Function%for_sparse_jac%$$.

$end
-----------------------------------------------------------------------------
*/
/*!
Link from forward Jacobian sparsity sweep to atomic_base

\param q
is the column dimension for the Jacobian sparsity partterns.

\param r
is the Jacobian sparsity pattern for the argument vector x

\param s
is the Jacobian sparsity pattern for the result vector y
*/
virtual bool for_sparse_jac(
	size_t                                  q  ,
	const vector< std::set<size_t> >&       r  ,
	      vector< std::set<size_t> >&       s  )
{	return false; }
virtual bool for_sparse_jac(
	size_t                                  q  ,
	const vector<bool>&                     r  ,
	      vector<bool>&                     s  )
{	return false; }
/*
-------------------------------------- ---------------------------------------
$begin atomic_rev_sparse_jac$$
$spell
	mul.hpp
	rt
	afun
	Jacobian
	jac
	CppAD
	std
	bool
	const
	hes
$$

$section Atomic Reverse Jacobian Sparsity Patterns$$
$index atomic, rev_sparse_jac callback$$
$index rev_sparse_jac, atomic callback$$
$index rev_sparse_jac, atomic virtual$$

$head Syntax$$
$icode%ok% = %afun%.rev_sparse_jac(%q%, %rt%, %st%)%$$

$head Purpose$$
This function is used by $cref RevSparseJac$$ to compute
Jacobian sparsity patterns.
For a fixed matrix $latex R \in B^{q \times m}$$,
the Jacobian of $latex R * f( x )$$ with respect to $latex x \in B^q$$ is
$latex \[
	S(x) = R * f^{(1)} (x)
\] $$
Given a $cref/sparsity pattern/glossary/Sparsity Pattern/$$ for $latex R$$,
$code rev_sparse_jac$$ computes a sparsity pattern for $latex S(x)$$.

$head Implementation$$
If you are using $cref RevSparseJac$$,
this virtual function must be defined by the
$cref/atomic_user/atomic_ctor/atomic_user/$$ class.

$subhead q$$
The argument $icode q$$ has prototype
$codei%
     size_t %q%
%$$
It specifies the number of rows in 
$latex R \in B^{q \times m}$$ and the Jacobian 
$latex S(x) \in B^{q \times n}$$. 

$subhead rt$$
This argument has prototype
$codei%
     const %atomic_sparsity%& %rt%
%$$
and is a 
$cref/atomic_sparsity/atomic_option/atomic_sparsity/$$ pattern for
$latex R^\R{T} \in B^{m \times q}$$.

$subhead st$$
This argument has prototype
$codei%
	%atomic_sparsity%& %st%
%$$
The input value of its elements 
are not specified (must not matter).
Upon return, $icode s$$ is a 
$cref/atomic_sparsity/atomic_option/atomic_sparsity/$$ pattern for
$latex S(x)^\R{T} \in B^{n \times q}$$. 

$head ok$$
The return value $icode ok$$ has prototype
$codei%
	bool %ok%
%$$
If it is $code true$$, the corresponding evaluation succeeded,
otherwise it failed.

$head Examples$$

$subhead Define rev_sparse_jac$$
The following files contain example atomic $code rev_sparse_jac$$ functions:
$cref%reciprocal.cpp%atomic_reciprocal.cpp%rev_sparse_jac%$$,
$cref%tangent.cpp%atomic_tangent.cpp%rev_sparse_jac%$$,
$cref%matrix_mul.hpp%atomic_matrix_mul.hpp%rev_sparse_jac%$$.

$subhead Use rev_sparse_jac$$
The following are links to user atomic function constructor uses:
$cref%reciprocal.cpp%
	atomic_reciprocal.cpp%Use Atomic Function%rev_sparse_jac%$$,
$cref%tangent.cpp%atomic_tangent.cpp%Use Atomic Function%rev_sparse_jac%$$,
$cref%mat_mul.cpp%atomic_mat_mul.cpp%Use Atomic Function%rev_sparse_jac%$$.

$end
-----------------------------------------------------------------------------
*/
/*!
Link from reverse Jacobian sparsity sweep to atomic_base

\param q [in]
is the row dimension for the Jacobian sparsity partterns

\param rt [out]
is the tansposed Jacobian sparsity pattern w.r.t to range variables y

\param st [in]
is the tansposed Jacobian sparsity pattern for the argument variables x
*/
virtual bool rev_sparse_jac(
	size_t                                  q  ,
	const vector< std::set<size_t> >&       rt ,
	      vector< std::set<size_t> >&       st )
{	return false; }
virtual bool rev_sparse_jac(
	size_t                                  q  ,
	const vector<bool>&                     rt ,
	      vector<bool>&                     st )
{	return false; }
/*
-------------------------------------- ---------------------------------------
$begin atomic_rev_sparse_hes$$
$spell
	mul.hpp
	vx
	afun
	Jacobian
	jac
	CppAD
	std
	bool
	hes
	const
$$

$section Atomic Reverse Hessian Sparsity Patterns$$
$index atomic, rev_sparse_hes callback$$
$index rev_sparse_hes, atomic callback$$
$index rev_sparse_hes, atomic virtual$$

$head Syntax$$
$icode%ok% = %afun%.rev_sparse_hes(%vx%, %s%, %t%, %q%, %r%, %u%, %v%)%$$

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
If you are using and $cref RevSparseHes$$,
this virtual function must be defined by the
$cref/atomic_user/atomic_ctor/atomic_user/$$ class.

$subhead vx$$
The argument $icode vx$$ has prototype
$codei%
     const CppAD:vector<bool>& %vx%
%$$
$icode%vx%.size() == %n%$$, and
for $latex j = 0 , \ldots , n-1$$,
$icode%vx%[%j%]%$$ is true if and only if
$icode%ax%[%j%]%$$ is a $cref/variable/glossary/Variable/$$ 
in the corresponding call to 
$codei%
	%afun%(%ax%, %ay%, %id%)
%$$

$subhead s$$
The argument $icode s$$ has prototype
$codei%
     const CppAD:vector<bool>& %s%
%$$
and its size is $icode m$$. 
It is a sparsity pattern for 
$latex S(x) = g^{(1)} (y) \in B^{1 \times m}$$.

$subhead t$$
This argument has prototype
$codei%
     CppAD:vector<bool>& %t%
%$$
and its size is $icode m$$.
The input values of its elements 
are not specified (must not matter).
Upon return, $icode t$$ is a 
sparsity pattern for 
$latex T(x) \in B^{1 \times n}$$ where
$latex \[
	T(x) = (g \circ f)^{(1)} (x) = S(x) * f^{(1)} (x)
\]$$

$subhead q$$
The argument $icode q$$ has prototype
$codei%
     size_t %q%
%$$
It specifies the number of columns in 
$latex R \in B^{n \times q}$$,
$latex U(x) \in B^{m \times q}$$, and
$latex V(x) \in B^{n \times q}$$. 

$subhead r$$
This argument has prototype
$codei%
     const %atomic_sparsity%& %r%
%$$
and is a $cref/atomic_sparsity/atomic_option/atomic_sparsity/$$ pattern for 
$latex R \in B^{n \times q}$$.

$head u$$
This argument has prototype
$codei%
     const %atomic_sparsity%& %u%
%$$
and is a $cref/atomic_sparsity/atomic_option/atomic_sparsity/$$ pattern for 
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
The input value of its elements 
are not specified (must not matter).
Upon return, $icode v$$ is a 
$cref/atomic_sparsity/atomic_option/atomic_sparsity/$$ pattern for 
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
\sum_{i=1}^m g_i^{(1)} (y) \; f_i^{(2)} (x) R
\\
& = &
f^{(1)} (x)^\R{T} U(x)
+
\sum_{i=1}^m S_i (x) \; f_i^{(2)} (x) R
\end{array}
\] $$

$head Examples$$

$subhead Define rev_sparse_hes$$
The following files contain example atomic $code rev_sparse_hes$$ functions:
$cref%reciprocal.cpp%atomic_reciprocal.cpp%rev_sparse_hes%$$,
$cref%tangent.cpp%atomic_tangent.cpp%rev_sparse_hes%$$,
$cref%matrix_mul.hpp%atomic_matrix_mul.hpp%rev_sparse_hes%$$.

$subhead Use rev_sparse_hes$$
The following are links to user atomic function constructor uses:
$cref%reciprocal.cpp%
	atomic_reciprocal.cpp%Use Atomic Function%rev_sparse_hes%$$,
$cref%tangent.cpp%atomic_tangent.cpp%Use Atomic Function%rev_sparse_hes%$$,
$cref%mat_mul.cpp%atomic_mat_mul.cpp%Use Atomic Function%rev_sparse_hes%$$.

$end
-----------------------------------------------------------------------------
*/
/*!
Link from reverse Hessian sparsity sweep to base_atomic

\param vx [in]
which componens of x are variables.

\param s [in]
is the reverse Jacobian sparsity pattern w.r.t the result vector y.

\param t [out]
is the reverse Jacobian sparsity pattern w.r.t the argument vector x.

\param q [in]
is the column dimension for the sparsity partterns.

\param r [in]
is the forward Jacobian sparsity pattern w.r.t the argument vector x

\param u [in]
is the Hessian sparsity pattern w.r.t the result vector y.

\param v [out]
is the Hessian sparsity pattern w.r.t the argument vector x.
*/
virtual bool rev_sparse_hes(
	const vector<bool>&                     vx ,
	const vector<bool>&                     s  ,
	      vector<bool>&                     t  ,
	size_t                                  q  ,
	const vector< std::set<size_t> >&       r  ,
	const vector< std::set<size_t> >&       u  ,
	      vector< std::set<size_t> >&       v  )
{	return false; }
virtual bool rev_sparse_hes(
	const vector<bool>&                     vx ,
	const vector<bool>&                     s  ,
	      vector<bool>&                     t  ,
	size_t                                  q  ,
	const vector<bool>&                     r  ,
	const vector<bool>&                     u  ,
	      vector<bool>&                     v  )
{	return false; }
/*
------------------------------------------------------------------------------
$begin atomic_base_clear$$
$spell
	alloc
$$

$section Free Static Variables$$
$index free, atomic static$$
$index atomic, free static$$
$index static, free atomic$$
$index clear, atomic static$$

$head Syntax$$
$codei%atomic_base<%Base%>::clear()%$$

$head Purpose$$
Each $code atomic_base$$ objects holds onto work space in order to
avoid repeated memory allocation calls and thereby increase speed
(until it is deleted).
If an the $code atomic_base$$ object is global or static because,
the it does not get deleted.
This is a problem when using 
$code thread_alloc$$ $cref/free_all/ta_free_all/$$ 
to check that all allocated memory has been freed. 
Calling this $code clear$$ function will free all the
memory currently being held onto by the 
$codei%atomic_base<%Base%>%$$ class.

$head Future Use$$
If there is future use of an $code atomic_base$$ object,
after a call to $code clear$$,
the work space will be reallocated and held onto.

$head Restriction$$
This routine cannot be called
while in $cref/parallel/ta_in_parallel/$$ execution mode.

$end
------------------------------------------------------------------------------
*/
/*!
Free all thread_alloc static memory held by atomic_base (avoids reallocations).
(This does not include class_object() which is an std::vector.)
*/
/// Free vector memory used by this class (work space)
static void clear(void)
{	CPPAD_ASSERT_KNOWN(
		! thread_alloc::in_parallel() ,
		"cannot use atomic_base clear during parallel execution"
	);
	size_t i = class_object().size();
	while(i--)
	{	size_t thread = CPPAD_MAX_NUM_THREADS;
		while(thread--)
		{
			atomic_base* op = class_object()[i];
			if( op != CPPAD_NULL )
			{	op->afun_vx_[thread].clear();
				op->afun_vy_[thread].clear();
				op->afun_tx_[thread].clear();
				op->afun_ty_[thread].clear();
			}
		}
	}
	return;
}
// -------------------------------------------------------------------------
/*!
Set value of id (used by deprecated old_atomic class)

This function is called just before calling any of the virtual funcitons
and has the corresponding id of the corresponding virtual call.
*/
virtual void set_id(size_t id) 
{ }
// ---------------------------------------------------------------------------
};
/*! \} */
} // END_CPPAD_NAMESPACE
# endif
