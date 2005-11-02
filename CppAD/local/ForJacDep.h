# ifndef CppADForJacDepIncluded
# define CppADForJacDepIncluded

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

/*
$begin ForJacDep$$
$spell
	Jacobian
	Jac
	const
	Bool
	Dy
	Dep
	proportional
$$

$section Forward Mode Jacobian Dependency$$ 

$index forward, depend$$
$index depend, forward$$
$index bit pattern, forward$$

$table
$bold Syntax$$ $cnext
$syntax%%Dy% = %F%.Forward(q, %Dx%)%$$
$rnext $cnext
$tend

$fend 20$$

$head Jacobian Dependency Matrix$$
$index Jacobian, depend matrix$$
$index matrix, Jacobian depend$$
$index depend, Jacobian$$
Suppose that $latex G : B^q \rightarrow B^p$$ is a differentiable function.
A boolean valued $latex p \times q$$ matrix $latex D$$ is a dependency
matrix for $latex G$$ if 
for $latex i = 1, \ldots , p$$ and $latex j = 1 , \ldots q$$,
$latex \[
D_{i,j} = {\rm false} \Rightarrow G^{(1)}_{i,j} (x) = 0  
\hspace{1cm} ({\rm for \; all}) \; x \in B^p
\] $$


$head Description$$
Given the function 
$latex F : B^n \rightarrow B^m$$ defined by the argument $italic F$$,
and a Jacobian dependency matrix $latex Dx$$ for a function
$latex X : B^q \rightarrow B^n$$,
$code ForJacDep$$ returns a dependency matrix for the function
$latex F \circ X : B^q \rightarrow B^m$$. 

$head F$$
The object $italic F$$ has prototype
$syntax%
	ADFun<%Base%> %F%
%$$
It defines a function
$latex F : B^n \rightarrow B^m$$,
where $latex n$$ is the dimension of the 
$xref/ADFun/Domain/domain/$$ space for $italic F$$, and
$latex m$$ is the dimension of the 
$xref/ADFun/Range/range/$$ space for $italic F$$.

$head q$$
The argument $italic q$$ has prototype
$syntax%
	size_t %q%
%$$
It specifies the number of components we are computing the 
dependency relation with respect to.
Note that the memory required for the calculation is proportional 
to $latex q$$ times the total number of variables on the tape.
Thus it may be desireable to break the dependency calculation into 
groups that do not require to much memory. 

$head Dx$$
The argument $italic Dx$$ has prototype
$syntax%
	const %VectorBool% &%Dx%
%$$
and is a vector with size $latex q * n$$.
The dependency matrix for $latex X$$ is given by
$latex \[
	Dx_{i,j} = Dx [ i * q + j ]
\] $$
for $latex i = 1 , \ldots , n$$ and $latex j = 1 , \ldots , q$$.

$head Dy$$
The return value $italic Dy$$ has prototype
$syntax%
	const %VectorBool% &%Dy%
%$$
and is a vector with size $latex q * m$$.
The dependency matrix for $latex F \circ X$$ is given by
$latex \[
	Dy_{i,j} = Dy [ i * q + j ]
\] $$
for $latex i = 1 , \ldots , m$$ and $latex j = 1 , \ldots , q$$.

$head VectorBool$$
The type $italic VectorBool$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type bool/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head Example$$
$children%
	Example/ForJacDep.cpp
%$$
The file
$xref/ForJacDep.cpp/$$
contains an example and a test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
template <class VectorBool>
VectorBool ADFun<Base>::ForJacDep(size_t q, const VectorBool &Dx)
{
	// type used to pack bits (must support standard bit operations)
	typedef size_t Pack;

	// temporary indices
	size_t i, j, k, p;

	// check VectorBool is Simple Vector class with bpp; type elements
	CheckSimpleVector<bool, VectorBool>();

	// range and domain dimensions for F
	size_t m = depvar.size();
	size_t n = indvar.size();

	CppADUsageError(
	Dx.size() == n * q,
	"Second argument to ForDep does not have length equal to\n"
	"first argument times domain dimension for the corresponding ADFun."
	);

	
	// number of packed values per variable on the tape
	size_t npv = 1 + q / sizeof(Pack);

	// array that will hold packed values
	Pack *pack;
	try
	{	pack = new Pack[totalNumVar * npv]; 
	}
	catch(...)
	{	CppADUsageError(0, "cannot allocate sufficient memory");
		abort();
	}

	// set values corresponding to independent variables
	Pack mask;
	for(i = 0; i < n; i++)
	{	CppADUnknownError( indvar[i] < totalNumVar );
		// indvar[i] is operator taddr for i-th independent variable
		CppADUnknownError( Rec->GetOp( indvar[i] ) == InvOp );

		// initialize all bits as zero
		for(k = 0; k < npv; k++)
			pack[ indvar[i] * npv + k ] = 0;

		// set bits that are true
		for(j = 0; j < q; j++) 
		{	k    = j / sizeof(Pack);
			p    = j - k * sizeof(Pack);
			mask = Pack(1) << p;
			if( Dx[ i * q + j ] )
				pack[ indvar[i] * npv + k ] |= mask;
		}
	}

	// evaluate the dependencies
	ForJacSweep(npv, totalNumVar, Rec, pack);

	// return values corresponding to dependent variables
	VectorBool Dy(m * q);
	for(i = 0; i < m; i++)
	{	CppADUnknownError( depvar[i] < totalNumVar );

		// set bits 
		for(j = 0; j < q; j++) 
		{	k     = j / sizeof(Pack);
			p     = j - k * sizeof(Pack);
			mask  = Pack(1) << p;
			mask &=	pack[ depvar[i] * npv + k ];
			Dy[ i * q + j ] = (mask != 0);
		}
	}

	// free local memory
	delete [] pack;

	return Dy;
}

} // END CppAD namespace
	

# endif
