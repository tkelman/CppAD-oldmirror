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
$begin OdeSpeed.cpp$$
$spell
	Cpp
$$

$mindex ODE ordinary differential equation speed test tape$$
$section ODE Solver: Speed Test$$

$comment This file is in the Speed subdirectory$$ 
$code
$verbatim%Speed/Ode.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
/*
Suppose that:
1. for i = 0,       x[i](0) = exp(w[0]), x[i]'(t) = 0
2. for n-1 > i > 0, x[i](0) = 0,         x[i]'(t) = w[i] * x[i-1]
3. for n-1 = i,     x[i](0) = 0,         x[i]'(t) = x[0] * x[1]

It follows that for n-1 > i
	x[i](t) = exp(w[0]) * (w[1]/1) * ... * (w[i]/i) * (t-t0)^i 
and
	x[n-1](t) = exp(2 * w[0]) * w[1] * (t-t0)^2 / 2
*/

# include <CppAD/CppAD.h>

# include <iostream>
# include <cassert>
# include <sstream>
# include <string>

namespace { // BEGIN Empty namespace
	template <class Type>
	class TestFun {
	public:
		TestFun(const CppADvector<Type> &w_)
		{	w.resize( w_.size() );
			w = w_;
		}
		void Ode(
			const Type                &t, 
			const CppADvector<Type>   &x, 
			CppADvector<Type>         &f) 
		{
			size_t n = x.size();
	
			size_t i;
			f[0]  = 0.;
			for(i = 1; i < n-1; i++)
				f[i] = w[i] * x[i-1];
	
			f[n-1] = x[0] * x[1];
		}
	private:
		CppADvector<Type> w;
	};
} // END Empty namespace

std::string doubleOde(size_t size, size_t repeat)
{	using namespace CppAD;
	using CppAD::exp;

	size_t m = size;
	size_t n = m + 1;

	// set up the case where y[i](t) = t^i 
	double           t0 = 0.;
	double           t1 = 1.;
	size_t        nstep = 10;

	CppADvector<double> w(m);
	size_t i;
	for(i = 0; i < m; i++)
		w[i] = double(i);

	// initial and final value of x
	CppADvector<double> x0(n);
	CppADvector<double> x1(n);

	while(repeat--)
	{	// funciton we are integrating
		TestFun<double>  fun(w);

		// value of independent variables
		x0[0] = exp( w[0] );
		for(i = 1; i < n; i++)
			x0[i] = 0.;

		// solve differential equation
		x1 = Runge45(fun, nstep, t0, t1, x0);
	}

	return "double: Ode";
}

std::string ADdoubleOde(size_t size, size_t repeat)
{	using namespace CppAD;
	using CppAD::exp;

	typedef AD<double> ADdouble;

	size_t m = size;
	size_t n = m + 1;

	// set up the case where y[i](t) = t^i 
	ADdouble           t0 = 0.;
	ADdouble           t1 = 1.;
	size_t          nstep = 10;

	CppADvector<ADdouble> w(m);
	size_t i;
	for(i = 0; i < m; i++)
		w[i] = ADdouble(i);

	// initial and final value of x
	CppADvector<ADdouble> x0(n);
	CppADvector<ADdouble> x1(n);

	while(repeat--)
	{	// function we are integrating
		TestFun<ADdouble> fun(w);

		// value of independent variables
		x0[0] = exp( w[0] );
		for(i = 1; i < n; i++)
			x0[i] = 0.;

		// solve differential equation
		x1 = Runge45(fun, nstep, t0, t1, x0);
	}

	return "ADdouble: Ode";
}

std::string TapeOde(size_t size, size_t repeat)
{	using namespace CppAD;
	using CppAD::exp;

	typedef AD<double> ADdouble;

	size_t i;

	size_t m = size;
	size_t n = m + 1;

	// set up the case where y[i](t) = t^i 
	ADdouble           t0 = 0.;
	ADdouble           t1 = 1.;
	size_t          nstep = 10;

	// independent variable vector and values
	CppADvector<double>   w(m);
	CppADvector<ADdouble> W(m);
	for(i = 0; i < m; i++)
		W[i] = w[i] = double(i);

	// initial and final value of X
	CppADvector<ADdouble> X0(n);
	CppADvector<ADdouble> X1(n);

	size_t memory = 0;
	size_t length = 0;
	while(repeat--)
	{	// declare independent variables
		Independent(W);

		// function we are integrating
		TestFun<ADdouble> fun(W);

		// value of independent variable
		X0[0] = exp( W[0] );
		for(i = 1; i < n; i++)
			X0[i] = 0.;

		// solve differential equation
		X1 = Runge45(fun, nstep, t0, t1, X0);

		// create f : W -> X1
		ADFun<double> f(W, X1);

		// save for later return
		memory = f.Memory();
		length = f.Size();
	}

	std::ostringstream buf;
	buf << "Tape of ODE w.r.t. parameter vector: Length = ";;
	buf << length << ", Memory = " << memory;

	return buf.str();
}

std::string JacOde(size_t size, size_t repeat)
{	using namespace CppAD;
	using CppAD::exp;

	typedef AD<double> ADdouble;

	size_t i;

	size_t m = size;
	size_t n = m + 1;

	// set up the case where y[i](t) = t^i 
	ADdouble           t0 = 0.;
	ADdouble           t1 = 1.;
	size_t          nstep = 10;

	// independent variable vector and values
	CppADvector<double>   w(m);
	CppADvector<ADdouble> W(m);
	for(i = 0; i < m; i++)
		W[i] = w[i] = double(i);

	// initial and final value of X
	CppADvector<ADdouble> X0(n);
	CppADvector<ADdouble> X1(n);

	// Jacobian
	CppADvector<double> J(n * m);

	size_t memory = 0;
	size_t length = 0;
	while(repeat--)
	{	// declare independent variables
		Independent(W);

		// function we are integrating
		TestFun<ADdouble> fun(W);

		// value of independent variable
		X0[0] = exp( W[0] );
		for(i = 1; i < n; i++)
			X0[i] = 0.;

		// solve differential equation
		X1 = Runge45(fun, nstep, t0, t1, X0);

		// create f : W -> X1
		ADFun<double> f(W, X1);

		J = f.Jacobian(w);

		// save for later return
		memory = f.Memory();
		length = f.Size();
	}

	std::ostringstream buf;
	buf << "Jacobian of ODE w.r.t. parameter vector: Length = ";;
	buf << length << ", Memory = " << memory;

	return buf.str();
}

std::string HesOde(size_t size, size_t repeat)
{	using namespace CppAD;
	using CppAD::exp;

	typedef AD<double> ADdouble;

	size_t i;

	size_t m = size;
	size_t n = m + 1;

	// set up the case where y[i](t) = t^i 
	ADdouble           t0 = 0.;
	ADdouble           t1 = 1.;
	size_t          nstep = 10;

	// independent variable vector and values
	CppADvector<double>   w(m);
	CppADvector<ADdouble> W(m);
	for(i = 0; i < m; i++)
		W[i] = w[i] = double(i);

	// initial and final value of X
	CppADvector<ADdouble> X0(n);
	CppADvector<ADdouble> X1(n);

	// Hessian for one component function
	CppADvector<double> Hi(m * m);

	size_t memory = 0;
	size_t length = 0;
	while(repeat--)
	{	// declare independent variables
		Independent(W);

		// function we are integrating
		TestFun<ADdouble> fun(W);

		// value of independent variable
		X0[0] = exp( W[0] );
		for(i = 1; i < n; i++)
			X0[i] = 0.;

		// solve differential equation
		X1 = Runge45(fun, nstep, t0, t1, X0);

		// create f : W -> X1
		ADFun<double> f(W, X1);

		// evaluate Hessian for each component function 
		for(i = 0; i < n; i++)
			Hi = f.Hessian(w, i);

		// save for later return
		memory = f.Memory();
		length = f.Size();
	}

	std::ostringstream buf;
	buf << "Hessian of ODE w.r.t. parameter vector: Length = ";
	buf << length << ", Memory = " << memory;

	return buf.str();
}


// END PROGRAM