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

	template <class Fun, class FloatVector>
	class SliceLast {
		typedef typename FloatVector::value_type Scalar;
	private:
		Fun        *F;
		size_t      last;
		FloatVector x;
	public:
		SliceLast( Fun *F_, size_t last_, const FloatVector &x_ ) 
		: F(F_) , last(last_), x(last + 1)
		{	size_t i;
			for(i = 0; i < last; i++)
				x[i] = x_[i];
		}
		double operator()(const Scalar &xlast)
		{	x[last] = xlast;
			return (*F)(x);
		}
	};

	template <class Fun, class SizeVector, class FloatVector, class Scalar>
	class IntegrateLast {
	private:
		Fun                        *F; 
		const size_t                last;
		const FloatVector           a; 
		const FloatVector           b; 
		const SizeVector            n; 
		const SizeVector            p; 
		Scalar                      esum;
		size_t                      ecount;

	public:
		IntegrateLast(
			Fun                         *F_    , 
			size_t                       last_ ,
			const FloatVector           &a_    , 
			const FloatVector           &b_    , 
			const SizeVector            &n_    , 
			const SizeVector            &p_    ) 
		: F(F_) , last(last_), a(a_) , b(b_) , n(n_) , p(p_) 
		{ }		
		Scalar operator()(const FloatVector           &x)
		{	Scalar r, e;
			SliceLast<Fun, FloatVector           > S(F, last, x);
			r     = CppAD::Romberg(
				S, a[last], b[last], n[last], p[last], e
			);
			esum += e;
			ecount++;
			return r;
		}
		void ClearEsum(void)
		{	esum   = 0.; }
		Scalar GetEsum(void)
		{	return esum; }

		void ClearEcount(void)
		{	ecount   = 0; }
		size_t GetEcount(void)
		{	return ecount; }
	};

	template <class Fun, class SizeVector, class FloatVector, size_t m>
	class RombergMul {
		typedef typename FloatVector::value_type Scalar;
	public:
		RombergMul(void)
		{	}
		Scalar operator() (
			Fun                         &F  , 
			const FloatVector           &a  ,
			const FloatVector           &b  ,
			const SizeVector            &n  ,
			const SizeVector            &p  ,
			Scalar                      &e  )
		{	Scalar r;

			typedef IntegrateLast<
				Fun                  , 
				SizeVector           , 
				FloatVector          , 
				Scalar               > IntegrateOne;

			IntegrateOne Fm1(&F, m-1, a, b, n, p);
			RombergMul<
				IntegrateOne, 
				SizeVector  ,
				FloatVector ,
				m-1         > RombergMulM1;

			Fm1.ClearEsum();
			Fm1.ClearEcount();

			r  = RombergMulM1(Fm1, a, b, n, p, e);

			size_t i, j;
			Scalar prod = 1;
			size_t pow2 = 1;
			for(i = 0; i < m-1; i++)
			{	prod *= (b[i] - a[i]);
				for(j = 0; j < (n[i] - 1); j++)
					pow2 *= 2;
			}
			assert( Fm1.GetEcount() == (pow2+1) );
			
			e += Fm1.GetEsum() * prod / Fm1.GetEcount();

			return r;
		}
	};

	template <class Fun, class SizeVector, class FloatVector>
	class RombergMul <Fun, SizeVector, FloatVector, 1> {
		typedef typename FloatVector::value_type Scalar;
	public:
		Scalar operator() (
			Fun                         &F  , 
			const FloatVector           &a  ,
			const FloatVector           &b  ,
			const SizeVector            &n  ,
			const SizeVector            &p  ,
			Scalar                      &e  )
		{	Scalar r;
			typedef IntegrateLast<
				Fun                  , 
				SizeVector           , 
				FloatVector          , 
				Scalar               > IntegrateOne;

			IntegrateOne F0(&F, 0, a, b, n, p);

			F0.ClearEsum();
			F0.ClearEcount();

			r  = F0(a);

			assert( F0.GetEcount() == 1 );
			e = F0.GetEsum();

			return r;
		}
	};

}

bool MulRomberg(void)
{	bool ok = true;
	size_t i;
	size_t k;

	CppAD::vector<size_t> deg(2);
	deg[0] = 5;
	deg[1] = 3;
	TestFun F(deg);

	RombergMul<
		TestFun              , 
		CppAD::vector<size_t>, 
		CppAD::vector<double>, 
		2                    > RombergMulTest;

	// arugments to RombergMul
	CppAD::vector<double> a(2);
	CppAD::vector<double> b(2);
	CppAD::vector<size_t> n(2);
	CppAD::vector<size_t> p(2);
	for(i = 0; i < 2; i++)
	{	a[i] = 0.;
		b[i] = 1.;
	}
	n[0] = 4;
	n[1] = 3;
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
	for(k = 0; k <= n[1]; k++)
	{	spow = spow * step * step;
		double bnd = 3 * (deg[1] + 1) * spow;

		for(i = 0; i < 2; i++)
			p[i] = k;
		r    = RombergMulTest(F, a, b, n, p, e);

		ok  &= e < bnd;
		ok  &= CppAD::NearEqual(check, r, 0., e);	

		std::cout << "check = "  << check
		          << ", r = "    << r
		          << ", e = "    << e
		          << ", bnd = "  << bnd
		          << std::endl;
	}

	return ok;
}

// END PROGRAM
