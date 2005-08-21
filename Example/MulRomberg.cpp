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

# include <CppAD/Romberg.h>
# include <CppAD/CppAD_vector.h>
# include <CppAD/NearEqual.h>


namespace {

	class Fun {
	private:
		const size_t x0deg;
		const size_t x1deg;
	public:
		// constructor
		Fun(size_t x0deg_, size_t x1deg_) 
		: x0deg(x0deg_) , x1deg(x1deg_)
		{ }

		// function F(x0, x1) = x0^x0deg * x1^x1deg
		double operator () (const CppAD::vector<double> &x)
		{	size_t i;
			double   f = 1;
			for(i = 0; i < x0deg; i++)
				f *= x[0];
			for(i = 0; i < x1deg; i++)
				f *= x[1];
			return f;
		}
	};

	class SliceZero {
	private:
		Fun                      F;
		size_t                   m;
		CppAD::vector<double>    x;
	public:
		SliceZero(Fun F_, const double &x1) : F(F_) , x(2)
		{	x[1] = x1; }
		double operator()(const double &x0)
		{	x[0] = x0;
			return F(x);
		}
	};

	class IntegrateZero {
	private:
		Fun           F;
		const double  a;
		const double  b;
		const size_t  n;
		const size_t  p;

	public:
		IntegrateZero(
			Fun           F_ , 
			const double &a_ , 
			const double &b_ , 
			size_t        n_ ,
			size_t        p_ )
		: F(F_) , a(a_) , b(b_) , n(n_) , p(p_) 
		{ }		
		double operator()(const double &x1)
		{	CppAD::vector<double> r(n), e(n);
			SliceZero S(F, x1);
			Romberg(S, a, b, n, r, e);
			return r[p];
		}
	};

}

bool MulRomberg(void)
{	bool ok = true;
	size_t i;

	size_t x0deg = 3;
	size_t x1deg = 3;
	Fun F(x0deg, x1deg);

	CppAD::vector<double> a(2);
	CppAD::vector<double> b(2);
	CppAD::vector<size_t> n(2);
	CppAD::vector<size_t> p(2);
	for(i = 0; i < 2; i++)
	{	a[i] = 0.;
		b[i] = 1.;
		n[i] = 3;
		p[i] = 2;
	}

	IntegrateZero G(F, a[0], b[0], n[0], p[0]);

	CppAD::vector<double> r(n[1]), e(n[1]);
	CppAD::Romberg(G, a[1], b[1], n[1], r, e);

	// int_a1^b1 dx1 int_a0^b0 F(x0,x1) dx0
	//	= [ b0^(x0deg+1) - a0^(x0deg+1) ] / (x0deg+1) 
	//	* [ b1^(x1deg+1) - a1^(x1deg+1) ] / (x1deg+1) 
	double bpow = 1.;
	double apow = 1.;
	for(i = 0; i <= x0deg; i++)
	{	bpow *= b[0];
		apow *= a[0];
	}  
	double check = (bpow - apow) / (x0deg+1);
	bpow = 1.;
	apow = 1.;
	for(i = 0; i <= x1deg; i++)
	{	bpow *= b[1];
		apow *= a[1];
	}  
	check *= (bpow - apow) / (x1deg+1);

	double step = (b[1] - a[1]) / exp(log(2.)*(n[1]-1));
	double spow = 1;
	for(i = 0; i < n[1]; i++)
	{	spow = spow * step * step;
		ok  &= e[i] < (x1deg+1) * spow;
		ok  &= CppAD::NearEqual(check, r[i], 0., e[i]);	

		std::cout << "check = "  << check
		          << ", r[i] = " << r[i]
		          << ", e[i] = " << e[i]
		          << std::endl;
	}

	return ok;
}

// END PROGRAM
