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

namespace { // Begin empty namespace

using CppAD::vector;

class matrix_multiply {
private:
	// order of this operation plus one
	size_t k_plus1_;
	// number of rows in the left matrix
	size_t nr_left_;
	// number of columns (rows) in left (right) matrix
	size_t nc_left_;
	// number of columns in the right matrix
	size_t nc_right_;

	// index in tx of Taylor coefficient of order ell for left[i,j]
	size_t left(size_t i, size_t j, size_t ell)
	{	assert( i < nr_left_ );
		assert( j < nc_left_ );
		return (3 + i * nc_left_ + j) * k_plus1_ + ell;
	}
	// index in tx of Taylor coefficient of order ell for right[i,j]
	size_t right(size_t i, size_t j, size_t ell)
	{	assert( i < nc_left_ );
		assert( j < nc_right_ );
		size_t offset = 3 + nr_left_ * nc_left_;
		return (offset + i * nc_right_ + j) * k_plus1_ + ell;
	}
	// index in ty of Taylor coefficinet of order ell for result[i,j]
	size_t result(size_t i, size_t j, size_t ell)
	{	assert( i < nr_left_ );
		assert( j < nc_right_ );
		return (i * nc_right_ + j) * k_plus1_ + ell;
	}
	// multiply left times right and sum into result
	void multiply_and_sum(
		size_t                order_left , 
		size_t                order_right, 
		const vector<double>&         tx ,
		vector<double>&               ty ) 
	{	size_t i, j, l, il_left, lj_right, ij_result;
		size_t order_result = order_left + order_right; 
		for(i = 0; i < nr_left_; i++)
		{	for(j = 0; j < nc_right_; j++)
			{	double sum = 0.;
				for(l = 0; l < nc_left_; l++)
				{	il_left  = left(i, l, order_left);
					lj_right = right(l, j, order_right);
					sum     += tx[il_left] * tx[lj_right];
				}
				ij_result = result(i, j, order_result);
				ty[ ij_result ] += sum;
			}
		}
		return;
	}

public:
	// forward mode routine
	bool forward_multiply(
		size_t                 k,
		size_t                 n,
		size_t                 m,
		const vector<bool>&   vx,
		vector<bool>&         vy,
		const vector<double>& tx,
		vector<double>&       ty
	)
	{	size_t i, j, l, il_left, lj_right, ell;
		k_plus1_   = k + 1;	
		nr_left_   = size_t ( tx[0 * k_plus1_ + 0] ); // stored in ax[0]
		nc_left_   = size_t ( tx[1 * k_plus1_ + 0] ); // stored in ax[1]
		nc_right_  = size_t ( tx[2 * k_plus1_ + 0] ); // stored in ax[2]

		// check total number of components in ax
		assert( 3 + nr_left_ * nc_left_ + nc_left_ * nc_right_  == n );

		// check if we are computing vy
		if( vy.size() > 0 )
		{	assert( k == 0 && vx.size() > 0 );
			// multiply left times right
			for(i = 0; i < nr_left_; i++)
			{	for(j = 0; j < nc_right_; j++)
				{	// compute vy[ result(i, j, 0) ]
					bool   var = false;
					bool   nz_left, nz_right;
					for(l = 0; l < nc_left_; l++)
					{	il_left  = left(i, l, k);
						lj_right = right(l, j, k);
						nz_left  = vx[il_left]  | (tx[il_left] != 0.);
						nz_right = vx[lj_right] | (tx[lj_right]!= 0.);
						// if not multiplying by the constant zero
						if( nz_left & nz_right )
							var |= vx[il_left] & vx[lj_right];
					}
				}
			}
		}

		// initialize result as zero
		for(i = 0; i < nr_left_; i++)
		{	for(j = 0; j < nc_right_; j++)
				ty[ result(i, j, k) ] = 0.;
		}
		// sum the product of proper orders
		for(ell = 0; ell <=k; ell++)
			multiply_and_sum(ell, k-ell, tx, ty);
	}
};

} // End empty namespace

bool user_atomic(void)
{	return true; // not yet implemented
}
// END PROGRAM
