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
$begin OdeStiff.cpp$$
$spell
	Rosen
$$

$section A Stiff Ode: Example and Test$$

$index stiff, ode$$
$index ode, stiff$$
$index example, stiff ode$$
$index test, stiff ode$$

Define 
$latex x : \R \rightarrow \R^2$$ by
$latex \[
\begin{array}{rcl}
	x_0 (0)        & = & 1 \\
	x_1 (0)        & = & 0 \\
	x_0^\prime (t) & = & - a_0 x_0 (t) \\
	x_1^\prime (t) & = & + a_0 x_0 (t) - a_1 x_1 (t)
\end{array}
\] $$ 
If $latex a_0 \gg a_1 > 0$$, this is a stiff Ode and 
the analytic solution is
$latex \[
\begin{array}{rcl}
x_0 (t)    & = & \exp( - a_0 t ) \\
x_1 (t)    & = & a_0 [ \exp( - a_1 t ) - \exp( - a_0 t ) ] / ( a_0 - a_1 ) 
\end{array}
\] $$
The example tests Rosen34 using the relations above:

$code
$verbatim%Example/OdeStiff.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>        // For automatic differentiation

namespace {
	// --------------------------------------------------------------
	class Fun {
	private:
		CppADvector<double> a;
	public:
		// constructor
		Fun(const CppADvector<double>& a_) : a(a_)
		{ }

		// compute f(t, x) 
		void Ode(
			const double              &t, 
			const CppADvector<double> &x, 
			CppADvector<double>       &f)
		{	f[0]  = - a[0] * x[0];
			f[1]  = + a[0] * x[0] - a[1] * x[1]; 
		}

		// compute partial of f(t, x) w.r.t. t 
		void Ode_ind(
			const double              &t, 
			const CppADvector<double> &x, 
			CppADvector<double>       &f_t)
		{	f_t[0] = 0.;
			f_t[1] = 0.;
		}

		// compute partial of f(t, x) w.r.t. x 
		void Ode_dep(
			const double              &t, 
			const CppADvector<double> &x, 
			CppADvector<double>       &f_x)
		{	f_x[0] = -a[0];  
			f_x[1] = 0.;
			f_x[2] = +a[0];
			f_x[3] = -a[1];
		}
	};

	// --------------------------------------------------------------
	class Method {
	private:
		Fun F;
	public:
		// constructor
		Method(const CppAD::vector<double> &a_) : F(a_)
		{ }
		void step(
			double                 ta , 
			double                 tb , 
			CppAD::vector<double> &xa ,
			CppAD::vector<double> &xb ,
			CppAD::vector<double> &eb )
		{	xb = CppAD::Runge45(F, 1, ta, tb, xa, eb);
		}
		size_t order(void)
		{	return 5; }
	};
}

bool OdeStiff(void)
{	bool ok = true;     // initial return value

	CppAD::vector<double> a(2);
	a[0] = 1e3;
	a[1] = 1.;
	Method method(a);

	CppAD::vector<double> xi(2);
	xi[0] = 1.;
	xi[1] = 0.;

	CppAD::vector<double> eabs(2);
	eabs[0] = 1e-6;
	eabs[1] = 1e-6;

	CppAD::vector<double> ef(2);
	CppAD::vector<double> xf(2);
	CppAD::vector<double> maxabs(2);
	size_t                nstep;

	double ti   = 0.;
	double tf   = 1.;
	double smin = 1e-10;
	double smax = 1.;
	double scur = .5;
	double erel = 0.;
	
	xf = OdeErrControl(method,
	ti, tf, xi, smin, smax, scur, eabs, erel, ef, maxabs, nstep);
	std::cout << "nstep = " << nstep << std::endl;

	double x0 = exp(-a[0]*tf);
	std::cout << "x0         = " << x0 << std::endl;
	std::cout << "xf[0]      = " << xf[0] << std::endl;
	std::cout << "x0 - xf[0] = " << x0 - xf[0] << std::endl;
	std::cout << "ef[0]      = " << ef[0] << std::endl;
	ok &= CppAD::NearEqual(x0, xf[0], 0., eabs[0]);
	ok &= CppAD::NearEqual(0., ef[0], 0., eabs[0]);

	double x1 = a[0] * (exp(-a[1]*tf) - exp(-a[0]*tf))/(a[0] - a[1]);
	std::cout << "x1         = " << x1 << std::endl;
	std::cout << "xf[1]      = " << xf[1] << std::endl;
	std::cout << "x1 - xf[1] = " << x1 - xf[1] << std::endl;
	std::cout << "ef[1]      = " << ef[1] << std::endl;
	ok &= CppAD::NearEqual(x1, xf[1], 0., eabs[1]);
	ok &= CppAD::NearEqual(0., ef[1], 0., eabs[0]);

	return ok;
}

// END PROGRAM
