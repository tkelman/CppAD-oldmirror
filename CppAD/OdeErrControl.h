# ifndef CppADOdeErrControlIncluded
# define CppADOdeErrControlIncluded

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
$begin OdeErrControl$$

$spell
	exp
	scur
	CppAD
	xf
	tf
	xi
	smin
	smax
	eabs
	erel
	ef
	ta
	tb
	xa
	xb
	const
	eb
$$

$index OdeErrControl$$
$index ODE, control error$$
$index control, ODE error$$
$index error, control ODE$$
$index differential, ODE error control$$
$index equation, ODE error control$$

 
$section An Error Controller for ODE Solvers$$

$table
$bold Syntax$$ 
$cnext $code # include <CppAD/OdeErrControl.h>$$
$rnext $cnext
$syntax%%xf% = OdeErrControl(%method%, 
	%ti%, %tf%, %xi%, %smin%, %smax%, %scur%, %eabs%, %erel%, %ef% )%$$
$tend

$fend 20$$

$head Description$$
Let $latex \R$$ denote the real numbers
and let $latex F : \R \times \R^n \rightarrow \R^n$$ be a smooth function.
We define $latex X : [ti , tf] \rightarrow \R^n$$ by 
the following initial value problem:
$latex \[
\begin{array}{rcl}
	X(ti)  & = & xi    \\
	X'(t)  & = & F[t , X(t)] 
\end{array}
\] $$
The routine $code OdeErrControl$$ can be used to adjust the step size
used by either of these methods in order to be as fast as possible
and still with in a requested error bound.

$head Notation$$
The template parameter types $xref/OdeErrControl/Scalar/Scalar/$$ and
$xref/OdeErrControl/Vector/Vector/$$ are documented below.

$head Method$$
The class $italic Method$$
and the object $italic method$$ satisfy the following syntax
$syntax%
	%Method% &%method%
%$$
The object $italic method$$ must support the following syntax 
$syntax%

%method%.step(%ta%, %tb%, %xa%, %xb%, %eb%)
%$$
$subhead ta$$
The argument $italic ta$$ has prototype
$syntax%
	const %Scalar% &%ta%
%$$
It specifies the initial time for this step in the 
ODE integration.
(see description of $xref/OdeErrControl/Scalar/Scalar/$$ below). 

$subhead tb$$
The argument $italic tb$$ has prototype
$syntax%
	const %Scalar% &%tb%
%$$
It specifies the final time for this step in the 
ODE integration.

$subhead xa$$
The argument $italic xa$$ has prototype
$syntax%
	const %Vector% &%xa%
%$$
and size $italic n$$.
It specifies the value of $latex X(ta)$$.
(see description of $xref/OdeErrControl/Vector/Vector/$$ below). 

$subhead xb$$
The argument value $italic xb$$ has prototype
$syntax%
	%Vector% &%xb%
%$$
and size $italic n$$.
The input value of its elements does not matter.
On output, 
it contains the approximation for $latex X(tb)$$ that the method obtains.

$subhead eb$$
The argument value $italic eb$$ has prototype
$syntax%
	%Vector% &%eb%
%$$
and size $italic n$$.
The input value of its elements does not matter.
On output, 
it contains an estimate for the error in the approximation $italic xb$$.
It is assumed (locally) that the error bound in this approximation 
nearly equal to $latex K (tb - ta)^m$$ 
where $italic K$$ is a fixed constant and $italic m$$
is the corresponding argument to $code CodeControl$$.
$pre

$$
If $italic m$$ is $code size_t$$,
the object $italic method$$ must also support the following syntax 
$syntax%

%m% = %method%.order()
%$$
The return value $italic m$$ is the order of the ODE integration method;
i.e., there is a constant K such that if $latex ti \leq ta \leq tb \leq tf$$,
$latex \[
	| X(tb) - xb | \leq K | tb - ta |^m
\] $$
where $italic ta$$, $italic tb$$, and $italic xb$$ are as in 
$syntax%%method%.step(%ta%, %tb%, %xa%, %xb%, %eb%)%$$


$head ti$$
The argument $italic ti$$ has prototype
$syntax%
	const %Scalar% &%ti%
%$$
It specifies the initial time for the integration of 
the differential equation.


$head tf$$
The argument $italic tf$$ has prototype
$syntax%
	const %Scalar% &%tf%
%$$
It specifies the final time for the integration of 
the differential equation.

$head xi$$
The argument $italic xi$$ has prototype
$syntax%
	const %Vector% &%xi%
%$$
and size $italic n$$.
It specifies value of $latex X(ti)$$.

$head smin$$
The argument $italic smin$$ has prototype
$syntax%
	const %Scalar% &%smin%
%$$
The step size during a call to $italic method$$ is defined as
the corresponding value of $latex tb - ta$$.
If $latex tf - ti \leq smin$$,
the integration will be done in one step of size $italic tf - ti$$.
Otherwise,
the minimum value of $italic tb - ta$$ will be $latex smin$$
except for the last two calls to $italic method$$ where it may be
as small as $latex smin / 2$$.

$head smax$$
The argument $italic smax$$ has prototype
$syntax%
	const %Scalar% &%smax%
%$$
It specifies the maximum step size to use during the integration; 
i.e., the maximum value for $latex tb - ta$$ in a call to $italic method$$.
The value of $italic smax$$ must be greater than or equal $italic smin$$.

$head scur$$
The argument $italic scur$$ has prototype
$syntax%
	%Scalar% &%scur%
%$$
The value of $italic scur$$ is the suggested next step size,
based on error criteria, to try in the next call to $italic method$$.
On input it corresponds to the first call to $italic method$$,
in this call to $code OdeErrControl$$ (where $latex ta = ti$$).
On output it corresponds to the next call to $italic method$$,
in a subsequent call to $code OdeErrControl$$ (where $italic ta = tf$$).

$head eabs$$
The argument $italic eabs$$ has prototype
$syntax%
	const %Vector% &%eabs%
%$$
and size $italic n$$.
Each of the elements of $italic eabs$$ must be 
greater than or equal zero.
It specifies a bound for the absolute
error in the return value $italic xf$$ as an approximation for $latex X(tf)$$.
(see description of total error bound under $italic erel$$).

$head erel$$
The argument $italic erel$$ has prototype
$syntax%
	const %Scalar% &%erel%
%$$
and is greater than or equal zero.
It specifies a bound for the relative 
error in the return value $italic xf$$ as an approximation for $latex X(tf)$$.
The total error bound is the sum of the absolute and relative error bounds; 
i.e., the error bound for the $th j$$ component is 
$latex \[
	| xf[j] - X_j (tf) | \leq eabs[j] + erel * | X_j (tf) |
\] $$

$head ef$$
The argument value $italic ef$$ has prototype
$syntax%
	%Vector% &%ef%
%$$
and size $italic n$$.
The input value of its elements does not matter.
On output, 
it contains an estimate for the 
absolute error in the approximation $italic xf$$; i.e.,
$latex \[
	ef_i \approx | X( tf )_i - xf_i |
\] $$

$head Scalar$$
The following operations must be defined for $italic Scalar$$ objects
$italic a$$ and $italic b$$:

$table
$bold Operation$$ $cnext $bold Description$$  $rnext
$syntax%%Scalar% %a%(%i%)%$$ $cnext
	constructs a $italic Scalar$$ object from the $code int$$ $italic i$$
$rnext
$syntax%%Scalar% %a%(%b%)%$$ $cnext
	constructs $italic a$$ with the same value as $italic b$$.
$rnext
$syntax%%a% <= %b%$$ $cnext
	returns true (false) if $italic a$$ is less than or equal 
	(greater than) $italic b$$.
$rnext
$syntax%%a% == %b%$$ $cnext
	returns true (false) if $italic a$$ is equal to $italic b$$.
$rnext
$syntax%%a% = %b%$$ $cnext
	changes the value of $italic a$$ so it is equal to $italic b$$.
$rnext
$syntax%%a% + %b%$$ $cnext
	returns a $italic Scalar$$ equal to the sum $latex a + b$$
$rnext
$syntax%%a% - %b%$$ $cnext
	returns a $italic Scalar$$ equal to the difference $latex a - b$$ 
$rnext
$syntax%%a% * %b%$$ $cnext
	returns a $italic Scalar$$ equal to the product $latex a * b$$ 
$rnext
$syntax%%a% / %b%$$ $cnext
	returns a $italic Scalar$$ equal to the quotient $latex a / b$$ 
$rnext
$syntax%log(%a%)%$$ $cnext
	returns a $italic Scalar$$ equal to the logarithm of $italic a$$
$rnext
$syntax%exp(%a%)%$$ $cnext
	returns a $italic Scalar$$ equal to the exponential of $italic a$$
$tend


$head Vector$$
The type $italic Vector$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type Scalar/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head Example$$
$children%
	Example/OdeErrControl.cpp
%$$
The file
$xref/OdeErrControl.cpp/$$
contains an example and test a test of using this routine.
It returns true if it succeeds and false otherwise.

$head Theory$$
Let $latex e(s)$$ be the error as a function of the
step size $latex s$$ and suppose that there is a constant
$latex K$$ such that $latex e(s) = K s^m$$.
Let $latex a$$ be our error bound.
Given the value of $latex e(s)$$, a step of size $latex \lambda s$$
would be ok provided that
$latex \[
\begin{array}{rcl}
	a  & \geq & e( \lambda s ) (tf - ti) / ( \lambda s ) \\
	a  & \geq & K \lambda^m s^m (tf - ti) / ( \lambda s ) \\
	a  & \geq & \lambda^{m-1} s^{m-1} (tf - ti) e(s) / s^m \\
	a  & \geq & \lambda^{m-1} (tf - ti) e(s) / s           \\
	\lambda^{m-1} & \leq & \frac{a}{e(s)} \frac{s}{tf - ti}
\end{array}
\] $$
Thus if the right hand side of the last inequality is greater 
than or equal to one, the step of size $latex s$$ is ok. 

$head Source Code$$
The source code for this routine is in the file
$code CppAD/OdeErrControl.h$$.

$end
--------------------------------------------------------------------------
*/

# include <CppAD/CppADError.h>
# include <CppAD/CheckSimpleVector.h>

namespace CppAD { // Begin CppAD namespace

template <typename Scalar, typename Vector, typename Method>
Vector OdeErrControl(
	Method          &method, 
	const Scalar    &ti    , 
	const Scalar    &tf    , 
	const Vector    &xi    , 
	const Scalar    &smin  , 
	const Scalar    &smax  , 
	Scalar          &scur  ,
	const Vector    &eabs  , 
	const Scalar    &erel  , 
	Vector          &ef    ) 
{
	// check simple vector class specifications
	CheckSimpleVector<Scalar, Vector>();

	size_t n = xi.size();

	CppADUsageError(
		smin <= smax,
		"Error in OdeErrControl: smin > smax"
	);
	CppADUsageError(
		eabs.size() == n,
		"Error in OdeErrControl: size of eabs is not equal to n"
	);
	size_t m = method.order();
	CppADUsageError(
		m > 1,
		"Error in OdeErrControl: m is less than or equal one"
	);

	size_t i;
	Vector xa(n), xb(n), eb(n);

	// initialization
	Scalar zero(0);
	Scalar one(1);
	Scalar two(2);
	Scalar m1(m-1);
	Scalar ta = ti;
	for(i = 0; i < n; i++)
	{	ef[i] = zero;
		xa[i] = xi[i];
	}  

	Scalar tb, step, lambda, axbi, a, r, root;
	while( ! (ta == tf) )
	{	// start with value suggested by error criteria
		step = scur;

		// check maximum
		if( step >= smax )
			step = smax;

		// check minimum
		if( step <= smin )
			step = smin;

		// check if near the end
		if( tf <= ta + 1.5 * step )
			tb = tf;
		else	tb = ta + step;

		// try using this step size
		method.step(ta, tb, xa, xb, eb);
		step = tb - ta;

		// compute value of lambda for this step
		lambda = Scalar(10) * scur / step;
		for(i = 0; i < n; i++)
		{	if( zero <= xb[i] )
				axbi = xb[i];
			else	axbi = -xb[i];
			a    = eabs[i] + erel * axbi;
			if( ! (eb[i] == zero) )
			{	r = ( a / eb[i] ) * step / (tf - ti);
				root = exp( log(r) / m1 ); 
				if( root <= lambda )
					lambda = root;
			}
		}
		if( one <= lambda || step <= 1.5 * smin)
		{	// this step is within error limits or 
			// close to the minimum size
			ta = tb;
			for(i = 0; i < n; i++)
			{	xa[i]  = xb[i];
				ef[i] += eb[i];
			}
		}

		// step suggested by error criteria 
		// do not use last step becasue it may be very small
		if( ! (ta == tf) )
			scur = lambda * step / two;
	}
	return xa;
}

} // End CppAD namespace 

# endif
