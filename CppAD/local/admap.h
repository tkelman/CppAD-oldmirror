# ifndef CppADadmapIncluded
# define CppADadmapIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

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
// This file contains the implementation of the admap template class.  
// The constructor is documented in omh/admap.  
// The member functions are documented in this file.

// BEGIN CppAD namespace
namespace CppAD {


template <class Base>
class admap {
private:
	// type used for packing sparsity patters
	typedef size_t Pack;

	// state of an admap object
	enum admap_state { empty_state, defined_state };

	TapeRec<Base>    *rec;
	enum admap_state  state;
	size_t            n;
	size_t            m;

public:
	// constructor 
	admap(void)
	: rec(CppADNull), state(empty_state), n(0), m(0)
	{ }

	// destructor
	~admap(void)
	{	if( rec != CppADNull )
			delete rec;
	}

	// empty
	void empty(void);

	// map
	template <typename Vector>
	void map(const Vector x, const Vector &y);

	// domain
	size_t domain(void) const;

	// range
	size_t range(void) const;

	// one forward mode sweep
	template <typename Vector>
	Vector forward(size_t p, const Vector &x) const; 

	// one reverse mode sweep
	template <typename Vector>
	Vector reverse(size_t p, const Vector &x, Vector &w) const;
};
/*
-----------------------------------------------------------------------------
$begin admap_empty$$
$spell
	cpp
	admap
$$

$section Empty the Contents of an AD Mapping$$

$index empty, admap$$
$index admap, empty$$

$table
$bold Syntax$$ $cnext
$syntax%%f%.empty()%$$
$tend

$fend 20$$

$head Purpose$$
An AD mapping in the defined state has information
stored within it. 
This function allows to user to free memory used to store that information.

$head f$$
The object $italic f$$ has prototype
$syntax%
	admap<%Base%> %f%
%$$
Any previous mapping connected to this object is lost.

$head Mapping State$$
The object $italic f$$ may have any mapping state when
$syntax%%f%.empty()%$$ is called.
After this call,
the mapping state of $italic f$$ will be empty.

$head Example$$
The file $code admap.cpp$$ contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
*/
template<typename Base>
void admap<Base>::empty(void)
{	if( rec != CppADNull )
		delete rec;
	state = empty_state;
	n     = 0;
	m     = 0;
}

/*
----------------------------------------------------------------------------
$begin admap_map$$
$spell
	const
	cpp
	admap
$$

$section Store a New AD Mapping$$

$index map$$
$index stop, tape$$
$index tape, stop$$
$index variable, dependent$$
$index variable, independent$$
$index dependent, variable$$
$index independent, variable$$

$table
$bold Syntax$$ $cnext
$syntax%%f%.map(%x%, %y%)%$$
$tend

$fend 20$$

$head Purpose$$
This stores a new AD mapping in the object $italic f$$.

$head Tape State$$
$index state, tape$$
$index tape, state$$
This operation can only be done while the tape 
that records $syntax%AD<%Base%>%$$ operations is in the 
$xref/glossary/Tape State/record state/$$.
After this operation, the tape 
that records $syntax%AD<%Base%>%$$ operations 
will be in the empty state.

$head f$$
The object $italic f$$ has prototype
$syntax%
	admap<%Base%> %f%
%$$

$head x$$
The vector $italic x$$ has prototype
$syntax%
	const %Vector% &%x%
%$$
It specifies the independent variables for this mapping.
The length of $italic x$$, must be greater than zero,
and is the number of independent variables
(dimension of the domain space for $italic f$$).
The current recording of $syntax%AD<%Base%>%$$ operations 
was started with a call to $xref/Independent/$$ with $italic x$$
as the independent variable vector.
None of the elements of $italic x$$ may have changed
between the call
$syntax%
	Independent(%x%)
%$$
and the corresponding call
$syntax%
	%f%.map(%x%, %y%)
%$$

$head y$$
The vector $italic y$$ has prototype
$syntax%
	const %Vector% &%y%
%$$
It specifies the dependent variables for this mapping.
The length of $italic x$$, must be greater than zero,
and is the number of independent variables
(dimension of the domain space for $italic f$$).
The length of $italic y$$, must be greater than zero,
and is the number of dependent variables
(dimension of the range space for $italic f$$).

$head Vector$$
The type $italic Vector$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type/$$
$syntax%AD<%Base%>%$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head Mapping State$$
The object $italic f$$ can have any mapping state when 
$syntax%%f%.map%$$ is called.  
After this call,
the mapping state of $italic f$$ will be defined.

$head Example$$
The file $code admap.cpp$$ contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
----------------------------------------------------------------------------

$begin admap_domain$$
$spell
	cpp
	admap
	const
$$

$section Determine Domain Dimension for an AD Mapping$$

$index domain$$
$index dimension, domain$$
$index variable, number independent$$
$index independent, number variables$$

$table
$bold Syntax$$ $cnext
$syntax%%n% = %f%.domain()%$$
$tend

$fend 20$$

$head Purpose$$
Determine the dimension of the domain space 
(number of independent variables)
corresponding to
$latex \[
	f : B^n \rightarrow B^m
\] $$ 
One can also use $syntax%%f%.domain%$$ to determine the 
mapping state of $italic f$$ (see the description of $italic n$$ below).

$head f$$
The object $italic f$$ has prototype
$syntax%
	const admap<%Base%> %f%
%$$

$head n$$
The return value $italic n$$ has prototype
$syntax%
	size_t %n%
%$$
If the mapping state of $italic f$$ is empty,
$italic n$$ is equal to zero.
Otherwise, it is non-zero.

$head Mapping State$$
The object $italic f$$ can have any mapping state when 
$syntax%%f%.domain%$$ is called.  
The mapping state of $italic f$$ is not affected by this call.


$head Example$$
The file $code admap.cpp$$ contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
----------------------------------------------------------------------------

$begin admap_range$$
$spell
	cpp
	admap
	const
$$

$section Determine Range Dimension for an AD Mapping$$

$index range$$
$index dimension, domain$$
$index variable, number dependent$$
$index dependent, number variables$$

$table
$bold Syntax$$ $cnext
$syntax%%m% = %f%.range()%$$
$tend

$fend 20$$

Determine the dimension of the range space 
(number of dependent variables)
corresponding to
$latex \[
	f : B^n \rightarrow B^m
\] $$ 
One can also use $syntax%%f%.range%$$ to determine the 
mapping state of $italic f$$ (see the description of $italic m$$ below).

$head f$$
The object $italic f$$ has prototype
$syntax%
	const admap<%Base%> %f%
%$$
If the mapping state of $italic f$$ is empty,
$italic n$$ is equal to zero.
Otherwise, it is non-zero.

$head m$$
The return value $italic m$$ has prototype
$syntax%
	size_t %m%
%$$
If the mapping state of $italic f$$ is empty,
$italic m$$ is equal to zero.
Otherwise, it is non-zero.

$head Mapping State$$
The object $italic f$$ can have any mapping state when 
$syntax%%f%.range%$$ is called.  
The mapping state of $italic f$$ is not affected by this call.

$head Example$$
The file $code admap.cpp$$ contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
----------------------------------------------------------------------------
$begin admap_forward$$
$spell 
	cpp
	dy
	const
	admap
$$

$section AD Mapping Forward Mode Calculations$$

$table
$bold Syntax$$
$syntax%%y% = %f%.forward(%p%, %x%)%$$
$tend

$fend 20$$

$head Purpose$$
Compute derivatives up to order $latex p-1$$ for the function
$latex \[
	F : B^n \rightarrow B^m
\] $$
corresponding to the AD mapping object $italic f$$.

$head f$$
The object $italic f$$ has prototype
$syntax%
	const admap<%Base%> %f%
%$$

$head p$$
The argument $italic p$$ has prototype
$syntax%
	size_t %p%
%$$
It must be greater than or equal one and
specifies order of the derivatives that are being calculated.

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const %Vector% &%x%
%$$
and size $latex p * n$$.
For $latex k = 0, \ldots , p-1$$,
We use $latex x^{(k)} \in B^n$$ to denote the vector defined by
$latex \[
	x_j^{(k)} = x [ j * p + k ]
\]$$
for $latex j = 0 , \ldots , n-1$$.
We use $latex X : B \rightarrow B^n$$ to denote the 
mathematical function defined by
$latex \[
	X(t) = \frac{t^0}{0 !} x^{(0)}
	     + \frac{t^1}{1 !} x^{(1)}
	     + \cdots 
	     + \frac{t^{p-1}}{(p-1) !} x^{(p-1)} 
\] $$
Note that for $latex k = 0 , \ldots , p-1$$, the $th k$$ derivative of
$latex X(t)$$ at $latex t = 0$$ is given by
$latex \[
	X^{(k)} (0) = x^{(k)}
\] $$

$head Vector$$
The type $italic Vector$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type/$$
$italic Base$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head y$$
We use $latex Y : B \rightarrow B^m$$ to denote the function defined by
$latex \[
	Y(t) = F[ X(t) ]
\] $$
The return value $italic y$$ has prototype
$syntax%
	%Vector% &%y%
%$$
and size $latex p * m$$.
For 
$latex i = 0 , \ldots , m-1$$,
and $latex k = 0 , \ldots , p-1$$, the $th k$$ derivative of
$latex Y_i (t)$$ at $latex t = 0$$ is given by
$latex \[
	Y_i^{(k)} (0) = y [ i * p + k ]
\] $$

$head Zero Order$$
Values for the function $latex F$$ are given by the relation
$latex \[
	Y(0) = F( X(0) ) = F( x^{(0)} ) 
\] $$

$head First Order$$
In the case where $latex p$$ is greater than or equal two,
values for the derivative of $latex f$$ 
times a direction vector
are given by the relation
$latex \[
	Y^{(1)} (0) 
	=
	F^{(1)} ( X(0) ) * X^{(1)} (0)
	=
	F^{(1)} ( x^{(0)} ) * x^{(1)}
\] $$
Note that if $latex x^{(1)}$$ is the $th j$$ 
$xref/glossary/Elementary Vector/elementary vector/$$,
$latex \[
	\D{F}{x_j} ( x^{(0)} ) = Y^{(1)} (0)
\] $$ 

$head Second Order$$
In the case where $latex p$$ is greater than or equal three,
for $latex i = 0 , \ldots , m - 1$$
$latex \[
\begin{array}{rcl}
Y_i^{(2)} (0)
& = & (F_i \circ X)^{(2)} ( 0 ) 
\\
& = & 
F_i^{(1)} [ X(0) ] * X^{(2)} (0) 
	+ X^{(1)} (0)^T * F_i^{(2)} [ X(0) ] * X^{(1)} (0) 
\\
& = & 
F_i^{(1)} ( x^{(0)} ) * x^{(2)}
	+
	( x^{(1)} )^T * F_i^{(2)} ( x^{(0)} ) * x^{(1)}
\end{array}
\] $$
Note that if $latex x^{(1)}$$ is the $th j$$ 
$xref/glossary/Elementary Vector/elementary vector/$$
and $latex x^{(2)}$$ is zero,
$latex \[
\begin{array}{rcl}
	\DD{F_i}{x_j}{x_j} ( x^{(0)} ) = Y_i^{(2)} (0) 
\end{array}
\] $$
If $latex x^{(1)}$$ is the sum of the $th j$$ and $th l$$ 
$xref/glossary/Elementary Vector/elementary vectors/$$
and $latex x^{(2)}$$ is zero,
$latex \[
\begin{array}{rcl}
Y_i^{(2)} (0)
& = & 
\DD{F_i}{x_j}{x_j}       ( x^{(0)} )
+
\DD{F_i}{x_j}{x_\ell}    ( x^{(0)} )
+
\DD{F_i}{x_\ell}{x_j}    ( x^{(0)} )
+
\DD{F_i}{x_\ell}{x_\ell} ( x^{(0)} )
\\
\DD{F_i}{x_\ell}{x_j}    ( x^{(0)} )
& = & 
Y_i^{(2)} (0)
-
\DD{F_i}{x_j}{x_j}      ( x^{(0)} )
-
\DD{F_i}{x_\ell}{x_\ell} ( x^{(0)} )
\end{array} 
\] $$

$head Mapping State$$
The object $italic f$$ must be in the defined mapping state when
$syntax%%f%.forward%$$ is called.  
The mapping state of $italic f$$ is 
not affected by this call.


$head Example$$
The file $code admap_forward.cpp$$ contains 
an example and test of this operation. 
It returns true if it succeeds and false otherwise.

$end
----------------------------------------------------------------------------
$begin admap_reverse$$
$spell 
	cpp
	dy
	const
	admap
$$

$section AD Mapping Reverse Mode Calculations$$

$table
$bold Syntax$$
$syntax%%y% = %f%.reverse(%p%, %x%, %w%)%$$
$tend

$fend 20$$

$head Purpose$$
Compute derivatives up to order $latex p$$ for the function
$latex \[
	F : B^n \rightarrow B^m
\] $$
corresponding to the AD mapping object $italic f$$.

$head f$$
The object $italic f$$ has prototype
$syntax%
	const admap<%Base%> %f%
%$$

$head p$$
The argument $italic p$$ has prototype
$syntax%
	size_t %p%
%$$
must be greater than or equal one and
specifies order of the derivatives that are being calculated.

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const %Vector% &%x%
%$$
and size $latex p * n$$.
For $latex k = 0, \ldots , p-1$$,
We use $latex x^{(k)} \in B^n$$ to denote the vector defined by
$latex \[
	x_j^{(k)} = x [ j * p + k ]
\]$$
for $latex j = 0 , \ldots , n-1$$.
We use $latex X : B \times B^n \rightarrow B^n$$ to denote the 
mathematical function defined by
$latex \[
	X(t, u) 
		=  u
		+ \frac{t^1}{1 !} x^{(1)}
		+ \cdots 
		+ \frac{t^{p-1}}{(p-1) !} x^{(p-1)} 
\] $$
Note that for $latex k = 0 , \ldots , p-1$$, the $th k$$ partial of
$latex X(t, u)$$ with respect to $latex t$$
at $latex t = 0$$ and $latex u = x^{(0)}$$ is given by
$latex \[
	\Dpow{k}{t} X( 0, x^{(0)} ) = x^{(k)}
\] $$

$head Vector$$
The type $italic Vector$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type/$$
$italic Base$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head w$$
The argument $italic w$$ has prototype
$syntax%
	const %Vector% &%w%
%$$
and size $latex m$$.
This argument defines the function 
$latex Y : B \times B^n \rightarrow B$$ by
$latex \[
	Y(t, u) = \sum_{i=0}^{m-1} w_i F_i [ X(t, u) ]
\] $$


$head y$$
The return value $italic y$$ has prototype
$syntax%
	%Vector% &%y%
%$$
and size $latex p * n$$.
For 
$latex j = 0 , \ldots , n-1$$,
and $latex k = 0 , \ldots , p-1$$, 
$latex \[
y[ j * p + k ]
=
\frac{\partial}{\partial u_j} \left\{ 
	\Dpow{k}{t} \left[ Y ( t, u ) \right]_{t=0} 
\right\}_{u=x^{(0)}} 
%
\hspace{2cm} (\star)
\] $$

$head First Order$$
Substituting $latex k = 0$$ and 
$latex w$$ the $th i$$ 
$xref/glossary/Elementary Vector/elementary vector/$$ in
$latex (\star)$$ we obtain
$latex \[
\begin{array}{rcl}
y[ j * p + 0 ] 
& = & 
\frac{\partial}{\partial u_j} \left[ Y ( 0 , u ) \right]_{u=x^{(0)}} 
=
\frac{\partial}{\partial u_j} \left\{ F_i [ X ( 0 , u ) ] \right\}_{u=x^{(0)}} 
=
\frac{\partial}{\partial u_j} \left[ F_i ( u ) \right]_{u=x^{(0)}} 
\\
& = &
\D{F}{x_j} F_i ( x^{(0)} ) 
\end{array}
\] $$

$head Second Order$$
Suppose  $latex p \geq 2$$,
and $latex w$$ is the $th i$$ 
$xref/glossary/Elementary Vector/elementary vector/$$
It follows from $latex (\star)$$ with $latex k = 1$$ and
$latex j = 0, \ldots, n-1$$
$latex \[
\begin{array}{rcl}
y[ j * p + 1 ] 
& = & 
\frac{\partial}{\partial u_j} \left( 
	\Dpow{1}{t} \left\{ F_i [ X ( t, u ) ] \right\}_{t=0} 
\right)_{u=x^{(0)}} 
\\
& = & 
\frac{\partial}{\partial u_j} \left[ 
	F_i^{(1)} ( u ) * x^{(1)}
\right]_{u=x^{(0)}} 
\\
& = & 
\sum_{\ell=0}^{n-1} \DD{F_i}{x_j}{x_\ell} ( x^{(0)} )  x_\ell^{(1)}
\\
& = & \left[ F^{(2)} ( x^{(0)} ) * x^{(1)} \right]_j
\end{array}
\] $$
Thus, with one call to $code reverse$$ we can compute all the components
of the Hessian of $latex F$$ times a direction vector; i.e.
$latex \[
	F^{(2)} ( x^{(0)} ) * x^{(1)}
\] $$
Furthermore, if $latex x^{(1)}$$ is chosen to be the
$th l$$ elementary vector 
$latex \[
y[ j * p + 1 ] 
=
\DD{F_i}{x_j}{x_\ell} ( x^{(0)} ) 
\] $$


$head Mapping State$$
The object $italic f$$ must be in the defined mapping state when
$syntax%%f%.reverse%$$ is called.  
The mapping state of $italic f$$ is 
not affected by this call.

$head Example$$
The file $code admap_reverse.cpp$$ contains 
an example and test of this operation. 
It returns true if it succeeds and false otherwise.


$end
------------------------------------------------------------------------------
*/

} // END CppAD namespace

# endif
