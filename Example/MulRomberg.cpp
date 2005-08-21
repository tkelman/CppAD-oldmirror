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

	class SliceLast {
	private:
		Fun                      F;
		size_t                   m;
		CppAD::vector<double>    x;
	public:
		SliceLast(Fun F_, const double &x0) : F(F_) , x(2)
		{	x[0] = x0; }
		double operator()(const double &x1)
		{	x[1] = x1;
			return F(x);
		}
	};

	class IntegrateLast {
	private:
		Fun           F;
		const double  a;
		const double  b;
		const size_t  n;
		const size_t  p;
		double        esum;

	public:
		IntegrateLast(
			Fun           F_ , 
			const double &a_ , 
			const double &b_ , 
			size_t        n_ ,
			size_t        p_ )
		: F(F_) , a(a_) , b(b_) , n(n_) , p(p_) 
		{ }		
		double operator()(const double &x0)
		{	double r, e;
			SliceLast S(F, x0);
			r     = CppAD::Romberg(S, a, b, n, p, e);
			esum += e;
			return r;
		}
		void SetEsum(const double &esum_)
		{	esum = esum_; }
		double GetEsum(void)
		{	return esum; }
	};

	double MulRomberg(
		Fun                         &F  , 
		const CppAD::vector<double> &a  ,
		const CppAD::vector<double> &b  ,
		const CppAD::vector<size_t> &n  ,
		const CppAD::vector<size_t> &p  ,
		double                      &e  )
	{	double r, s0;
		IntegrateLast G(F, a[1], b[1], n[1], p[1]);
		G.SetEsum(0.);
		r  = CppAD::Romberg(G, a[0], b[0], n[0], p[0], e);
		s0 = (b[0] - a[0]) / exp( log(2.) * (n[0] - 1) );
		e += G.GetEsum() * s0;
		return r;
	}

}

bool MulRomberg(void)
{	bool ok = true;
	size_t i;
	size_t k;

	size_t x0deg = 4;
	size_t x1deg = 3;
	Fun F(x0deg, x1deg);

	// arugments to MulRomberg
	CppAD::vector<double> a(2);
	CppAD::vector<double> b(2);
	CppAD::vector<size_t> n(2);
	CppAD::vector<size_t> p(2);
	for(i = 0; i < 2; i++)
	{	a[i] = 0.;
		b[i] = 1.;
		n[i] = 4;
	}
	double r, e;

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
	for(k = 0; k < n[1]; k++)
	{	spow = spow * step * step;

		for(i = 0; i < 2; i++)
			p[i] = k;
		r    = MulRomberg(F, a, b, n, p, e);

		ok  &= e < (x1deg+1) * spow;
		ok  &= CppAD::NearEqual(check, r, 0., e);	

		std::cout << "check = "  << check
		          << ", r = "    << r
		          << ", e = "    << e
		          << std::endl;
	}

	return ok;
}

// END PROGRAM
