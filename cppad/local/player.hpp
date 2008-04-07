# ifndef CPPAD_PLAYER_INCLUDED
# define CPPAD_PLAYER_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin player$$ $comment CppAD Developer Documentation$$

$spell
	Var
	inline
	VecInd
	sizeof
	Num
	Ind
	Cpp
	Rec
	const
	Op
$$

$section A CppAD Program as Recorded on Tape$$
$index tape, record$$
$index record, tape$$
$index player$$

$head Syntax$$
$syntax%player<%Base%> %Rec%$$
$pre
$$
$syntax%player<%Base%> %Rec%(const player<%Base%> &%Other%)%$$


$head Default Constructors$$
The default constructor 
$syntax%
	player<%Base%> %Rec%
%$$
creates a program recording called $italic Rec$$ with no contents and some
default setting for the size of its buffers.

$head Copy Constructor$$
The copy constructor
$syntax%
	player<%Base%> %Rec%(const player<%Base%> &%Other%)
%$$
creates $italic Rec$$ as a program recording with all the same
information as $italic Other$$ and with the smallest possible buffer sizes
that will hold that information.

$head Erase$$
$index player, Erase$$
$index Erase, player$$
The syntax 
$syntax%
	void %Rec%.Erase()
%$$
erases the contents of $italic Rec$$.
The buffers used to store the tape information are returned
to the system (so as to conserve on memory).

$head Get$$
$index player, Get$$
$index Get, player$$

$subhead Op$$
$index GetOp$$
The syntax
$syntax%
	OpCode %Rec%.GetOp(size_t %i%) const
%$$
returns the value of $italic op$$ in the $th i+1$$ call to 
$syntax%%Rec%.PutOp(%op%)%$$.

$subhead VecInd$$
$index GetVecInd$$
The syntax
$syntax%
	OpCode %Rec%.GetVecInd(size_t %i%) const
%$$
returns the value of $italic vecInd$$ in the corresponding call 
$syntax%%Rec%.PutVecInd(%vecInd%)%$$
where $italic i$$ is the return value of $code PutVecInd$$. 

$subhead Ind$$
$index GetInd$$
The syntax
$syntax%
	const size_t *%Rec%.GetInd(size_t %n%, size_t %i%) const
%$$
returns a pointer to a copy of $italic n$$ values in the Ind recording
starting at the index $italic i$$.

$subhead Par$$
$index GetPar$$
The syntax
$syntax%
	const %Base% *%Rec%.GetPar(size_t %i%) const
%$$
returns a pointer to a value equal to
$italic par$$ in the corresponding call 
$syntax%%Rec%.PutPar(%par%)%$$ 
where $italic i$$ is the return value of $code PutPar$$.
(If $code NDEBUG$$ is not defined, $code GetPar$$ checks that
the argument $italic i$$ is valid.)

$head Num$$
$index player, Num$$
$index Num, player$$

$subhead Op$$
$index NumOp$$
The syntax
$syntax%
	size_t %Rec%.NumOp(void) const
%$$
returns the number of Op values
that are currently stored in $italic Rec$$.
This increments by one each time $code PutOp$$ is called; i.e.,
it is the number of calls to $code PutOp$$.

$subhead Ind$$
$index NumInd$$
The syntax
$syntax%
	size_t %Rec%.NumInd(void) const
%$$
returns the number of Ind values
that are currently stored in $italic Rec$$.
This increments by one for each value that is stored by $code PutInd$$.

The syntax
$syntax%
	size_t %Rec%.NumVecInd(void) const
%$$
returns the number of VecInd values
that are currently stored in $italic Rec$$.
This increments by one each time $code PutVecInd$$ is called.

$subhead Par$$
$index NumPar$$
The syntax
$syntax%
	size_t %Rec%.NumPar(void) const
%$$
returns the number of Par values
that are currently stored in $italic Rec$$.
This increment by one or zero each time $code PutPar$$ is called.

$head Replace$$
$index player, Replace$$
$index Replace, player$$

$subhead Ind$$
$index ReplaceInd$$
The syntax
$syntax%
	size_t %Rec%.ReplaceInd(size_t %index%, size_t %value%)
%$$
Replaces the single value with index $italic index$$ in the sequence of
Ind values stored by calls to $syntax%%Rec%.PutInd%$$.
The argument $italic index$$ must be less than $syntax%%Rec%.NumInd()%$$.

$head TotNumVar$$
$index recorder, TotNumVar$$
$index TotNumVar, recorder$$
The function call
$syntax%
	inline size_t %Rec%.TotNumVar()
%$$
returns the number of variables that are in the recording.


$head Memory$$
$index player, Memory$$
$index Memory, player$$
The syntax
$syntax%
	size_t %Rec%.Memory(void) const
%$$
returns the number of memory units ($code sizeof$$) required to store
the information in $italic Rec$$.



$end
------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
class player {

public:
	// default
	player(void) 
	{	
		TotalNumberVar_ = 0;

		NumberOp_       = 0;
		LengthOp_       = 0;
		Op_             = CPPAD_NULL;

		NumberVecInd_   = 0;
		LengthVecInd_   = 0;
		VecInd_         = CPPAD_NULL;

		NumberInd_      = 0;
		LengthInd_      = 0;
		Ind_            = CPPAD_NULL;

		NumberPar_      = 0;
		LengthPar_      = 0;
		Par_            = CPPAD_NULL;

		NumberTxt_      = 0;
		LengthTxt_      = 0;
		Txt_            = CPPAD_NULL;

	}

	// destructor
	~player(void)
	{	if( LengthOp_ > 0 )
			CPPAD_TRACK_DEL_VEC(Op_);
		if( LengthVecInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(VecInd_);
		if( LengthInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(Ind_);
		if( LengthPar_ > 0 )
			CPPAD_TRACK_DEL_VEC(Par_);
		if( LengthTxt_ > 0 )
			CPPAD_TRACK_DEL_VEC(Txt_);
	}

	// assignment from another recording
	void operator=(const recorder<Base> &Other)
	{	size_t i;

		if( LengthOp_ > 0 )
			CPPAD_TRACK_DEL_VEC(Op_);
		if( LengthVecInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(VecInd_);
		if( LengthInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(Ind_);
		if( LengthPar_ > 0 )
			CPPAD_TRACK_DEL_VEC(Par_);
		if( LengthTxt_ > 0 )
			CPPAD_TRACK_DEL_VEC(Txt_);

		// Var
		TotalNumberVar_  = Other.TotalNumberVar_;

		// Op
		NumberOp_        = Other.NumberOp_;
		LengthOp_        = Other.NumberOp_;

		// VecInd
		NumberVecInd_    = Other.NumberVecInd_;
		LengthVecInd_    = Other.NumberVecInd_;

		// Ind
		NumberInd_       = Other.NumberInd_;
		LengthInd_       = Other.NumberInd_;

		// Par
		NumberPar_       = Other.NumberPar_;
		LengthPar_       = Other.NumberPar_;

		// Txt
		NumberTxt_       = Other.NumberTxt_;
		LengthTxt_       = Other.NumberTxt_;

		// Allocate the memory
		if( LengthOp_ == 0 )
			Op_ = CPPAD_NULL;
		else	Op_ = CPPAD_TRACK_NEW_VEC(LengthOp_,      Op_);
		if( LengthVecInd_ == 0 )
			VecInd_ = CPPAD_NULL;
		else	VecInd_ = CPPAD_TRACK_NEW_VEC(LengthVecInd_, VecInd_);
		if( LengthInd_ == 0 )
			Ind_ = CPPAD_NULL;
		else	Ind_ = CPPAD_TRACK_NEW_VEC(LengthInd_,       Ind_);
		if( LengthPar_ == 0 )
			Par_ = CPPAD_NULL;
		else	Par_ = CPPAD_TRACK_NEW_VEC(LengthPar_,       Par_);
		if( LengthTxt_ == 0 )
			Txt_ = CPPAD_NULL;
		else	Txt_ = CPPAD_TRACK_NEW_VEC(LengthTxt_,       Txt_);

		// Copy the data
		i = NumberOp_;
		while(i--)
			Op_[i] = Other.Op_[i];
		i = NumberVecInd_;
		while(i--)
			VecInd_[i] = Other.VecInd_[i];
		i = NumberInd_;
		while(i--)
			Ind_[i] = Other.Ind_[i];
		i = NumberPar_;
		while(i--)
			Par_[i] = Other.Par_[i];
		i = NumberTxt_;
		while(i--)
			Txt_[i] = Other.Txt_[i];
	}

	// erase all information in recording
	void Erase(void)
	{	
		TotalNumberVar_  = 0;
		NumberOp_        = 0;
		NumberVecInd_    = 0;
		NumberInd_       = 0;
		NumberPar_       = 0;
		NumberTxt_       = 0;

		if( LengthOp_ > 0 )
			CPPAD_TRACK_DEL_VEC(Op_);
		if( LengthVecInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(VecInd_);
		if( LengthInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(Ind_);
		if( LengthPar_ > 0 )
			CPPAD_TRACK_DEL_VEC(Par_);
		if( LengthTxt_ > 0 )
			CPPAD_TRACK_DEL_VEC(Txt_);

		LengthOp_        = 0;
		LengthVecInd_    = 0;
		LengthInd_       = 0;
		LengthPar_       = 0;
		LengthTxt_       = 0;
	}

	/*
	retrieve information from recording
	*/
	OpCode GetOp (size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i < NumberOp_);
		return Op_[i];
	}
	size_t GetVecInd (size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i < NumberVecInd_);
		return VecInd_[i];
	}
	const Base *GetPar(size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i < NumberPar_);
		return Par_ + i;
	}
	const size_t *GetInd(size_t n, size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i + n <= NumberInd_)
		return Ind_ + i;
	}
	const char *GetTxt(size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i < NumberTxt_);
		return Txt_ + i;
	}
	
	/*
	replace information 
	*/
	void ReplaceInd(size_t i, size_t value)
	{	CPPAD_ASSERT_UNKNOWN( i < NumberInd_);
		Ind_[i] = value;
	}

	// number of values
	size_t TotNumVar(void) const
	{	return TotalNumberVar_; }
	size_t NumOp(void) const
	{	return NumberOp_; }
	size_t NumVecInd(void) const
	{	return NumberVecInd_; }
	size_t NumInd(void) const
	{	return NumberInd_; }
	size_t NumPar(void) const
	{	return NumberPar_; }
	size_t NumTxt(void) const
	{	return NumberTxt_; }

	// amount of memory used 
	size_t Memory(void) const
	{	return LengthOp_ * sizeof(OpCode) 
		     + LengthVecInd_ * sizeof(size_t)
		     + LengthInd_ * sizeof(size_t)
		     + LengthPar_ * sizeof(Base)
		     + LengthTxt_ * sizeof(char);
	}

private:
	size_t    TotalNumberVar_;

	size_t    NumberOp_;
	size_t    LengthOp_;
	OpCode   *Op_;

	size_t    NumberVecInd_;
	size_t    LengthVecInd_;
	size_t   *VecInd_;

	size_t    NumberInd_;
	size_t    LengthInd_;
	size_t   *Ind_;

	size_t    NumberPar_;
	size_t    LengthPar_;
	Base     *Par_;

	size_t    NumberTxt_;
	size_t    LengthTxt_;
	char     *Txt_;
};

} // END CppAD namespace

# endif
