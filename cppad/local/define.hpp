# ifndef CPPAD_DEFINE_INCLUDED
# define CPPAD_DEFINE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Define$$ $comment CppAD Developer Documentation$$
$spell
	cppad.hpp
	Microsoft
	VecAD_reference
	Cpp
	inline
	namespace
	std
	const
	Op
	OpenMp
$$
$aindex head$$

$mindex macro definition$$
$section Macros Used by CppAD Implementation$$ 

$head CPPAD_NULL$$
The preprocessor symbol $code CPPAD_NULL$$ is used for a null pointer. 
If it is not yet defined,
it is defined when $code cppad/local/define.hpp/$$ is included.

$head CPPAD_MAX_NUM_THREADS$$
The preprocessor symbol $code CPPAD_MAX_NUM_THREADS$$ 
is the maximum number of OpenMp threads that can
be used with CppAD.
If it is not yet defined,
it is defined when $code cppad/local/define.hpp/$$ is included.

$head CPPAD_FOLD_BINARY_OPERATOR$$
The syntax
$syntax%
	CPPAD_FOLD_BINARY_OPERATOR(%Type%, %Op%)
%$$
defines the operators
$syntax%
	%left% %Op% %right%
%$$
which has the result type $italic Type$$ and the 
following argument types:
$syntax%
	%left%                    %right%
	-----------------------   -----------------
	AD<%Base%>                VecAD_reference<%Base%>
	VecAD_reference<%Base%>   AD<%Base%>
	VecAD_reference<%Base%>   VecAD_reference<%Base%>
	%Base%                    AD<%Base%>
	%Base%                    VecAD_reference<%Base%>
	AD<%Base%>                %Base%
	VecAD_reference<%Base%>   %Base%
	int                       AD<%Base%>
	int                       VecAD_reference<%Base%>
	AD<%Base%>                int
	VecAD_reference<%Base%>   int
%$$
In the case where the left operand has type $syntax%AD<%Base%>%$$,
it defines a member function.
All of the arguments are $code const$$ and call by reference,
except for the $code int$$ case which is call by value.
In all cases, it converts the operands to $syntax%AD<%Base%>%$$ and then
uses the definition of the same operation for that case. 
Hence it assumes a definition
for the case where both operands are $syntax%AD<%Base%>%$$ objects.


$end
*/

# ifndef CPPAD_NULL
# define CPPAD_NULL     0
# endif

# ifndef CPPAD_MAX_NUM_THREADS
# define CPPAD_MAX_NUM_THREADS 16
# endif


# define CPPAD_FOLD_BINARY_OPERATOR(Type, Op)                          \
/* ----------------------------------------------------------------*/  \
/* Operations with VecAD_reference<Base> and AD<Base> only*/           \
template <class Base>                                                  \
                                                                       \
inline Type AD<Base>::operator Op                                      \
(const VecAD_reference<Base> &right) const                             \
{	return *this Op right.ADBase(); }                              \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
(const VecAD_reference<Base> &left, const VecAD_reference<Base> &right)\
{	return left.ADBase() Op right.ADBase(); }                      \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
	(const VecAD_reference<Base> &left, const AD<Base> &right)     \
{	return left.ADBase() Op right; }                               \
/* ----------------------------------------------------------------*/  \
/* Operations Base */                                                  \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
	(const Base &left, const AD<Base> &right)                      \
{	return AD<Base>(left) Op right; }                              \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
	(const Base &left, const VecAD_reference<Base> &right)         \
{	return AD<Base>(left) Op right.ADBase(); }                     \
                                                                       \
template <class Base>                                                  \
inline Type AD<Base>::operator Op (const Base &right)  const           \
{	return *this Op AD<Base>(right); }                             \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
	(const VecAD_reference<Base> &left, const Base &right)         \
{	return left.ADBase() Op AD<Base>(right); }                     \
                                                                       \
/* ----------------------------------------------------------------*/  \
/* Operations int */                                                   \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
	(int left, const AD<Base> &right)                              \
{	return AD<Base>(left) Op right; }                              \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
	(int left, const VecAD_reference<Base> &right)                 \
{	return AD<Base>(left) Op right.ADBase(); }                     \
                                                                       \
template <class Base>                                                  \
inline Type AD<Base>::operator Op (int right) const                    \
{	return *this Op AD<Base>(right); }                             \
                                                                       \
template <class Base>                                                  \
inline Type operator Op                                                \
	(const VecAD_reference<Base> &left, int right)                 \
{	return left.ADBase() Op AD<Base>(right); }

# endif
