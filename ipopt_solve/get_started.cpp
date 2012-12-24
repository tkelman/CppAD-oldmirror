/* $Id: get_started.cpp 2506 2012-10-24 19:36:49Z bradbell $ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*

$begin ipopt_solve_get_started.cpp$$
$spell
	cppad_nlp
	IpoptDir
	CppAD
$$
$index example, nonlinear, programming$$
$index programming, nonlinear example$$
$index ipopt, AD example$$

$section Nonlinear Programming Using CppAD and Ipopt: Example and Test$$

$head Purpose$$
This example program demonstrates how to use the class cppad_ipopt_nlp to 
solve the example problem in the Ipopt documentation; i.e., the problem
$latex \[
\begin{array}{lc}
{\rm minimize \; }      &  x_1 * x_4 * (x_1 + x_2 + x_3) + x_3
\\
{\rm subject \; to \; } &  x_1 * x_2 * x_3 * x_4  \geq 25
\\
                        &  x_1^2 + x_2^2 + x_3^2 + x_4^2 = 40
\\
                        &  1 \leq x_1, x_2, x_3, x_4 \leq 5  
\end{array}
\] $$


$head Configuration Requirement$$
This example will be compiled and tested provided that
the value $cref/ipopt_dir/InstallUnix/ipopt_dir/$$ is specified on the 
$cref/configure/InstallUnix/Configure/$$ command line.

$code
$verbatim%ipopt_solve/get_started.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

# include <ipopt_solve.hpp>

namespace {
	class FG_eval {
	public:
		typedef CppAD::vector< AD<double> >  ADvector;
		typedef AD<double>                   ADdouble;
		void operator()(ADvector& fg, const ADvector& x)
		{	assert( fg.size() == 3 );
			assert( x.size()  == 4 );

			// Fortran style indexing 
			ADdouble x1 = x[0];
			ADdouble x2 = x[1];
			ADdouble x3 = x[2];
			ADdouble x4 = x[3];
			// f(x)
			fg[0] = x1 * x4 * (x1 + x2 + x3) + x3;
			// g_1 (x)
			fg[1] = x1 * x2 * x3 * x4;
			// g_2 (x)
			fg[2] = x1 * x1 + x2 * x2 + x3 * x3 + x4 * x4;
			//
			return;
		}
	};
}
	
bool ipopt_solve_get_started(void)
{	bool ok = true;
	size_t i;
	typedef CppAD::vector<double> Dvector;


	// number of independent variables (domain dimension for f and g)
	size_t nx = 4;  
	// number of constraints (range dimension for g)
	size_t ng = 2;
	// initial value of the independent variables
	Dvector xi(nx);
	xi[0] = 1.0;
	xi[1] = 5.0;
	xi[2] = 5.0;
	xi[3] = 1.0;
	// lower and upper limits for x
	Dvector xl(nx), xu(nx);
	for(i = 0; i < nx; i++)
	{	xl[i] = 1.0;
		xu[i] = 5.0;
	}
	// lower and upper limits for g
	Dvector gl(ng), gu(ng);
	gl[0] = 25.0;     gu[0] = 1.0e19;
  	gl[1] = 40.0;     gu[1] = 40.0;

	// object that computes objective and constraints
	FG_eval fg_eval();

	// options file
	const char* options = "ipopt_solve.opt";

	// write out the options file
	std::ofstream options_file(options);

	// turn off any printing
	options_file << "print_level                0" << std::endl;

	// maximum number of iterations
	options_file << "max_iter                   10" << std::endl;

	// approximate accuracy in first order necessary conditions;
	// see Mathematical Programming, Volume 106, Number 1, 
	// Pages 25-57, Equation (6)
	options_file << "tol                        1e-9" << std::endl;

	// derivative testing
	options_file << "derivative_test            second-order" << std::endl;

	// maximum amount of random pertubation; e.g., when evaluation finite diff
	options_file << "point_perturbation_radius  0." << std::endl;

	// solve the problem
	CppAD::ipopt_solve(
		retape, xi, xu, gl, gu, fg_eval, options, result
	);
	//
 	// Check some of the result values
 	//
	ok &= result.status == ipopt_solve_status::success;
	//
	double check_x[]   = { 1.000000, 4.743000, 3.82115, 1.379408 };
	double check_z_l[] = { 1.087871, 0.,       0.,      0.       };
	double check_z_u[] = { 0.,       0.,       0.,      0.       };
	double rel_tol     = 1e-6;  // relative tolerance
	double abs_tol     = 1e-6;  // absolute tolerance
	for(j = 0; j < n; j++)
	{	ok &= CppAD::NearEqual(
			check_x[j],   result.x[j],   rel_tol, abs_tol
		);
		ok &= CppAD::NearEqual(
			check_z_l[j], result.zl[j], rel_tol, abs_tol
		);
		ok &= CppAD::NearEqual(
			check_z_u[j], result.zu[j], rel_tol, abs_tol
		);
	}

	return ok;
}

int main(void)
{	std::cout << "ok = " << ipopt_solve_get_started() << std::endl;
	return 0;
}

// END C++
