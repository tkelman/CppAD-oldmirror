/* $Id$ */
# ifndef CPPAD_AD_CTOR_INCLUDED
# define CPPAD_AD_CTOR_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
------------------------------------------------------------------------------

$begin ad_ctor$$
$spell
	initializes
	Vec
	const
$$

$index AD, constructor$$
$index constructor, AD$$
$index convert, to AD$$
$index Base, convert to AD$$
$index VecAD, convert to AD$$

$section AD Constructors $$

$head Syntax$$
$codei%AD<%Base%> %y%()
%$$
$codei%AD<%Base%> %y%(%x%)
%$$
$codei%AD<%Base%> %y% = %x%$$

$head Purpose$$
creates a new $codei%AD<%Base%>%$$ object $icode y$$ 
and initializes its value as equal to $icode x$$.

$head x$$
The argument $icode x$$ has prototype
$codei%
	const %Type% &%x%
%$$ 
where $icode Type$$ is
$codei%VecAD<%Base%>::reference%$$,
$codei%AD<%Base%>%$$,
$icode Base$$, 
or any type that has a constructor of the form
$icode%Base%(%x%)%$$.

$head y$$
The target $icode y$$ has prototype
$codei%
	AD<%Base%> %y%
%$$

$head Example$$
$children%
	example/ad_ctor.cpp
%$$
The files $cref ad_ctor.cpp$$ contain examples and tests of these operations.
It test returns true if it succeeds and false otherwise.

$end
------------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
\defgroup ad_ctor_hpp ad_ctor.hpp
\{
\file ad_ctor.hpp
AD<Base> constructors and and copy operations.
*/

/*!
\page AD_default_ctor
Use default copy constructor 
because they may be optimized better than the code below:
\code
template <class Base>
inline AD<Base>::AD(const AD &x) 
{
	value_    = x.value_;
	tape_id_  = x.tape_id_;
	taddr_    = x.taddr_;

	return;
}
\endcode
*/

/*!
Default Constructor.

\tparam Base
Base type for this AD object.
*/
template <class Base>
inline AD<Base>::AD(void) 
: value_()
, tape_id_(0)
, taddr_(0)
{ }


/*!
Constructor from Base type.

\tparam Base
Base type for this AD object.

\param b
is the Base type value corresponding to this AD object.
The tape identifier will be an invalid tape identifier,
so this object is initially a parameter.
*/
template <class Base>
inline AD<Base>::AD(const Base &b) 
: value_(b)
, tape_id_(0)
, taddr_(0)
{	// check that this is a parameter
	CPPAD_ASSERT_UNKNOWN( Parameter(*this) );
}	

/*!
Constructor from an ADVec<Base> element drops the vector information.

\tparam Base
Base type for this AD object.
*/
template <class Base>
inline AD<Base>::AD(const VecAD_reference<Base> &x)
{	*this = x.ADBase(); }

/*!
Constructor from any other type, converts to Base type, and uses constructor
from Base type.

\tparam Base
Base type for this AD object.

\tparam T
is the the type that is being converted to AD<Base>.
There must be a constructor for Base from Type.

\param t
is the object that is being converted from T to AD<Base>.
*/
template <class Base>
template <class T>
inline AD<Base>::AD(const T &t) 
: value_(Base(t))
, tape_id_(0)
, taddr_(0)
{ }

/*! \} */
} // END_CPPAD_NAMESPACE
# endif
