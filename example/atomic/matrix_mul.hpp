// $Id$
# ifndef CPPAD_MATRIX_MUL_INCLUDED
# define CPPAD_MATRIX_MUL_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin atomic_matrix_mul.hpp$$
$spell
$$

$section Matrix Multiply as an Atomic Operation$$
$index multiply, matrix atomic operation$$
$index atomic, matrix multiply operation$$
$index matrix, multiply atomic operation$$

$code
$verbatim%example/atomic/matrix_mul.hpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

namespace { // Begin empty namespace
using CppAD::vector;
//
void my_union(
	std::set<size_t>&         result  ,
	const std::set<size_t>&   left    ,
	const std::set<size_t>&   right   )
{	std::set<size_t> temp;
	std::set_union(
		left.begin()              ,
		left.end()                ,
		right.begin()             ,
		right.end()               ,
		std::inserter(temp, temp.begin())
	);
	result.swap(temp);
}
//
// matrix result = left * right 
class matrix_mul : public CppAD::atomic_base<double> {
private:
	// number of rows in left operand and in the result
	const size_t nr_result_;
	// number of columns in left operand and rows in right operand
	const size_t n_middle_;
	// number of columns in right operand and in the result
	const size_t nc_result_;
	// dimension of the domain space
	const size_t n_;
	// dimension of the range space
	const size_t m_;
public:
	// ---------------------------------------------------------------------
	// constructor
	matrix_mul(size_t nr_result, size_t n_middle, size_t nc_result)
	: atomic_base("matrix_mul"),
	nr_result_(nr_result) ,
	n_middle_(n_middle)    ,
	nc_result_(nc_result) ,
	n_( nr_result * n_middle + n_middle * nc_result ) ,
	m_( n_middle * nc_result ) 
	{ }
private:
	// left matrix index in the taylor coefficient vector tx.
	size_t left(
		size_t i  , // left matrix row index
		size_t j  , // left matrix column index
		size_t k  , // Taylor coeffocient order
		size_t nk ) // number of Taylor coefficients in tx 
	{	assert( i < nr_result_ );
		assert( j < n_middle_ );
		return (i * n_middle_ + j) * nk + k;
	}
	// right matrix index in the taylor coefficient vector tx.
	size_t right(
		size_t i  , // right matrix row index
		size_t j  , // right matrix column index
		size_t k  , // Taylor coeffocient order
		size_t nk ) // number of Taylor coefficients in tx 
	{	assert( i < n_middle_  );
		assert( j < nc_result_ );
		size_t offset = nr_result_ * n_middle_;
		return (offset + i * nc_result_ + j) * nk + k; 
	}
	// result matrix index in the taylor coefficient vector ty.
	size_t result(
		size_t i  , // result matrix row index
		size_t j  , // result matrix column index
		size_t k  , // Taylor coeffocient order
		size_t nk ) // number of Taylor coefficients in ty
	{	assert( i < nr_result_  );
		assert( j < nc_result_ );
		return (i * nc_result_ + j) * nk + k;
	}
	// Forward mode multiply Taylor coefficients in tx and sum into ty
	// (for one pair of left and right orders)
	void forward_multiply(
		size_t                 k_left  , // order for left coefficients
		size_t                 k_right , // order for right coefficients
		const vector<double>&  tx      , // domain space Taylor coefficients
		      vector<double>&  ty      ) // range space Taylor coefficients 
	{	size_t nk       = tx.size() / n_;
		assert( nk == ty.size() / m_ );
		//
		size_t k_result = k_left + k_right;
		assert( k_result < nk );
		//
		for(size_t i = 0; i < nr_result_; i++)
		{	for(size_t j = 0; j < nc_result_; j++)
			{	double sum = 0.0;
				for(size_t ell = 0; ell < n_middle_; ell++)
				{	size_t i_left  = left(i, ell, k_left, nk);
					size_t i_right = right(ell, j,  k_right, nk);
					sum           += tx[i_left] * tx[i_right];
				}
				size_t i_result = result(i, j, k_result, nk);
				ty[i_result]   += sum;
			}
		}
	}
	// Reverse mode partials of Taylor coefficients and sum into px
	// (for one pair of left and right orders)
	void reverse_multiply(
		size_t                 k_left  , // order for left coefficients
		size_t                 k_right , // order for right coefficients
		const vector<double>&  tx      , // domain space Taylor coefficients
		const vector<double>&  ty      , // range space Taylor coefficients 
		      vector<double>&  px      , // partials w.r.t. tx
		const vector<double>&  py      ) // partials w.r.t. ty 
	{	size_t nk       = tx.size() / n_;
		assert( nk == ty.size() / m_ );
		assert( tx.size() == px.size() );
		assert( ty.size() == py.size() );
		//
		size_t k_result = k_left + k_right;
		assert( k_result < nk );
		//
		for(size_t i = 0; i < nr_result_; i++)
		{	for(size_t j = 0; j < nc_result_; j++)
			{	size_t i_result = result(i, j, k_result, nk);
				for(size_t ell = 0; ell < n_middle_; ell++)
				{	size_t i_left  = left(i, ell, k_left, nk);
					size_t i_right = right(ell, j,  k_right, nk);
					// sum        += tx[i_left] * tx[i_right];
					px[i_left]    += tx[i_right] * py[i_result];
					px[i_right]   += tx[i_left]  * py[i_result];
				}
			}
		}
		return;
	}
	// ----------------------------------------------------------------------
	// forward mode routine called by CppAD
	bool forward(
		size_t                    q ,
		size_t                    p ,
		const vector<bool>&      vx ,
		      vector<bool>&      vy ,
		const vector<double>&    tx ,
		      vector<double>&    ty
	)
	{	size_t p1 = p + 1;
		assert( vx.size() == 0 || n_ == vx.size() );
		assert( vx.size() == 0 || m_ == vy.size() );
		assert( n_ * p1 == tx.size() );
		assert( m_ * p1 == ty.size() );
		size_t i, j, ell;

		// check if we are computing vy information
		if( vx.size() > 0 )
		{	size_t nk = 1;
			size_t k  = 0;
			for(i = 0; i < nr_result_; i++)
			{	for(j = 0; j < nc_result_; j++)
				{	bool var = false;
					for(ell = 0; ell < n_middle_; ell++)
					{	size_t i_left  = left(i, ell, k, nk);
						size_t i_right = right(ell, j, k, nk);
						bool   nz_left = vx[i_left] |(tx[i_left]  != 0.);
						bool  nz_right = vx[i_right]|(tx[i_right] != 0.);
						// if not multiplying by the constant zero
						if( nz_left & nz_right )
								var |= vx[i_left] | vx[i_right];
					}
					size_t i_result = result(i, j, k, nk);
					vy[i_result] = var;
				}
			}
		}

		// initialize result as zero
		size_t k;
		for(i = 0; i < nr_result_; i++)
		{	for(j = 0; j < nc_result_; j++)
			{	for(k = q; k <= p; k++)
					ty[ result(i, j, k, p1) ] = 0.0;
			}
		}
		for(k = q; k <= p; k++)
		{	// sum the produces that result in order k
			for(ell = 0; ell <= k; ell++)
				forward_multiply(ell, k - ell, tx, ty);
		}

		// all orders are implented, so always return true
		return true;
	}
	// ----------------------------------------------------------------------
	// reverse mode routine called by CppAD
	virtual bool reverse(
		size_t                     p ,
		const vector<double>&     tx ,
		const vector<double>&     ty ,
		      vector<double>&     px ,
		const vector<double>&     py
	)
	{	size_t p1 = p + 1;
		assert( n_ * p1 == tx.size() );
		assert( m_ * p1 == ty.size() );
		assert( px.size() == tx.size() );
		assert( py.size() == ty.size() );

		// initialize summation
		for(size_t i = 0; i < px.size(); i++)
			px[i] = 0.0;

		// number of orders to differentiate
		size_t k = p1;
		while(k--)
		{	// differentiate the produces that result in order k
			for(size_t ell = 0; ell <= k; ell++)
				reverse_multiply(ell, k - ell, tx, ty, px, py);
		}

		// all orders are implented, so always return true
		return true;
	}
	// ----------------------------------------------------------------------
	// forward Jacobian sparsity routine called by CppAD
	virtual bool for_sparse_jac(
		size_t                                q ,
		const vector<bool>&                   r ,
		      vector<bool>&                   s )
	{	assert( n_ * q == r.size() );
		assert( m_ * q == s.size() );
		size_t p;

		// sparsity for S(x) = f'(x) * R
		size_t nk = 1;
		size_t k  = 0;
		for(size_t i = 0; i < nr_result_; i++)
		{	for(size_t j = 0; j < nc_result_; j++)
			{	size_t i_result = result(i, j, k, nk);
				for(p = 0; p < q; p++)
					s[i_result * q + p] = false; 
				for(size_t ell = 0; ell < n_middle_; ell++)
				{	size_t i_left  = left(i, ell, k, nk);
					size_t i_right = right(ell, j, k, nk);
					for(p = 0; p < q; p++)
					{	s[i_result * q + p] |= r[i_left * q + p ]; 
						s[i_result * q + p] |= r[i_right * q + p ]; 
					}
				}
			}
		}
		return true;
	}
	virtual bool for_sparse_jac(
		size_t                                q ,
		const vector< std::set<size_t> >&     r ,
		      vector< std::set<size_t> >&     s )
	{	assert( n_ == r.size() );
		assert( m_ == s.size() );

		// sparsity for S(x) = f'(x) * R
		size_t nk = 1;
		size_t k  = 0;
		for(size_t i = 0; i < nr_result_; i++)
		{	for(size_t j = 0; j < nc_result_; j++)
			{	size_t i_result = result(i, j, k, nk);
				s[i_result].clear();
				for(size_t ell = 0; ell < n_middle_; ell++)
				{	size_t i_left  = left(i, ell, k, nk);
					size_t i_right = right(ell, j, k, nk);
					//
					my_union( s[i_result], s[i_result], r[i_left] );
					my_union( s[i_result], s[i_result], r[i_right] );
				}
			}
		}
		return true;
	}
	// ----------------------------------------------------------------------
	// reverse Jacobian sparsity routine called by CppAD
	virtual bool rev_sparse_jac(
		size_t                                q ,
		const vector<bool>&                  rt ,
		      vector<bool>&                  st )
	{	assert( n_ * q == st.size() );
		assert( m_ * q == rt.size() );
		size_t i, j, p;

		// initialize
		for(i = 0; i < n_; i++)
		{	for(p = 0; p < q; p++)
				st[ i * q + p ] = false;
		}

		// sparsity for S(x)^T = f'(x)^T * R^T
		size_t nk = 1;
		size_t k  = 0;
		for(i = 0; i < nr_result_; i++)
		{	for(j = 0; j < nc_result_; j++)
			{	size_t i_result = result(i, j, k, nk);
				for(size_t ell = 0; ell < n_middle_; ell++)
				{	size_t i_left  = left(i, ell, k, nk);
					size_t i_right = right(ell, j, k, nk);
					for(p = 0; p < q; p++)
					{	st[i_left * q + p] |= rt[i_result * q + p];
						st[i_right* q + p] |= rt[i_result * q + p];
					}
				}
			}
		}
		return true;
	}
	virtual bool rev_sparse_jac(
		size_t                                q ,
		const vector< std::set<size_t> >&    rt ,
		      vector< std::set<size_t> >&    st )
	{	assert( n_ == st.size() );
		assert( m_ == rt.size() );
		size_t i, j;

		// initialize
		for(i = 0; i < n_; i++)
			st[i].clear();

		// sparsity for S(x)^T = f'(x)^T * R^T
		size_t nk = 1;
		size_t k  = 0;
		for(i = 0; i < nr_result_; i++)
		{	for(j = 0; j < nc_result_; j++)
			{	size_t i_result = result(i, j, k, nk);
				for(size_t ell = 0; ell < n_middle_; ell++)
				{	size_t i_left  = left(i, ell, k, nk);
					size_t i_right = right(ell, j, k, nk);
					//
					my_union(st[i_left],  st[i_left],  rt[i_result]);
					my_union(st[i_right], st[i_right], rt[i_result]);
				}
			}
		}
		return true;

	}

};

} // End empty namespace
// END C++

# endif
