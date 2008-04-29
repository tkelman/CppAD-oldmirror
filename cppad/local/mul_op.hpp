# ifndef CPPAD_MUL_OP_INCLUDED
# define CPPAD_MUL_OP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ForMulOp$$ $comment CppAD Developer Documentation$$
$spell
	Mul
	Mulpv
	Mulvp
	Mulvv
	Taylor
	const
	inline
	Op
$$

$index multiply, forward operator$$
$index forward, multiply operator$$
$index operator, multiply forward$$
$index ForMul$$


$section Forward Mode Multiplication Operator$$

$head Syntax$$

$codei%inline void ForMulvvOp(size_t %d%,
	%Base% *%z%, const %Base% *%x%, const %Base% *%y%)%$$
$pre
$$
$codei%inline void ForMulpvOp(size_t %d%,
	%Base% *%z%, const %Base% *%p%, const %Base% *%v%)%$$
$pre
$$
$codei%inline void ForMulvpOp(size_t %d%,
	%Base% *%z%, const %Base% *%x%, const %Base% *%p%)%$$


$head Description$$
Computes the $icode d$$ order Taylor coefficient for $latex Z$$ where
$table
Operation  $cnext Value  $rnext
Mulvv       $cnext $latex Z = X * Y$$ $rnext
Mulpv       $cnext $latex Z = P * Y$$ $rnext
Mulvp       $cnext $latex Z = X * P$$ 
$tend

$head x$$
The vector $icode x$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $icode X$$.

$head y$$
The vector $icode y$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $icode Y$$.
The vector $icode y$$ is not present in the MulL and MulR cases.

$head p$$
The scalar $codei%*%p%$$ contains the value of the parameter $icode P$$.

$head z$$
The vector $icode z$$ has length $latex d+1$$.
On input it contains the
$th d-1$$ order Taylor coefficient row vector for $icode Z$$.
On output it contains the
$th d$$ order Taylor coefficient row vector for $icode Z$$; i.e.,
$icode%z%[%d%]%$$ is set equal to the $th d$$ Taylor coefficient for
the function $icode Z$$.

$end
------------------------------------------------------------------------------
$begin RevMulvvOp$$ $comment CppAD Developer Documentation$$
$spell
	Mul
	Mulpv
	Mulvp
	Mulvv
	Taylor
	const
	inline
	Op
	px
	py
	pz
$$


$index multiply, reverse operator$$
$index reverse, multiply operator$$
$index operator, multiply reverse$$
$index RevMul$$

$section Reverse Mode Multiplication Operator$$

$head Syntax$$

$codei%inline void RevMulvvOp(size_t %d%,
	const %Base% *%z%, const %Base% *%x%, const %Base% *%y%,
	const %Base% *%pz%, %Base% *%px%, %Base% *%py%)%$$
$pre
$$
$codei%inline void RevMulpvOp(size_t %d%,
	const %Base% *%z%, const %Base% *%p%, const %Base% *%y%,
	const %Base% *%pz%, %Base% *%py%)%$$
$pre
$$
$codei%inline void RevMulvpOp(size_t %d%,
	const %Base% *%z%, const %Base% *%x%, const %Base% *%p%,
	const %Base% *%pz%, %Base% *%px%)%$$

$head Description$$
We are given the partial derivatives for a function
$latex G(z, x, y)$$ and we wish to compute the partial derivatives for
the function
$latex \[
	H(x, y) = G [ Z(x, y) , x , y ]
\]$$
where $latex Z(x, y)$$ is defined as the 
$th d$$ order Taylor coefficient row vector for $icode Z$$ as
a function of the corresponding vectors for 
$icode X$$ and $italic Y$$ where

$table
Operation  $cnext Value  $rnext
Mulvv       $cnext $latex Z = X * Y$$ $rnext
Mulpv       $cnext $latex Z = P * Y$$ $rnext
Mulvp       $cnext $latex Z = X * P$$ 
$tend

Note that $icode Z$$ has been used both the original multiplication 
function and for the corresponding mapping of Taylor coefficients.

$head z$$
The vector $icode z$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $icode Z$$.

$head x$$
The vector $icode x$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $icode X$$.

$head y$$
The vector $icode y$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $icode Y$$.

$head p$$
The scalar $codei%*%p%$$ contains the value of the parameter $icode P$$.


$head pz$$
The vector $icode pz$$ has length $latex d+1$$ and 
$icode%pz%[%j%]%$$ contains the partial for $icode G$$
with respect to the $th j$$ order Taylor coefficient for $icode Z$$.

$head On Input$$

$subhead px$$
The vector $icode px$$ has length $latex d+1$$ and 
$icode%px%[%j%]%$$ contains the partial for $icode G$$
with respect to the $th j$$ order Taylor coefficient for $icode X$$.

$subhead py$$
The vector $icode py$$ has length $latex d+1$$ and 
$icode%py%[%j%]%$$ contains the partial for $icode G$$
with respect to the $th j$$ order Taylor coefficient for $icode Y$$.

$head On Output$$

$subhead px$$
If present,
the vector $icode px$$ has length $latex d+1$$ and 
$icode%px%[%j%]%$$ contains the partial for $icode H$$
with respect to the $th j$$ order Taylor coefficient for $icode X$$.

$subhead py$$
If present,
the vector $icode py$$ has length $latex d+1$$ and 
$icode%py%[%j%]%$$ contains the partial for $icode H$$
with respect to the $th j$$ order Taylor coefficient for $icode Y$$.

$end
------------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

// --------------------------- Mulvv -----------------------------------------

template <class Base>
inline void ForMulvvOp(size_t j, 
	Base *z, const Base *x, const Base *y)
{
	size_t k;

	z[j] = Base(0);
	for(k = 0; k <= j; k++)
		z[j] += x[j-k] * y[k];
}

template <class Base>
inline void RevMulvvOp(size_t d, 
	const Base  *z, const Base *x, const Base *y,
	const Base *pz, Base      *px, Base      *py)
{	size_t k;

	// number of indices to access
	size_t j = d + 1;

	while(j)
	{	--j;
		for(k = 0; k <= j; k++)
		{	
			px[j-k] += pz[j] * y[k];
			py[k]   += pz[j] * x[j-k];
		}
	}
}

// --------------------------- Mulpv -----------------------------------------

template <class Base>
inline void ForMulpvOp(size_t j, 
	Base *z, const Base *p, const Base *y)
{
	z[j] = (*p) * y[j];
}

template <class Base>
inline void RevMulpvOp(size_t d, 
	const Base  *z, const Base *p, const Base *y,
	const Base *pz, Base *py)
{
	// number of indices to access
	size_t j = d + 1;

	while(j)
	{	--j;
		py[j] += pz[j] * (*p);

	}
}

// --------------------------- Mulvp -----------------------------------------

template <class Base>
inline void ForMulvpOp(size_t j,
	Base *z, const Base *x, const Base *p)
{
	z[j] = x[j] * (*p);
}

template <class Base>
inline void RevMulvpOp(size_t d,
	const Base  *z, const Base *x, const Base *p,
	const Base *pz, Base *px)
{
	// number of indices to access
	size_t j = d + 1;

	while(j)
	{	--j;
		px[j] += pz[j] * (*p);
	}
}

} // END CppAD namespace

# endif
