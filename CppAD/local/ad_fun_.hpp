# ifndef CppADADFunIncluded
# define CppADADFunIncluded

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */
/*
$begin ADFun$$
$spell 
	xk
	Ind
	bool
	Taylor
	sizeof
	const
	std
	ind_taddr
	dep_taddr
$$

$spell
$$

$section ADFun Objects$$

$index ADFun, object$$
$index object, ADFun$$

$head Purpose$$
An AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$
is stored in an $code ADFun$$ object by its $xref/FunConstruct/$$.
The $code ADFun$$ object can then be used to calculate function values,
derivative values, and other values related to the corresponding function.

$childtable%
	CppAD/local/independent_.hpp%
	CppAD/local/fun_construct_.hpp%
	CppAD/local/dependent_.hpp%
	omh/SeqProperty.omh%
	CppAD/local/fun_eval_.hpp%
	CppAD/local/drivers_.hpp%
	CppAD/local/fun_check_.hpp%
	omh/FunDeprecated.omh
%$$

$end
*/


// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
class ADFun {
	// type used for packing sparsity patters
	typedef size_t Pack;

public:
	// default constructor
	ADFun(void) 
	: totalNumVar(0), Taylor(CppADNull), ForJac(CppADNull)
	{ }

	// sequence constructor
	template <typename ADvector>
	ADFun(const ADvector &x, const ADvector &y);

	// destructor
	~ADFun(void)
	{	if( Taylor != CppADNull )
			CppADTrackDelVec(Taylor);
		if( ForJac != CppADNull )
			CppADTrackDelVec(ForJac);
	}

	// assign a new operation sequence
	template <typename ADvector>
	void Dependent(const ADvector &y);

	// forward mode sweep
	template <typename VectorBase>
	VectorBase Forward(size_t p, const VectorBase &u);

	// reverse mode sweep
	template <typename VectorBase>
	VectorBase Reverse(size_t p, const VectorBase &v) const;

	// forward mode Jacobian sparsity 
	template <typename VectorBase>
	VectorBase ForSparseJac(size_t q, const VectorBase &Px);

	// reverse mode Jacobian sparsity 
	template <typename VectorBase>
	VectorBase RevSparseJac(size_t q, const VectorBase &Py) const;

	// reverse mode Hessian sparsity 
	template <typename VectorBase>
	VectorBase RevSparseHes(size_t q, const VectorBase &Py) const;

	// does this AD operation sequence use VecAD<Base>::reference operands
	bool use_VecAD(void) const
	{	return Rec.NumVecInd() > 0; }

	// number of variables in opertion sequence
	size_t size_var(void) const
	{	return totalNumVar; }

	// number of Taylor coefficients currently calculated (per variable)
	size_t size_taylor(void) const
	{	return taylor_per_var; } 

	// set number of coefficients currently allocated (per variable)
	void capacity_taylor(size_t per_var);   

	// number of independent variables
	size_t Domain(void) const
	{	return ind_taddr.size(); }

	// number of dependent variables
	size_t Range(void) const
	{	return dep_taddr.size(); }

	// is variable a parameter
	bool Parameter(size_t i)
	{	CppADUsageError(
			i < dep_taddr.size(),
			"Argument to Parameter is >= dimension of range space"
		);
		return dep_parameter[i]; 
	}

# ifndef NDEBUG
	// in not NDEBUG case, number of comparison operations that change
	size_t CompareChange(void) const
	{	return compareChange; }
# endif

	// calculate entire Jacobian
	template <typename VectorBase>
	VectorBase Jacobian(const VectorBase &x); 

	// calculate Hessian for one component of f
	template <typename VectorBase>
	VectorBase Hessian(const VectorBase &x, size_t i); 

	// forward mode calculation of partial w.r.t one domain component
	template <typename VectorBase>
	VectorBase ForOne(
		const VectorBase   &x ,
		size_t              j );

	// reverse mode calculation of derivative of one range component
	template <typename VectorBase>
	VectorBase RevOne(
		const VectorBase   &x ,
		size_t              i );

	// forward mode calculation of a subset of second order partials
	template <typename VectorBase, typename VectorSize_t>
	VectorBase ForTwo(
		const VectorBase   &x ,
		const VectorSize_t &J ,
		const VectorSize_t &K );

	// reverse mode calculation of a subset of second order partials
	template <typename VectorBase, typename VectorSize_t>
	VectorBase RevTwo(
		const VectorBase   &x ,
		const VectorSize_t &I ,
		const VectorSize_t &J );

	// ------------------- Deprecated -----------------------------

	// number of variables in opertion sequence
	size_t Size(void) const
	{	return totalNumVar; }

	// number of Taylor coefficients currently stored (per variable)
	size_t Order(void) const
	{	return taylor_per_var - 1; }

	// amount of memory for each variable 
	size_t Memory(void) const
	{	size_t pervar  = TaylorColDim * sizeof(Base)
		+ ForJacColDim * sizeof(Pack);
		size_t total   = totalNumVar * pervar + Rec.Memory();
		return total;
	}

	// number of Taylor coefficients currently calculated (per variable)
	size_t taylor_size(void) const
	{	return taylor_per_var; } 
	// ------------------------------------------------------------
private:
	// maximum amount of memory required for this function object
	// mutable size_t memoryMax;

	// debug checking number of comparision operations that changed
	size_t compareChange;

	// number of Taylor coefficieint per variable (currently stored)
	size_t taylor_per_var;

	// number of bits currently calculated per row of the ForJac array
	size_t ForJacBitDim; 

	// number of columns currently allocated for Taylor array
	size_t TaylorColDim;

	// number of columns currently allocated for ForJac array
	size_t ForJacColDim;

	// number of rows (variables) in the recording (Rec)
	size_t totalNumVar;

	// tape address for the independent variables
	CppAD::vector<size_t> ind_taddr;

	// tape address and parameter flag for the dependent variables
	CppAD::vector<size_t> dep_taddr;
	CppAD::vector<bool>   dep_parameter;

	// the operations corresponding to this function
	TapeRec<Base> Rec;

	// results of the forward mode calculations
	Base *Taylor;

	// results of the forward mode Jacobian sparsity calculations
	Pack *ForJac;
};
// ---------------------------------------------------------------------------

} // END CppAD namespace

// non-user interfaces
# include <CppAD/local/forward_sweep_.hpp>
# include <CppAD/local/reverse_sweep_.hpp>
# include <CppAD/local/for_jac_sweep_.hpp>
# include <CppAD/local/rev_jac_sweep_.hpp>
# include <CppAD/local/rev_hes_sweep_.hpp>


// user interfaces
# include <CppAD/local/independent_.hpp>
# include <CppAD/local/dependent_.hpp>
# include <CppAD/local/fun_construct_.hpp>
# include <CppAD/local/fun_eval_.hpp>
# include <CppAD/local/drivers_.hpp>
# include <CppAD/local/fun_check_.hpp>

# endif
