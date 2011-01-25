/* $Id$ */
# ifndef CPPAD_USER_ATOMIC_INCLUDED
# define CPPAD_USER_ATOMIC_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin user_atomic$$
$spell
	bool
	namespace
	CppAD
	const
	Taylor
	tx
	ty
	px
	py
$$

$section User Defined Atomic AD Functions$$

$index atomic, function$$
$index user, function$$
$index operation, user defined$$
$index function, user defined$$

$head Syntax$$
$codei%CPPAD_ATOMIC_FUNCTION(%Base%, %name%)
%$$
$icode%ok% = %name%(%ax%, %ay%)
%$$
$icode%ok% = %name%(%k%, %n%, %m%, %tx%, %ty%)
%$$
$icode%ok% = %name%(%k%, %n%, %m%, %tx%, %ty%, %px%, %py%)
%$$

$head Purpose$$
In some cases, the user knows how to compute the derivative
of a function $latex y = f(x)$$
more efficiently than by coding it $codei%AD<%Base%>%$$ 
$cref/atomic/glossary/Operation/Atomic/$$ operations
and letting CppAD do the calculations.
In this case, $code CPPAD_ATOMIC_FUNCTION$$ can be used
make add the user code for $latex f(x)$$ and its derivatives
to the set of $codei%AD<%Base%>%$$ atomic operations. 

$head vector$$
Here and below $code vector$$ refers to the
CppAD $cref/vector/CppAD_vector/$$ template class.

$head Base$$
This is the 
$cref/base type/base_require/$$
corresponding to the operations sequence;
i.e., we are adding $icode name$$, 
with arguments and results of type $codei%AD<%Base%>%$$,
to the list of available atomic operations.

$head name$$
This is the name of the function (as it is used in the source code).
The user must provide a version of $icode name$$
where the arguments are vectors with elements of type $icode Base$$.
CppAD uses this to create a version of $icode name$$
where the argument are vectors with elements of  type $codei%AD<%Base%>%$$.

$head ok$$
The return value $icode ok$$ has prototype
$codei%
	bool %ok%
%$$
If it is $code true$$, the corresponding evaluation succeeded,
otherwise it failed.

$head ax$$
The argument $icode ax$$ has prototype
$codei%
	const vector< AD<%Base%> >& %ax%
%$$
It is the argument value at which the $codei%AD<%Base%>%$$ version of 
$latex y = f(x)$$ is to be evaluated.
The size of this vector,
(the dimension of the domain space for $latex f$$)
may depend on the call to $icode name$$.

$head ay$$
The result $icode ay$$ has prototype
$codei%
	vector< AD<%Base%> > %ay%
%$$
It is the result of the evaluation of the $codei%AD<%Base%>%$$ version of 
$latex y = f(x)$$.
The size of this vector,
(the dimension of the range space for $latex f$$)
may depend on the call to $icode name$$.

$head k$$
The argument $icode k$$ has prototype
$codei%
	size_t %k%
%$$
The value $icode%k%$$ is the order of the Taylor coefficient that
we are either evaluating, or taking the derivative of.

$head n$$
The argument $icode n$$ has prototype
$codei%
	size_t %n%
%$$
It is the size of the vector $icode ax$$ in the corresponding call to
$icode%name%(%ax%, %ay%)%$$.

$head m$$
The argument $icode m$$ has prototype
$codei%
	size_t %m%
%$$
It is the size of the vector $icode ay$$ in the corresponding call to
$icode%name%(%ax%, %ay%)%$$.

$head tx$$
The argument $icode tx$$ has prototype
$codei%
	const vector<%Base%>& %tx%
%$$
and $icode%tx%.size() >= (%k% + 1) * %n%$$.
For $latex j = 0 , \ldots , n-1$$ and $latex \ell = 0 , \ldots , k$$,
we use the Taylor coefficient notation
$latex \[
\begin{array}{rcl}
	x_j^\ell & = & tx [ j * ( k + 1 ) + \ell ]
	\\
	X_j (t) & = & x_j + x_j^1 t^1 + \cdots + x_j^k t^k
\end{array}
\] $$
If $icode%tx%.size() > (%k% + 1) * %n%$$,
the other components of $icode tx$$ are not specified.

$head ty$$
During $cref/forward mode/user_atomic/Forward Mode/$$ 
the argument $icode ty$$ has prototype
$codei%
	vector<%Base%>& %ty%
%$$
while during $cref/reverse mode/user_atomic/Reverse Mode/$$
it has prototype
$codei%
	const vector<%Base%>& %ty%
%$$
For $latex i = 0 , \ldots , m-1$$ and $latex \ell = 0 , \ldots , k$$,
we use the Taylor coefficient notation
$latex \[
\begin{array}{rcl}
	y_i^\ell & = & ty [ i * ( k + 1 ) + \ell ]
	\\
	Y_i (t) & = & y_i + y_i^1 t^1 + \cdots + y_i^k t^k
\end{array}
\] $$
If $icode%ty%.size() > (%k% + 1) * %m%$$,
the other components of $icode ty$$ are not specified and should not be used.

$head px$$
The argument $icode px$$ has prototype
$codei%
	vector<%Base%>& %px%
%$$
and $icode%px%.size() >= (%k% + 1) * %n%$$.
For $latex j = 0 , \ldots , n-1$$ and $latex \ell = 0 , \ldots , k$$,
$latex \[
	px[ j * (k + 1 ) + \ell ]
\] $$
is a partial derivative of a scalar values function with respect to
the Taylor coefficient $latex x_j^\ell$$.
If $icode%px%.size() > (%k% + 1) * %n%$$,
the other components of $icode px$$ are not specified and should not be used.

$head py$$
The argument $icode py$$ has prototype
$codei%
	vector<%Base%>& %py%
%$$
and $icode%py%.size() = (%k% + 1) * %m%$$.
For $latex i = 0 , \ldots , m-1$$ and $latex \ell = 0 , \ldots , k$$,
$latex \[
	py[ i * (k + 1 ) + \ell ]
\] $$
is a partial derivative of a scalar values function with respect to
the Taylor coefficient $latex y_i^\ell$$.
If $icode%py%.size() > (%k% + 1) * %m%$$,
the other components of $icode py$$ are not specified and should not be used.

$head Create AD Version$$
$index CPPAD_ATOMIC_FUNCTION$$
The preprocessor macro invocation
$codei%
	CPPAD_ATOMIC_FUNCTION(%Base%, %name%)
%$$ 
defines the $codei%AD<%Base%>%$$ version of $icode name$$.
This macro can be placed within a namespace 
(not the $code CppAD$$ namespace) 
but must be outside of any routine.

$head Forward Mode$$
The user defined function
$codei%
	%name%(%k%, %n%, %m%, %tx%, %ty%)
%$$
is used to compute results during a $cref/forward/Forward/$$ mode sweep.
For this call, we are given the Taylor coefficients for $latex x$$ 
form order zero through $icode k$$,
and the Taylor coefficient for $latex y$$ with order less than $icode k$$.
The routine $icode name$$ must compute the 
$icode k$$ order Taylor coefficients for $latex y$$ using the relations
$latex \[
	Y(t) = f[ X(t) ]
\] $$
Then, for $latex i = 0 , \ldots , m-1$$, it sets
$latex \[
	ty [ i * (k + 1) + k ] = y_i^k
\] $$
The other components of $icode ty$$ must be left unchanged.


$head Reverse Mode$$
The user defined function
$codei%
	%name%(%k%, %n%, %m%, %tx%, %ty%, %px%, %py%)
%$$
is used to compute results during a $cref/reverse/Reverse/$$ mode sweep. 
The input value of the vectors $icode tx$$ and $icode ty$$
contain Taylor coefficient up to order $icode k$$.
There is an arbitrary function of these Taylor coefficients
$latex g : B^{n \times k} \times B^{m \times k} \rightarrow B$$.
The input value of the vectors $icode px$$ and $icode py$$ 
contain the partial derivatives
of $latex g$$ with w.r.t the Taylor coefficients.
To be specific
$latex \[
\begin{array}{rcl}
	\partial g / \partial x_j^\ell & = & px [ j * ( k + 1 ) + \ell ]
	\\
	\partial g / \partial y_i^\ell & = & px [ i * ( k + 1 ) + \ell ]
\end{array}
\] $$
Using the calculations from forward mode,
the Taylor coefficient for $icode y$$ are a function of the Taylor
Coefficient for $icode x$$; so we write $latex y(x)$$ 
for this function and define
$latex \[
	h(x) = g [ x , y(x) ]
\] $$
The output values in the vector $icode px$$ contain the derivative
of $latex h$$ with respect to $latex x$$.
To be specific, 
for $latex j = 0 , \ldots , n-1$$ and $latex \ell = 0 , \ldots , k$$,
$latex \[
\begin{array}{rcl}
	px [ i * (k + 1) + k ] & = & \partial h / \partial x_j^\ell
	\\
	& = & 
	\partial g / \partial x_j^\ell 
	\; + \; 
	( \partial g / \partial y^\ell ) ( \partial y / \partial x_j^\ell )
\end{array}
\] $$
The output values of the elements of $icode py$$ does not matter.

$head Efficiency$$
If any elements of $icode ax$$ are variables,
all of elements of $icode ay$$ must be treated as variables because
the user's $icode name$$ routine does not get and track this information.
In addition, no sparsity pattern information is passed to and from
the user's $icode name$$ routine. 
Hence the 
$cref/sparsity pattern/glossary/Sparsity Pattern/$$ will not be 
as efficient they should be.
Perhaps these sort of improvements
(at the expense of the user's routine being more complicated)
should be added to the CppAD $cref/wish list/WishList/$$.

$end
------------------------------------------------------------------------------
*/
CPPAD_BEGIN_NAMESPACE
/*!
\file user_atomic.hpp
user defined atomic operations.
*/

/*!
\def CPPAD_ATOMIC_FUNCTION(Base, name)
Defines the function <tt>name(ax, ay)</tt>  
where \c ax and \c ay are vectors with <tt>AD<Base></tt> elements.

\par Base
is the base type for the atomic operation.

\par name 
is the name of the user defined function that corresponding to this operation.
*/

// note that #name is a version of name with quotes arround it.
# define CPPAD_ATOMIC_FUNCTION(Base, name)                   \
inline bool name (                                           \
     const CppAD::vector< AD<Base> >& ax,                    \
     CppAD::vector< AD<Base> >&       ay                     \
)                                                            \
{    static CppAD:user_atomic<Base> fun(#name, name, name);  \
     return fun.ad(ax, ay);                                  \
}

/*!
Class that actually implements the <tt>name(ax, ay)</tt> call.

A new user_atomic object is generated for each time the user invokes
the CPPAD_ATOMIC_FUNCTION macro; see static object in that macro.
*/
template <class Base>
class user_atomic {
	/// type for the user routine that computes forward mode results
	typedef bool (*F) (
		size_t               k, 
		size_t               n,
		size_t               m,
		const vector<Base>& tx, 
		vector<Base>&       ty
	);
	/// type for the user routine that computes reverse mode results
	typedef bool (*R) (
		size_t               k, 
		size_t               n, 
		size_t               m, 
		const vector<Base>& tx, 
		const vector<Base>& ty,
		vector<Base>&       px,
		vector<Base>&       py
	);
private:
	/// name of this atomic operation
	const std::string     name_;
	/// user's implementation of forward mode
	const F                  f_;
	/// user's implementation of reverse mode
	const R                  r_;
	/// index of this object in the vector of all objects in this class
	const size_t         index_;

	/// work space vector used for a Base copy of ax
	vector<Base>             x_;
	/// work space vector used for a Base copy of ay
	vector<Base>             y_;

	/// the list of all object in this class
	static vector<user_atomic *> *List(void)
	{	static vector<user_atomic *> list;
		return &list;
	}
public:
	/*!
 	Constructor called for each invocation of CPPAD_ATOMIC_FUNCTION.

	Put this object in the list of all objects for this calls and set
	the constant private data name_, f_, r_, and index_.

	\param Name
	is the user's Name for this atomic operation.

	\param f
	user routine that does forward mode calculations for this operation.

	\param r
	user routine that does reverse mode calculations for this operation.
	*/
	user_atomic(const char* Name, F f, R r) : 
	name_(Name)
	, f_(f)
	, r_(r)
	, index_( List()->size() )
	{	List()->push_back(this); }

	/*!
 	Implement the user call <tt>name(ax, ay)</tt>.

	\param ax
	is the argument vector for this call,
	<tt>ax.size()</tt> determines the number of arguments.

	\param ay
	is the result vector for this call,
	<tt>ay.size()</tt> determines the number of results.

	This routine is not \c const because it may modify the works
	space vectors \c x_ and \c y_.
 	*/
	void ad(const AD<Base>& ax, AD<Base>& ay)
	{	size_t i, j;
		size_t n = ax.size();
		size_t m = ay.size();
		//
		if( x_.size() < n )
			x_.resize(n);
		if( y_.size() < m )
			y_.resize(m);
		//
		// Determine if we are going to have to tape this operation
		size_t tape_id    = 0;
		ADTape<Base> tape = CPPAD_NULL;
		for(j = 0; j < n; j++)
		{	x_[j]   = ax[j].value_;
			if ( tape == CPPAD_NULL & Variable(ax[j]) )
			{	tape    = ax[j].tape_this();
				tape_id = ax[j].id_;
			}
# ifndef NDEBUG
			if( (tape_id != 0) & Variable(ax[j]) & (tape_id != ax[j]._id) )
			{	std::string msg = name_ + 
				": ax contains variables from different OpenMP threads.";
				CPPAD_ASSERT_KNOWN(false, msg.c_str());
			}
# endif
		}
		// Use zero order forward mode to compute values
		bool ok = f_(0, x_, y_);  
		if( ! ok )
		{	std::string msg = name_ +
			": ok returned false from zero order forward mode calculation";
			CPPAD_ASSERT_KNOWN(false, msg.c_str());
		}
		// pass back values
		for(i = 0; i < m; i++)
			ay[i].value_ = y_[i];
		//
		if( tape != CPPAD_NULL )
		{	Base parameter;
			// Note the actual number of results is m
			CPPAD_ASSERT_UNKNOWN( NumRes(UserOp) == 0 );
			CPPAD_ASSERT_UNKNOWN( NumArg(UserOp) == 3 );

			// Start of operators corresponding to a user atomic operation.
			// Put function index, domain size, and range size in tape
			tape->Rec_.PutArg(index_, n, m);
			tape->Rec_.PutOp(UserOp);
			// n + m operators follow for this one atomic operation

			// Now put the information for the argument vector in the tape
			CPPAD_ASSERT_UNKNOWN( NumRes(UservOp) == 0 );
			CPPAD_ASSERT_UNKNOWN( NumRes(UserpOp) == 0 );
			CPPAD_ASSERT_UNKNOWN( NumArg(UservOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumArg(UserpOp) == 1 );
			for(j = 0; j < n; j++)
			{	if( Variable(ax[j]) )
				{	// information for an argument that is a variable
					tape->Rec_.PutArg(ax[j].taddr_);
					tape->Rec_.PutOp(UservOp);
				}
				else
				{	// information for an arugment that is parameter
					parameter = tape->Rec_.PutPar(ax[j].value_);
					tape->Rec_.PutArg(parameter);
					tape->Rec_.PutOp(UserpOp);
				}
			}

			// Now put the information for the results in the tape
			CPPAD_ASSERT_UNKNOWN( NumRes(UserrOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumArg(UserrOp) == 0 );
			for(i = 0; i < m; i++)
			{	ay[i].taddr = tape->Rec_.PutOp(UserrOp);
				ay[i].id_   = tape_id;
			}
		} 
		return;
	}

	/// Name corresponding to a user_atomic object
	static const char* name(size_t index)
	{	return *(List[index])->name_.c_str(); }
	/*!
 	Link from forward mode sweep to users routine.

	\param index
	index in the list of all user_atomic objects
	corresponding to this function.

	\param k
	order for this forward mode calculation.

	\param n
	domain space size for this calcualtion.

	\param m
	range space size for this calculation.

	\param tx
	Taylor coefficients corresponding to \c x for this calculation.

	\param ty
	Taylor coefficient corresponding to \c y for this calculation

	See the forward mode documentation for user_atomic 
 	*/
	static void forward(
		size_t              index, 
		size_t                  k,
		size_t                  n, 
		size_t                  m, 
		const vector<Base>&    tx,
		vector<Base>&          ty
	)
	{
		CPPAD_ASSERT_UNKNOWN(index < List()->size() );
		user_atomic* op = *( List()[index] );

		bool ok = op->f_(k, n, m, tx, ty);
		if( ! ok )
		{	std::stringstream ss;
			ss << k;
			std::string msg = op->name_ + ": ok returned false from ";
				ss.str() + " order forward mode calculation";
			CPPAD_ASSERT_KNOWN(false, msg.c_str());
		}
	}


	/*!
 	Link from reverse mode sweep to users routine.

	\param index
	index in the list of all user_atomic objects
	corresponding to this function.

	\param k
	order for this forward mode calculation.

	\param n
	domain space size for this calcualtion.

	\param m
	range space size for this calculation.

	\param tx
	Taylor coefficients corresponding to \c x for this calculation.

	\param ty
	Taylor coefficient corresponding to \c y for this calculation

	\param px
	Partials w.r.t. the \c x Taylor coefficients.

	\param py
	Partials w.r.t. the \c y Taylor coefficients.

	See reverse mode documentation for user_atomic 
 	*/
	static void reverse(
		size_t              index, 
		size_t                  k,
		size_t                  n, 
		size_t                  m, 
		const vector<Base>&    tx,
		const vector<Base>&    ty,
		vector<Base>&          px,
		vector<Base>&          py
	)
	{
		CPPAD_ASSERT_UNKNOWN(index < List()->size() );
		user_atomic* op = *( List()[index] );

		bool ok = op->f_(k, n, m, tx, ty, px, py);
		if( ! ok )
		{	std::stringstream ss;
			ss << k;
			std::string msg = op->name_ + ": ok returned false from ";
				ss.str() + " order reverse mode calculation";
			CPPAD_ASSERT_KNOWN(false, msg.c_str());
		}
	}
};

CPPAD_END_NAMESPACE
# endif
