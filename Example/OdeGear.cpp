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
$begin OdeGear.cpp$$
$spell
	Rosen
$$

$section OdeGear: Example and Test$$

$index OdeGear, example$$
$index example, OdeGear$$
$index test, OdeGear$$

Define 
$latex x : \R \rightarrow \R^n$$ by
$latex \[
	x_i (t) =  t^{i+1}
\] $$ 
for $latex i = 1 , \ldots , n-1$$.
It follows that
$latex \[
\begin{array}{rclr}
x_i(0)     & = & 0                             & {\rm for \; all \;} i \\
x_i ' (t)  & = & 1                             & {\rm if \;} i = 0      \\
x_i '(t)   & = & (i+1) t^i = (i+1) x_{i-1} (t) & {\rm if \;} i > 0
\end{array}
\] $$
The example tests OdeGear using the relations above:

$comment This file is in the Example subdirectory$$ 
$code
$verbatim%Example/OdeGear.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/OdeGear.h>
# include <CppAD/CppAD.h>        // For automatic differentiation

namespace {
	class Fun {
	public:
		// constructor
		Fun(bool use_x_) : use_x(use_x_) 
		{ }

		// compute f(t, x) both for double and AD<double>
		template <typename Scalar>
		void Ode(
			const Scalar              &t, 
			const CppADvector<Scalar> &x, 
			CppADvector<Scalar>       &f)
		{	size_t N  = x.size();	
			Scalar ti(1);
			f[0]   = Scalar(1);
			size_t i;
			for(i = 1; i < N; i++)
			{	ti *= t;
				if( use_x )
					f[i] = (i+1) * x[i-1];
				else	f[i] = (i+1) * ti;
			}
		}

		void Ode_dep(
			const double              &t, 
			const CppADvector<double> &x, 
			CppADvector<double>       &f_x)
		{	using namespace CppAD;

			size_t N  = x.size();	
			CppADvector< AD<double> > T(1);
			CppADvector< AD<double> > X(N);
			CppADvector< AD<double> > F(N);

			// set argument values
			T[0] = t;
			size_t i, j;
			for(i = 0; i < N; i++)
				X[i] = x[i];

			// declare independent variables
			Independent(X);

			// compute f(t, x)
			this->Ode(T[0], X, F);

			// define AD function object
			ADFun<double> Fun(X, F);

			// compute partial of f w.r.t x
			CppADvector<double> dx(N);
			CppADvector<double> df(N);
			for(j = 0; j < N; j++)
				dx[j] = 0.;
			for(j = 0; j < N; j++)
			{	dx[j] = 1.;
				df = Fun.Forward(1, dx);
				for(i = 0; i < N; i++)
					f_x [i * N + j] = df[i];
				dx[j] = 0.;
			}
		}

	private:
		const bool use_x;

	};
}

bool OdeGear(void)
{	bool ok = true; // initial return value
	size_t i, j;    // temporary indices

	size_t  J = 3;  // number of previous steps in multi-step method
	size_t  K = J;  // index of next value in X
	size_t  N = J;  // number of components in x(t)

	// vector of times
	CppADvector<double> T(K+1); 
	double step = .1;
	T[0]        = 0.;
	for(j = 1; j <= K; j++)
	{	T[j] = T[j-1] + step;
		step = 2. * step;
	}

	// initial values for x( T[K-j] ) 
	CppADvector<double> X((K+1) * N);
	for(j = 0; j < K; j++)
	{	double ti = T[j];
		for(i = 0; i < N; i++)
		{	X[ j * N + i ] = ti;
			ti *= T[j];
		}
	}

	size_t use_x;
	for( use_x = 0; use_x < 2; use_x++)
	{	// function object depends on value of use_x
		Fun F(use_x > 0); 

		// compute OdeGear approximation for x( T[K] )
		CppAD::OdeGear(F, J, K, N, T, X);

		double check = T[K];
		for(i = 0; i < N; i++)
		{	// method is exact for orders <= J+1
			ok &= CppAD::NearEqual(
				X[K * N + i], check, 1e-10, 1e-10
			);
			// check value for next i
			check *= T[K];
		}
	}
	return ok;
}

// END PROGRAM
