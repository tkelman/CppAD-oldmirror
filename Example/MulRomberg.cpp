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

	class TestFun {
	private:
		const CppAD::vector<size_t> deg;
	public:
		// constructor
		TestFun(const CppAD::vector<size_t> deg_) 
		: deg(deg_)
		{ }

		// function F(x) = x[0]^deg[0] * x[1]^deg[1]
		double operator () (const CppAD::vector<double> &x)
		{	size_t i;
			double   f = 1;
			for(i = 0; i < deg[0]; i++)
				f *= x[0];
			for(i = 0; i < deg[1]; i++)
				f *= x[1];
			return f;
		}
	};

	template <class Fun>
	class SliceLast {
	private:
		Fun                      F;
		size_t                   m;
		CppAD::vector<double>    x;
	public:
		SliceLast(Fun F_, size_t m_, const CppAD::vector<double> &x_) 
		: F(F_) , m(m_), x(m + 1)
		{	size_t i;
			for(i = 0; i < m; i++)
				x[i] = x_[i];
		}
		double operator()(const double &xm)
		{	x[m] = xm;
			return F(x);
		}
	};

	template <class Fun>
	class IntegrateLast {
	private:
		Fun                         F; 
		const size_t                m;
		const CppAD::vector<double> a; 
		const CppAD::vector<double> b; 
		const CppAD::vector<size_t> n; 
		const CppAD::vector<size_t> p; 
		double                      esum;
		size_t                      ecount;

	public:
		IntegrateLast(
			Fun                         &F_ , 
			size_t                       m_ ,
			const CppAD::vector<double> &a_ , 
			const CppAD::vector<double> &b_ , 
			const CppAD::vector<size_t> &n_ , 
			const CppAD::vector<size_t> &p_ ) 
		: F(F_) , m(m_), a(a_) , b(b_) , n(n_) , p(p_) 
		{ }		
		double operator()(const CppAD::vector<double> &x)
		{	double r, e;
			SliceLast<Fun> S(F, m, x);
			r     = CppAD::Romberg(S, a[m], b[m], n[m], p[m], e);
			esum += e;
			ecount++;
			return r;
		}
		void ClearEsum(void)
		{	esum   = 0.; }
		double GetEsum(void)
		{	return esum; }

		void ClearEcount(void)
		{	ecount   = 0; }
		size_t GetEcount(void)
		{	return ecount; }
	};

	template <class Fun>
	double MulRomberg(
		Fun                         &F  , 
		const CppAD::vector<double> &a  ,
		const CppAD::vector<double> &b  ,
		const CppAD::vector<size_t> &n  ,
		const CppAD::vector<size_t> &p  ,
		double                      &e  )
	{	double r;
		size_t m = a.size();
		if( m == 1 )
		{	IntegrateLast<Fun> F0(F, 0, a, b, n, p);
			F0.ClearEsum();
			F0.ClearEcount();
			r  = F0(a);
			e  = F0.GetEsum();
			assert( F0.GetEcount() == 1 );
		}
		else if( m == 2 )
		{	IntegrateLast<Fun> F1(F, 1, a, b, n, p);
			IntegrateLast< IntegrateLast<Fun> > 
				F0(F1, 0, a, b, n, p);

			F1.ClearEsum();
			F1.ClearEcount();
			F0.ClearEsum();
			F0.ClearEcount();

			r  = F0(a);

			e  = F0.GetEsum();
			e += F1.GetEsum() * (b[0] - a[0]) / F0.GetEcount();
		}
		else	assert(0);
		
		return r;
	}

}

bool MulRomberg(void)
{	bool ok = true;
	size_t i;
	size_t k;

	CppAD::vector<size_t> deg(2);
	deg[0] = 4;
	deg[1] = 3;
	TestFun F(deg);

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
	//	= [ b0^(deg[0]+1) - a0^(deg[0]+1) ] / (deg[0]+1) 
	//	* [ b1^(deg[1]+1) - a1^(deg[1]+1) ] / (deg[1]+1) 
	double bpow = 1.;
	double apow = 1.;
	for(i = 0; i <= deg[0]; i++)
	{	bpow *= b[0];
		apow *= a[0];
	}  
	double check = (bpow - apow) / (deg[0]+1);
	bpow = 1.;
	apow = 1.;
	for(i = 0; i <= deg[1]; i++)
	{	bpow *= b[1];
		apow *= a[1];
	}  
	check *= (bpow - apow) / (deg[1]+1);

	double step = (b[1] - a[1]) / exp(log(2.)*(n[1]-1));
	double spow = 1;
	for(k = 0; k < n[1]; k++)
	{	spow = spow * step * step;

		for(i = 0; i < 2; i++)
			p[i] = k;
		r    = MulRomberg(F, a, b, n, p, e);

		ok  &= e < (deg[1]+1) * spow;
		ok  &= CppAD::NearEqual(check, r, 0., e);	

		std::cout << "check = "  << check
		          << ", r = "    << r
		          << ", e = "    << e
		          << std::endl;
	}

	return ok;
}

// END PROGRAM
