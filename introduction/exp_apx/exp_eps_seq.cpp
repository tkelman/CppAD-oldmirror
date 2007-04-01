/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin exp_eps_seq.cpp$$
$spell
	std
	exp_eps_seq
	cmath
	bool
	fabs
$$


$section exp_eps: Verify Zero Order Forward Sweep$$

$index zero, order exp_eps$$
$index order, zero exp_eps$$
$index exp_eps, zero order$$

$codep */
# include <cmath>              // for fabs function
bool exp_eps_seq(double *v)    // double v[8]
{	bool  ok = true;
	double x = .5;

	v[1] = x;                                  // abs_x = x;
	ok  &= std::fabs( v[1] - 0.5) < 1e-10;

	v[2] = 1. * v[1];                          // temp = term * abx_x;
	ok  &= std::fabs( v[2] - 0.5) < 1e-10;

	v[3] = v[2] / 1.;                          // term = temp / Type(k);
	ok  &= std::fabs( v[3] - 0.5) < 1e-10;

	v[4] = 1. + v[3];                          // sum = sum + term;
	ok  &= std::fabs( v[4] - 1.5) < 1e-10;

	v[5] = v[3] * v[1];                        // temp = term * abs_x;
	ok  &= std::fabs( v[5] - 0.25) < 1e-10;

	v[6] = v[5] / 2.;                          // term = temp / Type(k);
	ok  &= std::fabs( v[6] - 0.125) < 1e-10;

	v[7] = v[4] + v[6];                        // sum = sum + term;
	ok  &= std::fabs( v[7] - 1.625) < 1e-10;

	return ok;
}
bool exp_eps_seq(void)
{	double v[8];
	return exp_eps_seq(v);
}
/* $$
$end
*/

double a[1], q[3], r[3], s[3], k[3]; // global variables set by call
bool exp_eps_seq_old(void)
{	bool  ok = true;
	double x = .5;

	r[0] = s[0] = k[0] = 1.;                   // r = s = k = Type(1);

	a[0] = x;                                  // a = x;
	ok  &= std::fabs( a[0] - 0.5) < 1e-10;

	q[1] = r[0] * a[0];                        // q  = a * r;
	ok  &= std::fabs( q[1] - 0.5) < 1e-10;

	r[1] = q[1] / k[0];                        // r  = q / k;
	ok  &= std::fabs( r[1] - 0.5) < 1e-10;

	s[1] = s[0] + r[1];                        // s  = s + r;
	ok  &= std::fabs( s[1] - 1.5) < 1e-10;

	k[1] = k[0] + 1.;                          // k  = k + Type(1);
	ok  &= std::fabs( k[1] - 2.0) < 1e-10;

	q[2] = r[1] * a[0];                        // q  = a * r;
	ok  &= std::fabs( q[2] - 0.25) < 1e-10;

	r[2] = q[2] / k[1];                        // r  = q / k;
	ok  &= std::fabs( r[2] - 0.125) < 1e-10;

	s[2] = s[1] + r[2];                        // s  = s + r;
	ok  &= std::fabs( s[2] - 1.625) < 1e-10;

	k[2] = k[1] + 1.;                          // k  = k + Type(1);
	ok  &= std::fabs( k[2] - 3.0) < 1e-10;

	return ok;
}
/* $$
$end
*/
