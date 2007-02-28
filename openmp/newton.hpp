# include <cppad/cppad.hpp>
# include <cassert>

# ifdef _OPENMP
# include <omp.h>
# endif

template <class Fun>
void newton_one(double &fcur, double &xcur, Fun *fun, 
	double xlow, double xin, double xup, double epsilon, size_t max_itr)
{	using CppAD::AD;
	using CppAD::vector;
	using CppAD::abs;

	size_t itr;
	xcur = xin;
	for(itr = 0; itr < max_itr; itr++)
	{	// domain space vector
		size_t n = 1;
		vector< AD<double> > X(n);
		X[0] = xcur;
		CppAD::Independent(X);
		// range space vector
		size_t m = 1;
		vector< AD<double> > Y(m);
		Y[0] = fun(X[0]);
		// F : X -> Y
		CppAD::ADFun<double> F(X, Y);
		// fcur = F(xcur)
		fcur  = Value(Y[0]);
		// evaluate dfcur = F'(xcur)
		vector<double> dx(n), dy(m);
		dx[0] = 1;
		dy = F.Forward(1, dx);
		double dfcur = dy[0];
		// check end of iterations
		if( abs(fcur) <= epsilon )
			return;
		if( (xcur == xlow) & (fcur * dfcur > 0.) )
			return; 
		if( (xcur == xup)  & (fcur * dfcur < 0.) )
			return; 
		// next Newton iterate
		if( dfcur * (xcur - xlow) <= fcur )
			xcur = xlow;
		else if( dfcur * (xcur - xup) >= fcur )
			xcur = xup;
		else	xcur = xcur - fcur / dfcur;
	}
	return;
}

template <class Fun>
void newton_multiple(
	CppAD::vector<double> &xout , 
	Fun *fun                    , 
	double xlow                 , 
	double xup                  , 
	size_t n_grid               , 
	double epsilon              , 
	size_t max_itr              )
{	using CppAD::AD;
	using CppAD::vector;
	using CppAD::abs;

	// OpenMP uses integers in place of size_t
	int i, n = int(n_grid);

# if 0
	// for version that used tape_max_active
	i = omp_get_max_threads();
	assert( i > 0 );
	CppAD::AD<double>::tape_max_active(size_t(i));
# endif

	// set up grid
	vector<double> grid(n_grid + 1);
	vector<double> fcur(n_grid), xcur(n_grid), xmid(n_grid);
	size_t i_grid;
	double dx = (xup - xlow) / double(n_grid);
	for(i_grid = 0; i_grid < n_grid; i_grid++)
	{	grid[i_grid] = xlow + i_grid * dx;
		xmid[i_grid] = xlow + (i_grid + .5) * dx;
	}
	grid[n_grid] = xup;

# pragma omp parallel for 
	for(i = 0; i < n; i++) 
	{	newton_one(
			fcur[i]   ,
			xcur[i]   ,
			fun       , 
			grid[i]   , 
			xmid[i]   , 
			grid[i+1] , 
			epsilon   , 
			max_itr
		);
	}
// end omp parallel for

	// remove duplicates and points that are not solutions
	double xlast  = xlow - fabs(xlow) - 1.;
	size_t n_zero = 0;
	for(i_grid = 0; i_grid < n_grid; i_grid++)
	{
		if( abs( fcur[i_grid] ) <= epsilon && xcur[i_grid] != xlast )
			xcur[n_zero++] = xlast = xcur[i_grid];
	}

	// resize output vector and set its values
	xout.resize(n_zero);
	for(i = 0; i < n_zero; i++)
		xout[i] = xcur[i];

	return;
}
