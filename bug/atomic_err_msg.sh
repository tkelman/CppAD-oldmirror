#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
	mkdir build
fi
cd build
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
echo "create $name.cpp"
cat << EOF > $name.cpp
# include <cppad/cppad.hpp>
using CppAD::vector;
using CppAD::AD;
//
class atomic_get_started : public CppAD::atomic_base<double> {
public:
	atomic_get_started(const std::string& name) : 
	CppAD::atomic_base<double>(name)
	{ }
private:
	// ----------------------------------------------------------------------
	// forward mode routine called by CppAD
	virtual bool forward(
		size_t                    q ,
		size_t                    p ,
		const vector<bool>&      vx ,
		      vector<bool>&      vy ,
		const vector<double>&    tx ,
		      vector<double>&    ty
	)
	{	size_t n = tx.size() / (p + 1);
		size_t m = ty.size() / (p + 1);
		assert( n == 1 );
		assert( m == 1 );

		// return flag
		bool ok = p == 0;
		if( ! ok )
			return ok;

		// check for defining variable information
		// This case must always be implemented
		if( vx.size() > 0 )
			vy[0] = vx[0];

		// Order zero forward mode.
		// This case must always be implemented
		// y^0 = f( x^0 ) = 1 / x^0
		double f = 1. / tx[0];
		if( q <= 0 )
			ty[0] = f;
		return ok;
	}
};
AD<double> atomic_simple(const AD<double>& x)
{	// should have static infront here
	atomic_get_started afun("atomic_simple");
	vector< AD<double> > vx(1);
	vector< AD<double> > vy(1);
	vx[0] = x;
	afun(vx, vy);
	return vy[0];
}

int main(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	double eps = 10. * CppAD::numeric_limits<double>::epsilon();

	// domain space vector
	size_t n  = 1;
	double  x0 = 0.5;
	vector< AD<double> > ax(n);
	ax[0]     = x0;

	// declare independent variables and start tape recording
	CppAD::Independent(ax);

	// range space vector 
	size_t m = 1;
	vector< AD<double> > ay(m);

	// call user function and store get_started(x) in au[0] 
	AD<double>  au = atomic_simple( ax[0] );

	// now use AD division to invert to invert the operation
	ay[0] = 1.0 / au;  // y = 1 / u = x

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f;
	f.Dependent (ax, ay);  // f(x) = x

	// --------------------------------------------------------------------
	// Check forward mode results
	//
	// check function value 
	double check = x0;
	ok &= NearEqual( Value(ay[0]) , check,  eps, eps);

	// check zero order forward mode
	size_t p;
	vector<double> x_p(n), y_p(m);
	p      = 0;
	x_p[0] = x0;
	// ---------------------------------------------------------------------
	std::cout << "Expect a meaning error message" << std::endl;
	y_p    = f.Forward(p, x_p);
	// ---------------------------------------------------------------------
	ok &= NearEqual(y_p[0] , check,  eps, eps);

	return ok;
}
EOF
echo "g++ -g $name.cpp -I../.. -std=c++11 -o $name"
g++ -g $name.cpp -I../.. -std=c++11 -o $name
#
echo "./$name"
./$name
#
echo "rm $name $name.cpp"
rm $name $name.cpp
