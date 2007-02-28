# include <cppad/cppad.hpp>
# include <cmath>
# include "newton.hpp"

using CppAD::vector;

# define LENGTH_OF_SUMMATION 10

// A slow version of the sine function
CppAD::AD<double> fun(const CppAD::AD<double> &x)
{	CppAD::AD<double> sum = 0.;
	size_t i;
	for(i = 0; i < LENGTH_OF_SUMMATION; i++)
		sum += sin(x);

	return sum / double(LENGTH_OF_SUMMATION);
}

void test_once(CppAD::vector<double> &xout, size_t size)
{	double pi      = 4. * atan(1.); 
	double xlow    = 0.;
	double xup     = 7. * pi;
	size_t n_grid  = size;
	double epsilon = 1e-6;
	size_t max_itr = 20;

	newton_multiple(
		xout    ,
		fun     ,
		xlow    ,
		xup     ,
		n_grid  ,
		epsilon ,
		max_itr
	);
	return;
}

void test_repeat(size_t size, size_t repeat)
{	size_t i;
	CppAD::vector<double> xout;
	for(i = 0; i < repeat; i++)
		test_once(xout, size);
	return;
}

int main(void)
{
	using std::cout;
	using std::endl;
	using CppAD::vector;

	// size of the test cases
	vector<size_t> size_vec(2);
	size_vec[0] = 10;
	size_vec[1] = 20;

# ifdef _OPENMP
	cout << "OpenMP: version = "         << _OPENMP
	<< ", omp_get_max_threads() = " << omp_get_max_threads()
	          << endl;
# else
	cout << "_OPENMP is not defined" << endl;
# endif

	// solve once to check for correctness
	vector<double> xout;
	test_once(xout, size_vec[0]);
	cout << "Zeros found for sin(x) in the interval [0, 7*pi] " << endl;
	cout << xout << endl;

	// minimum time for test
	double time_min = 1.;

	// run the test cases
	vector<size_t> rate_vec( size_vec.size() );
	rate_vec = CppAD::speed_test(test_repeat, size_vec, time_min);

	// results
	cout << "size_vec = " << size_vec << endl;
	cout << "rate_vec = " << rate_vec << endl;

	return 0;
}
