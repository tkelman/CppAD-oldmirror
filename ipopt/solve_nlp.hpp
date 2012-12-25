/* $Id:$ */
# ifndef CPPAD_SOLVE_NLP_INCLUDED
# define CPPAD_SOLVE_NLP_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

CPPAD_BEGIN_NAMESPACE
namespace ipopt {
/*
\defgroup solve_nlp solve_nlp.hpp
\{
\file solve_nlp.hpp
\brief Class that connects ipopt::solve to Ipopt
*/

/*!
Class that contains information about solve problem result

\tparam Dvector
a simple vector with elements of type double
*/
template <class Dvector>
class solve_result 
{	
public:
	/// possible values for the result status
	enum status_type {
		not_defined,
		success,
		maxiter_exceeded,
		stop_at_tiny_step,
		stop_at_acceptable_point,
		local_infeasibility,
		user_requested_stop,
		feasible_point_found,
		diverging_iterates,
		restoration_failure,
		error_in_step_computation,
		invalid_number_detected,
		too_few_degrees_of_freedom,
		internal_error,
		unknown
	};

	/// possible values for solution status
	status_type status;
	/// the approximation solution
	Dvector x;
	/// Lagrange multipliers corresponding to lower bounds on x
	Dvector zl;
	/// Lagrange multipliers corresponding to upper bounds on x
	Dvector zu;
	/// value of g(x)
	Dvector g;
	/// Lagrange multipliers correspondiing constraints on g(x)
	Dvector lambda;
	/// value of f(x)
	double obj_value;
	/// constructor initializes solution status as not yet defined
	solve_result(void)
	{	status = not_defined; }
};

/*!
Class that Ipopt uses for obtaining information about this problem.


\section Evaluation_Methods Evaluation Methods
The set of evaluation methods for this class is
\verbatim
	{ eval_f, eval_grad_f, eval_g, eval_jac_g, eval_h }
\endverbatim
Note that the bool return flag for the evaluations methods 
does not appear in the Ipopt documentation.
Looking at the code, it seems to be a flag telling Ipopt to abort
when the flag is false.
*/
template <class Dvector, class ADvector, class FG_eval>
class solve_nlp : public Ipopt::TNLP
{
private:
	// ------------------------------------------------------------------
 	// Types used by this class
	// ------------------------------------------------------------------
	/// A Scalar value used by Ipopt
	typedef Ipopt::Number                         Number;
	/// An index value used by Ipopt 
	typedef Ipopt::Index                          Index;
	/// Indexing style used in Ipopt sparsity structure 
	typedef Ipopt::TNLP::IndexStyleEnum           IndexStyleEnum;
	// ------------------------------------------------------------------
 	// Values directly passed in to constuctor
	// ------------------------------------------------------------------
	/// dimension of the range space for f(x) 
	const size_t                    nf_;
	/// dimension of the domain space for f(x) and g(x)
	const size_t                    nx_;
	/// dimension of the range space for g(x)
	const size_t                    ng_;
	/// initial value for x
	const Dvector&                  xi_;
	/// lower limit for x 
	const Dvector&                  xl_;
	/// upper limit for x 
	const Dvector&                  xu_;
	/// lower limit for g(x)
	const Dvector&                  gl_;
	/// upper limit for g(x)
	const Dvector&                  gu_;
	/// AD function object that evaluates x -> [ f(x) , g(x) ]
	CppAD::ADFun<double>            fg_ad_eval_;
	/// place where final results are placed
	solve_result<Dvector>&          solution_;
public:
	// ----------------------------------------------------------------------
	/*! 
	Constructor for the interface between ipopt::solve and Ipopt
	
	\param nf
	dimension of the range space for f(x)
	
	\param nx
	dimension of the domain space for f(x) and g(x).
	
	\param ng
	dimension of the range space for g(x)
	
	\param xi
	initial value of x during the optimization procedure (size nx).
	
	\param xl
	lower limit for x (size nx).
	
	\param xu
	upper limit for x (size nx).
	
	\param gl
	lower limit for g(x) (size ng).
	
	\param gu
	upper limit for g(x) (size ng).

	\param fg_eval
	function object that evaluations f(x) and g(x) using fg_eval(fg, x)
	
	\param solution
	object where final results are stored.
	*/
	solve_nlp(
		size_t                 nf       ,
		size_t                 nx       , 
		size_t                 ng       ,
		const Dvector&         xi       ,
		const Dvector&         xl       ,
		const Dvector&         xu       ,
		const Dvector&         gl       ,
		const Dvector&         gu       ,
		FG_eval&               fg_eval  ,
		solve_result<Dvector>& solution ) : 
	nf_ ( nf ),
	nx_ ( nx ),
	ng_ ( ng ),
	xi_ ( xi ),
	xl_ ( xl ),
	xu_ ( xu ),
	gl_ ( gl ),
	gu_ ( gu ),
	solution_ ( solution )
	{	size_t i;
		// make fg_ad_eval correspond to x -> [ f(x), g(x) ]
		ADvector x(nx_), fg(nf_ + ng_);
		for(i = 0; i < nx_; i++)
			x[i] = xi_[i];
		CppAD::Independent(x);
		fg_eval(fg, x);
		fg_ad_eval_.Dependent(x, fg);
	}
	// -----------------------------------------------------------------------
	/*!
	Return dimension information about optimization problem.
	
	\param[out] n
	is set to the value nx_.
	
	\param[out] m
	is set to the value ng_.
	
	\param[out] nnz_jac_g
	is set to ng_ * nx_ (sparsity not yet implemented) 
	
	\param[out] nnz_h_lag
	is set to nx_ * nx_ (sparsity not yet implemented)
	
	\param[out] index_style
	is set to C_STYLE; i.e., zeoro based indexing is used in the
	information passed to Ipopt.
	*/
	virtual bool get_nlp_info(
		Index&          n            , 
		Index&          m            , 
		Index&          nnz_jac_g    ,
		Index&          nnz_h_lag    , 
		IndexStyleEnum& index_style  )
	{
		n         = static_cast<Index>(nx_);
		m         = static_cast<Index>(ng_);
		nnz_jac_g = static_cast<Index>(ng_ * nx_);
		nnz_h_lag = static_cast<Index>(nx_ * nx_);
	
	  	// use the fortran index style for row/col entries
		index_style = C_STYLE;
	
		return true;
	}
	// -----------------------------------------------------------------------
	/*!
	Return bound information about optimization problem.
	
	\param[in] n
	is the dimension of the domain space for f(x) and g(x); i.e.,
	it must be equal to nx_.
	
	\param[out] x_l
	is a vector of size nx_.
	The input value of its elements does not matter.
	On output, it is a copy of the lower bound for \f$ x \f$; i.e.,
	xl_.
	
	\param[out] x_u
	is a vector of size nx_.
	The input value of its elements does not matter.
	On output, it is a copy of the upper bound for \f$ x \f$; i.e.,
	xu_.
	
	\param[in] m
	is the dimension of the range space for g(x). i.e.,
	it must be equal to ng_.
	
	\param[out] g_l
	is a vector of size ng_.
	The input value of its elements does not matter.
	On output, it is a copy of the lower bound for \f$ g(x) \f$; i.e., gl_.
	
	\param[out] g_u
	is a vector of size ng_.
	The input value of its elements does not matter.
	On output, it is a copy of the upper bound for \f$ g(x) \f$; i.e, gu_.
	*/
	virtual bool get_bounds_info(
		Index       n        , 
		Number*     x_l      , 
		Number*     x_u      ,
		Index       m        , 
		Number*     g_l      , 
		Number*     g_u      )
	{	size_t i;
		// here, the n and m we gave IPOPT in get_nlp_info are passed back 
		CPPAD_ASSERT_UNKNOWN(static_cast<size_t>(n) == nx_);
		CPPAD_ASSERT_UNKNOWN(static_cast<size_t>(m) == ng_);
	
		// pass back bounds
		for(i = 0; i < nx_; i++)
		{	x_l[i] = xl_[i];
			x_u[i] = xu_[i];
		}
		for(i = 0; i < ng_; i++)
		{	g_l[i] = gl_[i];
			g_u[i] = gu_[i];
		}
		
		return true;
	}
	// -----------------------------------------------------------------------
	/*!
	Return initial x value where optimiation is started.
	
	\param[in] n
	must be equal to the domain dimension for f(x) and g(x); i.e.,
	it must be equal to nx_.
	
	\param[in] init_x
	must be equal to true.
	
	\param[out] x
	is a vector of size nx_.
	The input value of its elements does not matter.
	On output, it is a copy of the initial value for \f$ x \f$; i.e. xi_.
	
	\param[in] init_z
	must be equal to false.
	
	\param z_L
	is not used.
	
	\param z_U
	is not used.
	
	\param[in] m
	must be equal to the domain dimension for f(x) and g(x); i.e.,
	it must be equal to ng_.
	
	\param init_lambda
	must be equal to false.
	
	\param lambda
	is not used.
	*/
	virtual bool get_starting_point(
		Index           n            , 
		bool            init_x       , 
		Number*         x            ,
		bool            init_z       , 
		Number*         z_L          , 
		Number*         z_U          ,
		Index           m            , 
		bool            init_lambda  ,
		Number*         lambda       )
	{	size_t j;
	
		CPPAD_ASSERT_UNKNOWN(static_cast<size_t>(n) == nx_ );
		CPPAD_ASSERT_UNKNOWN(static_cast<size_t>(m) == ng_ );
		CPPAD_ASSERT_UNKNOWN(init_x == true);
		CPPAD_ASSERT_UNKNOWN(init_z == false);
		CPPAD_ASSERT_UNKNOWN(init_lambda == false);
	
		for(j = 0; j < nx_; j++)
			x[j] = xi_[j];
	
		return true;
	}
	// -----------------------------------------------------------------------
	/*!
	Evaluate the objective fucntion f(x).
	
	\param[in] n
	is the dimension of the argument space for f(x); i.e., must be equal nx_.
	
	\param[in] x
	is a vector of size nx_ containing the point at which to evaluate
	the function sum_i f_i (x).
	
	\param[in] new_x
	is false if the previous call to any one of the 
	\ref Evaluation_Methods used the same value for x.
	
	\param[out] obj_value
	is the value of the objective sum_i f_i (x) at this value of x.
	
	\return
	The return value is always true; see \ref Evaluation_Methods.
	*/
	virtual bool eval_f(
		Index          n           , 
		const Number*  x           , 
		bool           new_x       , 
		Number&        obj_value   )
	{	size_t i;
		Dvector x0(nx_), fg0(nf_ + ng_);
		for(i = 0; i < nx_; i++)
			x0[i] = x[i];
		fg0 = fg_ad_eval_.Forward(0, x0);
		double sum = 0.0;
		for(i = 0; i < nf_; i++)
			sum += fg0[i];
		//
		obj_value = static_cast<Number>(sum);
	}
	// -----------------------------------------------------------------------
	/*!
	Evaluate the gradient of f(x).
	
	\param[in] n
	is the dimension of the argument space for f(x); i.e., must be equal nx_.
	
	\param[in] x
	has a vector of size nx_ containing the point at which to evaluate
	the gradient of f(x).
	
	\param[in] new_x
	is false if the previous call to any one of the 
	\ref Evaluation_Methods used the same value for x.
	
	\param[out] grad_f
	is a vector of size nx_.
	The input value of its elements does not matter.
	The output value of its elements is the gradient of f(x) 
	at this value of.
	
	\return
	The return value is always true; see \ref Evaluation_Methods.
	*/
	virtual bool eval_grad_f(
		Index           n        , 
		const Number*   x        , 
		bool            new_x    , 
		Number*         grad_f   )
	{	size_t i;
		if( new_x )
		{	Dvector x0(nx_), fg0(nf_ + ng_);
			for(i = 0; i < nx_; i++)
				x0[i] = x[i];
			fg_ad_eval_.Forward(0, x0);
		}
		Dvector w(nf_ + ng_), dw(nx_);
		for(i = 0; i < nf_; i++)
			w[i] = 1.0;
		for(i = 0; i < ng_; i++)
			w[nf_ + i] = 0.0;
		dw = fg_ad_eval_.Reverse(1, w);
		for(i = 0; i < nx_; i++)
			grad_f[i] = dw[i];
	}
	// -----------------------------------------------------------------------
	/*!
	Evaluate the function g(x).
	
	\param[in] n
	is the dimension of the argument space for g(x); i.e., must be equal nx_.
	
	\param[in] x
	has a vector of size n containing the point at which to evaluate
	the gradient of g(x).
	
	\param[in] new_x
	is false if the previous call to any one of the 
	\ref Evaluation_Methods used the same value for x.
	
	\param[in] m
	is the dimension of the range space for g(x); i.e., must be equal to ng_.
	
	\param[out] g
	is a vector of size ng_.
	The input value of its elements does not matter.
	The output value of its elements is 
	the value of the function g(x) at this value of x.
	
	\return
	The return value is always true; see \ref Evaluation_Methods.
	*/
	virtual bool eval_g(
		Index   n            , 
		const   Number* x    , 
		bool    new_x        , 
		Index   m            , 
		Number* g            )
	{	size_t i;
		Dvector x0(nx_), fg0(nf_ + ng_);
		for(i = 0; i < nx_; i++)
			x0[i] = x[i];
		fg0 = fg_ad_eval_.Forward(0, x0);
		for(i = 0; i < ng_; i++)
			g[i] = fg0[nf_ + i];
	}
	// -----------------------------------------------------------------------
	/*!
	Evaluate the Jacobian of g(x).
	
	\param[in] n
	is the dimension of the argument space for g(x); 
	i.e., must be equal nx_.
	
	\param x
	If values is not NULL,
	x is a vector of size nx_ containing the point at which to evaluate
	the gradient of g(x).
	
	\param[in] new_x
	is false if the previous call to any one of the 
	\ref Evaluation_Methods used the same value for  x.
	
	\param[in] m
	is the dimension of the range space for g(x); 
	i.e., must be equal to ng_.
	
	\param[in] nele_jac
	is the number of possibly non-zero elements in the Jacobian of g(x);
	i.e., must be equal to ng_ * nx_.
	
	\param iRow
	if values is not NULL, iRow is not defined.
	if values is NULL, iRow
	is a vector with size nele_jac.
	The input value of its elements does not matter.
	On output, 
	For <tt>k = 0 , ... , nele_jac-1, iRow[k]</tt> is the 
	base zero row index for the 
	k-th possibly non-zero entry in the Jacobian of g(x).
	
	\param jCol
	if values is not NULL, jCol is not defined.
	if values is NULL, jCol
	is a vector with size nele_jac.
	The input value of its elements does not matter.
	On output, 
	For <tt>k = 0 , ... , nele_jac-1, jCol[k]</tt> is the 
	base zero column index for the 
	k-th possibly non-zero entry in the Jacobian of g(x).
	
	\param values
	if \c values is not \c NULL, \c values
	is a vector with size \c nele_jac.
	The input value of its elements does not matter.
	On output, 
	For <tt>k = 0 , ... , nele_jac-1, values[k]</tt> is the 
	value for the 
	k-th possibly non-zero entry in the Jacobian of g(x).
	
	\return
	The return value is always true; see \ref Evaluation_Methods.
	*/
	virtual bool eval_jac_g(
		Index n, 
		const Number* x, 
		bool new_x,
                       
		Index m, 
		Index nele_jac, 
		Index* iRow, 
		Index *jCol,
                       
		Number* values)
	{	size_t i, j, k;
		CPPAD_ASSERT_UNKNOWN(static_cast<size_t>(m)         == ng_ );
		CPPAD_ASSERT_UNKNOWN(static_cast<size_t>(n)         == nx_ );
		CPPAD_ASSERT_UNKNOWN(static_cast<size_t>(nele_jac)  == ng_ * nx_ );

		if( values == NULL )
		{	for(i = 0; i < nx_; i++)
			{	for(j = 0; j < nx_; j++)
				{	k       = i * nx_ + j;
					iRow[k] = i;
					jCol[k] = j;
				}
			}
			return true;
		}
		if( new_x )
		{	Dvector x0(nx_), fg0(nf_ + ng_);
			for(i = 0; i < nx_; i++)
				x0[i] = x[i];
			fg_ad_eval_.Forward(0, x0);
		}
		if( nx_ < ng_ )
		{	// user forward mode
			Dvector x1(nx_), fg1(nf_ + ng_);
			for(j = 0; j < nx_; j++)
				x1[j] = 0.0;
			for(j = 0; j < nx_; j++)
			{	// compute j-th column of Jacobian of g(x)
				x1[j] = 1.0;
				fg1 = fg_ad_eval_.Forward(1, x1);
				for(i = 0; i < ng_; i++)
				{	k = i * nx_ + j;
					values[k] = fg1[nf_ + i];
				}
				x1[j] = 0.0;
			}
		}
		if( nx_ > ng_ )
		{	// user reverse mode
			Dvector w(nf_ + ng_), dw(nx_);
			for(i = 0; i < nf_ + ng_; i++)
				w[i] = 0.0;
			for(i = 0; i < ng_; i++)
			{	// compute i-th row of Jacobian of g(x)
				w[i + nf_] = 1.0;
				dw = fg_ad_eval_.Reverse(1, w);
				for(j = 0; j < nx_; j++)
				{	k = i * nx_ + j;
					values[k] = dw[j];
				}
				w[i + nf_] = 0.0;
			}
		}
		return true;
	}
	// -----------------------------------------------------------------------
	/*!
	Evaluate the Hessian of the Lagragian
	
	\section The_Hessian_of_the_Lagragian The Hessian of the Lagragian
	The Hessian of the Lagragian is defined as
	\f[
	H(x, \sigma, \lambda ) 
	=
	\sigma \nabla^2 f(x) + \sum_{i=0}^{m-1} \lambda_i \nabla^2 g(x)_i
	\f]
	
	\param[in] n
	is the dimension of the argument space for g(x); 
	i.e., must be equal nx_.
	
	\param x
	if values is not NULL, x
	is a vector of size nx_ containing the point at which to evaluate
	the Hessian of the Lagragian.
	
	\param[in] new_x
	is false if the previous call to any one of the 
	\ref Evaluation_Methods used the same value for x.
	
	\param[in] obj_factor
	the value \f$ \sigma \f$ multiplying the Hessian of
	f(x) in the expression for \ref The_Hessian_of_the_Lagragian.
	
	\param[in] m
	is the dimension of the range space for g(x); 
	i.e., must be equal to ng_.
	
	\param[in] lambda
	if values is not NULL, lambda
	is a vector of size ng_ specifing the value of \f$ \lambda \f$
	in the expression for \ref The_Hessian_of_the_Lagragian.
	
	\param[in] new_lambda
	is true if the previous call to eval_h had the same value for
	lambda and false otherwise.
	(Not currently used.)
	
	\param[in] nele_hess
	is the number of possibly non-zero elements in the 
	Hessian of the Lagragian;
	i.e., must be equal to nx_ * nx_.
	
	\param iRow
	if values is not NULL, iRow is not defined.
	if values is NULL, iRow
	is a vector with size nele_jac.
	The input value of its elements does not matter.
	On output, 
	For <tt>k = 0 , ... , nele_jac-1, iRow[k]</tt> is the 
	base zero row index for the 
	k-th possibly non-zero entry in the Hessian fo the Lagragian.
	
	\param jCol
	if values is not NULL, jCol is not defined.
	if values is NULL, jCol
	is a vector with size nele_jac.
	The input value of its elements does not matter.
	On output, 
	For <tt>k = 0 , ... , nele_jac-1, jCol[k]</tt> is the 
	base zero column index for the 
	k-th possibly non-zero entry in the Hessian of the Lagragian.
	
	\param values
	if values is not NULL, it
	is a vector with size nele_jac.
	The input value of its elements does not matter.
	On output, 
	For <tt>k = 0 , ... , nele_jac-1, values[k]</tt> is the 
	value for the 
	k-th possibly non-zero entry in the Hessian of the Lagragian.
	
	\return
	The return value is always true; see \ref Evaluation_Methods.
	*/
	virtual bool eval_h(
		Index         n              ,
		const Number* x              , 
		bool          new_x          ,
		Number        obj_factor     , 
		Index         m              ,
		const Number* lambda         ,
		bool          new_lambda     , 
		Index         nele_hess      , 
		Index*        iRow           ,
		Index*        jCol           , 
		Number*       values         )
	{	size_t i, j, k;
		CPPAD_ASSERT_UNKNOWN(static_cast<size_t>(m) == ng_ );
		CPPAD_ASSERT_UNKNOWN(static_cast<size_t>(n) == nx_ );
		CPPAD_ASSERT_UNKNOWN(static_cast<size_t>(nele_hess) == nx_ * nx_ );

		if( values == NULL )
		{	// The Hessian is symmetric, only fill the lower left triangle
			k = 0;
			for(i = 0; i < nx_; i++)
			{	for(j = 0; j <= i; j++)
				{	iRow[k] = i;
					jCol[k] = j;
					k++;
				}
			}
			return true;
		}

		Dvector w(nf_ + ng_), hes(nx_ * nx_);
		for(i = 0; i < nf_; i++)
			w[i] = obj_factor;
		for(i = 0; i < ng_; i++)
			w[i + nf_] = lambda[i];
		hes = fg_ad_eval_.Hessian(x, w);
		k   = 0;
		for(i = 0; i < nx_; i++)
		{	for(j = 0; j <= i; j++)
				values[k++] = hes[i * nx_ + j];
		}
		return true;
	}
	// ----------------------------------------------------------------------
	/*!
	Pass solution information from Ipopt to users solution structure.
	
	\param[in] status
	is value that the Ipopt solution status
	which gets mapped to a correponding value for 
	\n
	<tt>solution_.status</tt>
	
	\param[in] n
	is the dimension of the domain space for f(x) and g(x); i.e.,
	it must be equal to nx_.
	
	\param[in] x
	is a vector with size nx_ specifing the final solution.
	This is the output value for
	\n
	<tt>solution_.x</tt> 
	
	\param[in] z_L
	is a vector with size nx_ specifing the Lagragian multipliers for the
	constraint \f$ x^l \leq x \f$.
	This is the output value for
	\n
	<tt>solution_.zl</tt>
	
	\param[in] z_U
	is a vector with size nx_ specifing the Lagragian multipliers for the
	constraint \f$ x \leq x^u \f$.
	This is the output value for
	\n
	<tt>solution_.zu</tt> 
	
	\param[in] m
	is the dimension of the range space for g(x). i.e.,
	it must be equal to ng_.
	
	\param[in] g
	is a vector with size ng_ containing the value of the constraint function
	g(x) at the final solution x.
	This is the output value for
	\n
	<tt>solution_.g</tt> 
	
	\param[in] lambda
	is a vector with size ng_ specifing the Lagragian multipliers for the
	constraints \f$ g^l \leq g(x) \leq g^u \f$.
	This is the output value for
	\n
	<tt>solution_.lambda</tt> 
	
	\param[in] obj_value
	is the value of the objective function f(x) at the final solution x.
	This is the output value for
	\n
	<tt>solution_.obj_value</tt>
	
	\param[in] ip_data
	is unspecified (by Ipopt) and hence not used.
	
	\param[in] ip_cq
	is unspecified (by Ipopt) and hence not used.
	
	\par solution_[out]
	this is a reference to the solution argument
	in the constructor for solve_nlp.
	The results are stored here
	(see documentation above).
	*/
	virtual void finalize_solution(
		Ipopt::SolverReturn               status    ,
		Index                             n         , 
		const Number*                     x         , 
		const Number*                     z_L       , 
		const Number*                     z_U       ,
		Index                             m         , 
		const Number*                     g         , 
		const Number*                     lambda    ,
		Number                            obj_value ,
		const Ipopt::IpoptData*           ip_data   ,
		Ipopt::IpoptCalculatedQuantities* ip_cq
	)
	{	size_t i, j;
	
		CPPAD_ASSERT_UNKNOWN(static_cast<size_t>(n) == nx_ );
		CPPAD_ASSERT_UNKNOWN(static_cast<size_t>(m) == ng_ );
	
		switch(status)
		{	// convert status from Ipopt enum to solve_result<Dvector> enum
			case Ipopt::SUCCESS:
			solution_.status = solve_result<Dvector>::success;
			break;
	
			case Ipopt::MAXITER_EXCEEDED:
			solution_.status = 
				solve_result<Dvector>::maxiter_exceeded;
			break;
	
			case Ipopt::STOP_AT_TINY_STEP:
			solution_.status = 
				solve_result<Dvector>::stop_at_tiny_step;
			break;
	
			case Ipopt::STOP_AT_ACCEPTABLE_POINT:
			solution_.status = 
				solve_result<Dvector>::stop_at_acceptable_point;
			break;
	
			case Ipopt::LOCAL_INFEASIBILITY:
			solution_.status = 
				solve_result<Dvector>::local_infeasibility;
			break;
	
			case Ipopt::USER_REQUESTED_STOP:
			solution_.status = 
				solve_result<Dvector>::user_requested_stop;
			break;
	
			case Ipopt::DIVERGING_ITERATES:
			solution_.status = 
				solve_result<Dvector>::diverging_iterates;
			break;
	
			case Ipopt::RESTORATION_FAILURE:
			solution_.status = 
				solve_result<Dvector>::restoration_failure;
			break;
	
			case Ipopt::ERROR_IN_STEP_COMPUTATION:
			solution_.status = 
				solve_result<Dvector>::error_in_step_computation;
			break;
	
			case Ipopt::INVALID_NUMBER_DETECTED:
			solution_.status = 
				solve_result<Dvector>::invalid_number_detected;
			break;
	
			case Ipopt::INTERNAL_ERROR:
			solution_.status = 
				solve_result<Dvector>::internal_error;
			break;
	
			default:
			solution_.status = 
				solve_result<Dvector>::unknown;
		}
	
		solution_.x.resize(nx_);
		solution_.zl.resize(nx_);
		solution_.zu.resize(nx_);
		for(j = 0; j < nx_; j++)
		{	solution_.x[j]   = x[j];
			solution_.zl[j]  = z_L[j];
			solution_.zu[j]  = z_U[j];
		}
		solution_.g.resize(ng_);
		solution_.lambda.resize(ng_);
		for(i = 0; i < ng_; i++)
		{	solution_.g[i]      = g[i];
			solution_.lambda[i] = lambda[i];
		}
		solution_.obj_value = obj_value;
		return;
	}
};

/*! \} */
} // end namespace ipopt
CPPAD_END_NAMESPACE

# endif
