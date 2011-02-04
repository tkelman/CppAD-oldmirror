/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin user_atomic.cpp$$

$section Define User Atomic Operations: Example and Test$$

$index user_atomic, example$$
$index user_atomic, test$$
$index matrix, atomic example$$

$code
$verbatim%example/user_atomic.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>

namespace { // Empty namespace 

	using CppAD::vector;

	// number of orders for this operation (k + 1)
	size_t n_order_;
	// number of rows in the result matrix
	size_t nr_result_;
	// number of columns in left matrix and number of rows in right matrix
	size_t n_middle_;
	// number of columns in the result matrix
	size_t nc_result_;

	// index in tx of Taylor coefficient of order ell for left[i,j]
	size_t left(size_t i, size_t j, size_t ell)
	{	assert( i < nr_result_ );
		assert( j < n_middle_ );
		return (i * n_middle_ + j) * n_order_ + ell;
	}

	// index in tx of Taylor coefficient of order ell for right[i,j]
	size_t right(size_t i, size_t j, size_t ell)
	{	assert( i < n_middle_ );
		assert( j < nc_result_ );
		size_t offset = nr_result_ * n_middle_;
		return (offset + i * nc_result_ + j) * n_order_ + ell;
	}

	// index in ty of Taylor coefficinet of order ell for result[i,j]
	size_t result(size_t i, size_t j, size_t ell)
	{	assert( i < nr_result_ );
		assert( j < nc_result_ );
		return (i * nc_result_ + j) * n_order_ + ell;
	}

	// multiply left times right and sum into result
	void multiply_and_sum(
		size_t                order_left , 
		size_t                order_right, 
		const vector<double>&         tx ,
		vector<double>&               ty ) 
	{	size_t i, j;
		size_t order_result = order_left + order_right; 
		for(i = 0; i < nr_result_; i++)
		{	for(j = 0; j < nc_result_; j++)
			{	double sum = 0.;
				size_t middle, im_left, mj_right, ij_result;
				for(middle = 0; middle < n_middle_; middle++)
				{	im_left  = left(i, middle, order_left);
					mj_right = right(middle, j, order_right);
					sum     += tx[im_left] * tx[mj_right];
				}
				ij_result = result(i, j, order_result);
				ty[ ij_result ] += sum;
			}
		}
		return;
	}

	void reverse_multiply(
		size_t                order_left , 
		size_t                order_right, 
		const vector<double>&         tx ,
		const vector<double>&         ty ,
		vector<double>&               px ,
		const vector<double>          py ) 
	{	size_t i, j;
		size_t order_result = order_left + order_right; 
		for(i = 0; i < nr_result_; i++)
		{	for(j = 0; j < nc_result_; j++)
			{	size_t middle, im_left, mj_right, ij_result;
				for(middle = 0; middle < n_middle_; middle++)
				{	ij_result = result(i, j, order_result);
					im_left   = left(i, middle, order_left);
					mj_right  = right(middle, j, order_right);
					// sum       += tx[im_left]  * tx[mj_right];
					px[im_left]  += tx[mj_right] * py[ij_result];
					px[mj_right] += tx[im_left]  * py[ij_result];
				}
			}
		}
		return;
	}

	// forward mode routine
	bool forward_mat_mul(
		const vector<size_t>&  info ,
		size_t                    k ,
		size_t                    n ,
		size_t                    m ,
		const vector<bool>&      vx ,
		vector<bool>&            vy ,
		const vector<double>&    tx ,
		vector<double>&          ty
	)
	{	size_t i, j, ell;
		n_order_   = k + 1;	
		assert( info[0] == 3 ); // number of values in ainfo
		nr_result_ = info[1];   // number of rows in left matrix
		n_middle_  = info[2];   // # of columns in left and rows in right
		nc_result_ = info[3];   // # of columns in right matrix

		// check total number of components in ax and ay
		assert( nr_result_ * n_middle_ + n_middle_ * nc_result_  == n );
		assert( nr_result_ * nc_result_ == m );

		// check if we are computing vy
		if( vy.size() > 0 )
		{	assert( k == 0 && vx.size() > 0 );
			// multiply left times right
			for(i = 0; i < nr_result_; i++)
			{	for(j = 0; j < nc_result_; j++)
				{	// compute vy[ result(i, j, 0) ]
					bool   var = false;
					bool   nz_left, nz_right;
					size_t middle, im_left, mj_right, ij_result;
					for(middle = 0; middle < n_middle_; middle++)
					{	im_left  = left(i, middle, k);
						mj_right = right(middle, j, k);
						nz_left  = vx[im_left]  | (tx[im_left] != 0.);
						nz_right = vx[mj_right] | (tx[mj_right]!= 0.);
						// if not multiplying by the constant zero
						if( nz_left & nz_right )
							var |= (vx[im_left] | vx[mj_right]);
					}
					ij_result     = result(i, j, k);
					vy[ij_result] = var;
				}
			}
		}

		// initialize result as zero
		for(i = 0; i < nr_result_; i++)
		{	for(j = 0; j < nc_result_; j++)
				ty[ result(i, j, k) ] = 0.;
		}
		// sum the product of proper orders
		for(ell = 0; ell <=k; ell++)
			multiply_and_sum(ell, k-ell, tx, ty);

		// All orders are implemented and there are no possible error
		// conditions, so always return true.
		return true;
	}

	// reverse mode routine
	bool reverse_mat_mul(
		const vector<size_t>&  info ,
		size_t                    k ,
		size_t                    n ,
		size_t                    m ,
		const vector<double>&    tx ,
		const vector<double>&    ty ,
		vector<double>&          px ,
		const vector<double>&    py
	)
	{	n_order_   = k + 1;	
		assert( info[0] == 3 ); // number of values in ainfo
		nr_result_ = info[1];   // number of rows in left matrix
		n_middle_  = info[2];   // # of columns in left and rows in right
		nc_result_ = info[3];   // # of columns in right matrix

		// check total number of components in ax and ay
		assert( nr_result_ * n_middle_ + n_middle_ * nc_result_  == n );
		assert( nr_result_ * nc_result_ == m );

		size_t ell = n * n_order_;
		while(ell--)
			px[ell] = 0.;

		size_t order = n_order_;
		while(order--)
		{	// reverse sum the products for specified order
			for(ell = 0; ell <=order; ell++)
				reverse_multiply(ell, order-ell, tx, ty, px, py);
		}

		// All orders are implemented and there are no possible error
		// conditions, so always return true.
		return true;
	}

	bool for_jac_sparse_mat_mul(
		const vector<size_t>&              info ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		const vector< std::set<size_t> >&     r ,
		vector< std::set<size_t> >&           s )
	{	
		assert( info[0] == 3 ); // number of values in ainfo
		nr_result_ = info[1];   // number of rows in left matrix
		n_middle_  = info[2];   // # of columns in left and rows in right
		nc_result_ = info[3];   // # of columns in right matrix

		return false;
	}

	CPPAD_ATOMIC_FUNCTION(
		CPPAD_TEST_VECTOR       ,
		double                  , 
		mat_mul                 , 
		forward_mat_mul         , 
		reverse_mat_mul         ,
		for_jac_sparse_mat_mul 
	)

} // End empty namespace

bool user_atomic(void)
{	bool ok = true;
	using CppAD::AD;

	CPPAD_TEST_VECTOR< AD<double> > X(4), Y(4); 
	size_t i, j;
	for(j = 0; j < X.size(); j++)
		X[j] = (j + 1);
	CppAD::Independent(X);

	// matrix sizes for this multiplication
	CPPAD_TEST_VECTOR<size_t> ainfo(3);
	size_t nr_left  = ainfo[0] = 2;
	size_t n_middle = ainfo[1] = 2;
	size_t nc_right = ainfo[2] = 2;
	
	// ax and ay must use CppAD::vector
	size_t n = nr_left * n_middle + n_middle * nc_right;
	size_t m = nr_left * nc_right;
	CPPAD_TEST_VECTOR< AD<double> > ax(n), ay(m);
	// left matrix
	ax[0]  = X[0];  // left[0,0]   = x[0] = 1
	ax[1]  = X[1];  // left[0,1]   = x[1] = 2
	ax[2]  = 5.;    // left[1,0]   = 5
	ax[3]  = 6.;    // left[1,1]   = 6
	// right matrix
	ax[4]  = X[2];  // right[0,0]  = x[2] = 3
	ax[5]  = 7.;    // right[0,1]  = 7
	ax[6]  = X[3];  // right[1,0]  = x[3] = 4 
	ax[7]  = 8.;    // right[1,1]  = 8
	/*
	[ x0 , x1 ] * [ x2 , 7 ] = [ x0*x2 + x1*x3 , x0*7 + x1*8 ]
	[ 5  , 6 ]    [ x3 , 8 ]   [ 5*x2  + 6*x3  , 5*7 + 6*8 ]
	*/
	mat_mul(ainfo, ax, ay);
	//
	ok &= ay[0] == (1*3 + 2*4); ok &= Variable( ay[0] );
	ok &= ay[1] == (1*7 + 2*8); ok &= Variable( ay[1] );
	ok &= ay[2] == (5*3 + 6*4); ok &= Variable( ay[2] );
	ok &= ay[3] == (5*7 + 6*8); ok &= Parameter( ay[3] );
	//
	for(i = 0; i < Y.size(); i++)
		Y[i] = ay[i]; 
	CppAD::ADFun<double> F(X, Y);
	// f(x) = [ x0*x2 + x1*x3 , x0*7 + x1*8 , 5*x2  + 6*x3  , 5*7 + 6*8 ]^T
	//
	// Test zero order forward mode evaluation of f(x)
	CPPAD_TEST_VECTOR<double> x( X.size() ), y( Y.size() );
	for(j = 0; j < x.size(); j++)
		x[j] = j + 2;
	y = F.Forward(0, x);
	ok &= y[0] == x[0] * x[2] + x[1] * x[3];
	ok &= y[1] == x[0] * 7.   + x[1] * 8.;
	ok &= y[2] == 5. * x[2]   + 6. * x[3];
	ok &= y[3] == 5. * 7.     + 6. * 8.;

	// f'(x) = [ x2, x3, x0, x1 ]
	//         [ 7 ,  8,  0, 0  ]
	//         [ 0 ,  0,  5, 6  ]
	//         [ 0 ,  0,  0, 0  ] 
	//
	// Test first order forward mode evaluation of f'(x) * [1, 2, 3, 4]^T 
	CPPAD_TEST_VECTOR<double> dx( x.size() ), dy( y.size() );
	for(j = 0; j < x.size(); j++)
		dx[j] = j + 1;
	dy = F.Forward(1, dx);
	ok &= dy[0] == 1. * x[2] + 2. * x[3] + 3. * x[0] + 4. * x[1];
	ok &= dy[1] == 1. * 7.   + 2. * 8.   + 3. * 0.   + 4. * 0.;
	ok &= dy[2] == 1. * 0.   + 2. * 0.   + 3. * 5.   + 4. * 6.;
	ok &= dy[3] == 1. * 0.   + 2. * 0.   + 3. * 0.   + 4. * 0.;

	// f_0^2 (x) = [ 0, 0, 1, 0 ], f_0^2 (x) * [1] = [3]
	//             [ 0, 0, 0, 1 ]              [2]   [4]
	//             [ 1, 0, 0, 0 ]              [3]   [1]
	//             [ 0, 1, 0, 0 ]              [4]   [2]
	//
	// Test second order forward mode 
	CPPAD_TEST_VECTOR<double> ddx( x.size() ), ddy( y.size() );
	for(j = 0; j < x.size(); j++)
		ddx[j] = 0.;
	ddy = F.Forward(2, ddx);
	// [1, 2, 3, 4] * f_0^2 (x) * [1, 2, 3, 4]^T = 1*3 + 2*4 + 3*1 + 4*2
	ok &= 2. * ddy[0] == 1. * 3. + 2. * 4. + 3. * 1. + 4. * 2.; 
	// for i > 0, [1, 2, 3, 4] * f_i^2 (x) * [1, 2, 3, 4]^T = 0
	ok &= ddy[1] == 0.;
	ok &= ddy[2] == 0.;
	ok &= ddy[3] == 0.;

	// Test second order reverse mode 
	CPPAD_TEST_VECTOR<double> w( y.size() ), dw( 2 * x.size() );
	for(i = 0.; i < y.size(); i++)
		w[i] = 0.;
	w[0] = 1.;
	dw = F.Reverse(2, w);
	// f_0'(x) = [ x2, x3, x0, x1 ]
	ok &= dw[0*2 + 0] == x[2];
	ok &= dw[1*2 + 0] == x[3];
	ok &= dw[2*2 + 0] == x[0];
	ok &= dw[3*2 + 0] == x[1];
	// f_0'(x)   * [1, 2, 3, 4]  = 1 * x2 + 2 * x3 + 3 * x0 + 4 * x1
	// f_0^2 (x) * [1, 2, 3, 4]  = [3, 4, 1, 2]
	ok &= dw[0*2 + 1] == 3.;
	ok &= dw[1*2 + 1] == 4.;
	ok &= dw[2*2 + 1] == 1.;
	ok &= dw[3*2 + 1] == 2.;
	
	// Free temporary work space. (If there are future calls to 
	// mat_mul they would create new temporary work space.)
	CppAD::user_atomic<double>::clear();

	return ok;
}
// END PROGRAM
