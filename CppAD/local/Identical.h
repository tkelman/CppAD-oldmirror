# ifndef CppADIdenticalIncluded
# define CppADIdenticalIncluded

// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
// END SHORT COPYRIGHT

/*
------------------------------------------------------------------------------
$begin IdenticalPar$$
$spell
	inline
	const
	bool
$$

$index identical, parameter$$
$index parameter, identical$$

$section Check if a Value is Identically a Parameter$$

$table
$bold Syntax$$ $cnext
$syntax%inline bool IdenticalPar(const %Type% &%x%)%$$
$tend

$head Description$$
If $italic x$$ is not of type $syntax%AD<%Base%>%$$,
this function returns true. 
Otherwise, it returns true if and only if 
the sequence of values
$syntax%
	%x%, %x%.value, %x%.value.value , ...
%$$
are all parameters.

$end 
------------------------------------------------------------------------------
$begin IdenticalZero$$
$spell
	inline
	const
	bool
$$

$index identical, zero$$
$index zero, identical$$

$section Check if a Value is Identically Zero$$

$table
$bold Syntax$$ $cnext
$syntax%inline bool IdenticalZero(const %Type% &%x%)%$$
$tend

$head Description$$
If $italic x$$ is not of type $syntax%AD<%Base%>%$$,
this function returns true if and only if
$italic x$$ is equal to zero.
Otherwise, it returns true if and only if it is equal to zero
and the sequence of values
$syntax%
	%x%, %x%.value, %x%.value.value , ...
%$$
are all parameters.

$end 
------------------------------------------------------------------------------


$begin IdenticalOne$$
$spell
	inline
	const
	bool
$$

$index identical, one$$
$index one, identical$$

$section Check if a Value is Identically One$$

$table
$bold Syntax$$ $cnext
$syntax%inline bool IdenticalOne(const %Type% &%x%)%$$
$tend

$head Description$$
If $italic x$$ is not of type $syntax%AD<%Base%>%$$,
this function returns true if and only if
$italic x$$ is equal to zero.
Otherwise, it returns true if and only if it is equal to zero
and the sequence of values
$syntax%
	%x%, %x%.value, %x%.value.value , ...
%$$
are all parameters.

$end 
------------------------------------------------------------------------------
$begin IdenticalEqual$$
$spell
	inline
	const
	bool
$$

$index identical, equal$$
$index equal, identical$$

$section Check if a Value is Identically Equal Another$$

$table
$bold Syntax$$ $cnext
$syntax%inline bool IdenticalEqual(const %Type% &%x%, const %Type%)%$$
$tend

$head Description$$
If $italic Type$$ is $syntax%AD<%Base%>%$$ for some $italic Base$$,
$italic x$$ is identically equal $italic y$$ if and only if
both $italic x$$ and $italic y$$ are parameters
and $syntax%%x%.value%$$ is identically equal $syntax%%y%.value%$$.
If $italic Type$$ is not $syntax%AD<%Base%>%$$ for some $italic Base$$,
$italic x$$ is identically equal $italic y$$ if and only if
$syntax%
	%x% == %y%
%$$

$end 
------------------------------------------------------------------------------
*/


namespace CppAD {
	// Parameter -------------------------------------------
	template <class Base>
	inline bool IdenticalPar(const AD<Base> &x)
	{	return Parameter(x) && IdenticalPar(x.value); }

	inline bool IdenticalPar(const float &x)
	{	return true; }

	inline bool IdenticalPar(const std::complex<float> &x)
	{	return true; }

	inline bool IdenticalPar(const double &x)
	{	return true; }

	inline bool IdenticalPar(const std::complex<double> &x)
	{	return true; }

	// Zero -----------------------------------------------
	template <class Base>
	inline bool IdenticalZero(const AD<Base> &x)
	{	return Parameter(x) && IdenticalZero(x.value); }

	inline bool IdenticalZero(const float &x)
	{	return (x == 0.); }

	inline bool IdenticalZero(const std::complex<float> &x)
	{	return (x == std::complex<float>(0., 0.) ); }

	inline bool IdenticalZero(const double &x)
	{	return (x == 0.); }

	inline bool IdenticalZero(const std::complex<double> &x)
	{	return (x == std::complex<double>(0., 0.) ); }

	// One -----------------------------------------------
	template <class Base>
	inline bool IdenticalOne(const AD<Base> &x)
	{	return Parameter(x) && IdenticalOne(x.value); }

	inline bool IdenticalOne(const float &x)
	{	return (x == 1.); }

	inline bool IdenticalOne(const std::complex<float> &x)
	{	return (x == std::complex<float>(1., 0.) ); }

	inline bool IdenticalOne(const double &x)
	{	return (x == 1.); }

	inline bool IdenticalOne(const std::complex<double> &x)
	{	return (x == std::complex<double>(1., 0.) ); }

	// Equal -----------------------------------------------
	template <class Base>
	inline bool IdenticalEqual(const AD<Base> &x, const AD<Base> &y)
	{	bool parameter;
		parameter = ( Parameter(x) & Parameter(y) );
		return parameter  && IdenticalEqual(x.value, y.value); 
	}

	inline bool IdenticalEqual(const float &x, const float &y)
	{	return (x == y); }
	inline bool IdenticalEqual(
		const std::complex<float> &x, const std::complex<float> &y)
	{	return (x == y); }

	inline bool IdenticalEqual(const double &x, const double &y)
	{	return (x == y); }
	inline bool IdenticalEqual(
		const std::complex<double> &x, const std::complex<double> &y)
	{	return (x == y); }
}

# endif
