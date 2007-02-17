# ifndef CPPAD_AD_INCLUDED
# define CPPAD_AD_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    GNU General Public License Version 2.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// simple AD operations that must be defined for AD as well as base class
# include <cppad/local/ordered.hpp>
# include <cppad/local/identical.hpp>

// define the template classes that are used by the AD template class
# include <cppad/local/op_code.hpp>
# include <cppad/local/tape_rec.hpp>
# include <cppad/local/ad_tape.hpp>


# define CPPAD_LENGTH_TAPE_TABLE 100

// use this marco for assignment and computed assignment
# define CPPAD_ASSIGN_MEMBER(Op)                              \
	inline AD& operator Op (const AD &right);             \
	inline AD& operator Op (int right)                    \
	{	return *this Op AD(right); }                  \
	inline AD& operator Op (const Base &right)            \
	{	return *this Op AD(right); }                  \
	inline AD& operator Op (const VecAD_reference<Base> &right) \
	{	return *this Op right.ADBase(); }

// use this marco for binary operators 
# define CPPAD_BINARY_MEMBER(Op)                                   \
	inline AD operator Op (const AD &right) const;             \
	inline AD operator Op (int right) const;                   \
	inline AD operator Op (const Base &right) const;           \
	inline AD operator Op (const VecAD_reference<Base> &right) const;

// use this marco for comparison operators 
# define CPPAD_COMPARE_MEMBER(Op)                                    \
	inline bool operator Op (const AD &right) const;             \
	inline bool operator Op (int right) const;                   \
	inline bool operator Op (const Base &right) const;           \
	inline bool operator Op (const VecAD_reference<Base> &right) const;

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
class AD {

	// one argument functions
	friend bool GreaterThanZero <Base> 
		(const AD<Base> &u);
	friend bool LessThanZero  <Base> 
		(const AD<Base> &u);
	friend bool LessThanOrZero  <Base> 
		(const AD<Base> &u);
	friend bool GreaterThanOrZero  <Base> 
		(const AD<Base> &u);
	friend bool Parameter     <Base> 
		(const AD<Base> &u);
	friend bool Variable      <Base> 
		(const AD<Base> &u);
	friend bool IdenticalPar  <Base> 
		(const AD<Base> &u);
	friend bool IdenticalZero <Base> 
		(const AD<Base> &u);
	friend bool IdenticalOne  <Base> 
		(const AD<Base> &u);
	friend int  Integer       <Base> 
		(const AD<Base> &u);
	friend AD Var2Par         <Base>
		(const AD<Base> &u);

	// power function
	friend AD pow <Base>
		(const AD<Base> &x, const AD<Base> &y);

	// IdenticalEqual function
	friend bool IdenticalEqual <Base> 
		(const AD<Base> &u, const AD<Base> &v);

	// EqualOpSeq function
	friend bool EqualOpSeq <Base> 
		(const AD<Base> &u, const AD<Base> &v);

	// NearEqual function
	friend bool NearEqual <Base> (
	const AD<Base> &x, const AD<Base> &y, const Base &r, const Base &a);

	friend bool NearEqual <Base> (
	const Base &x, const AD<Base> &y, const Base &r, const Base &a);

	friend bool NearEqual <Base> (
	const AD<Base> &x, const Base &y, const Base &r, const Base &a);

	// CondExp function
	friend AD<Base> CondExpOp  <Base> (
		enum CompareOp  cop       ,
		const AD<Base> &left      , 
		const AD<Base> &right     , 
		const AD<Base> &trueCase  , 
		const AD<Base> &falseCase 
	);

	// classes
	friend class ADTape<Base>;
	friend class ADDiscrete<Base>;
	friend class ADFun<Base>;
	friend class VecAD<Base>;
	friend class VecAD_reference<Base>;

	// output operations
	friend std::ostream& operator << <Base>
		(std::ostream &os, const AD<Base> &x);
	friend void PrintFor <Base>
		(const char *text, const AD<Base> &x);

public:
	// type of value
	typedef Base value_type;

	// comparison operators
	CPPAD_COMPARE_MEMBER( <  )
	CPPAD_COMPARE_MEMBER( <= )
	CPPAD_COMPARE_MEMBER( >  )
	CPPAD_COMPARE_MEMBER( >= )
	CPPAD_COMPARE_MEMBER( == )
	CPPAD_COMPARE_MEMBER( != )

	// binary operators
	CPPAD_BINARY_MEMBER(+)
	CPPAD_BINARY_MEMBER(-)
	CPPAD_BINARY_MEMBER(*)
	CPPAD_BINARY_MEMBER(/)

	// default constructor
	inline AD(void);

	// construction from base type
	inline AD(const Base &b);

	// use default copy constructor
	// inline AD(const AD &x);

	// contructor from VecAD<Base>::reference
	inline AD(const VecAD_reference<Base> &x);

	// construction from some other type
	template <class T>
	inline AD(const T &t);

	// base type corresponding to an AD object
	friend Base Value <Base> (const AD<Base> &x);

	// binary operators implemented as member functions
	CPPAD_ASSIGN_MEMBER(  = )
	CPPAD_ASSIGN_MEMBER( += )
	CPPAD_ASSIGN_MEMBER( -= )
	CPPAD_ASSIGN_MEMBER( *= )
	CPPAD_ASSIGN_MEMBER( /= )

	// unary operators
	inline AD operator +(void) const;
	inline AD operator -(void) const;

	// destructor
	~AD(void)
	{ }

	// interface so these functions need not be friends
	inline AD Abs(void) const;
	inline AD acos(void) const;
	inline AD asin(void) const;
	inline AD atan(void) const;
	inline AD cos(void) const;
	inline AD cosh(void) const;
	inline AD exp(void) const;
	inline AD log(void) const;
	inline AD sin(void) const;
	inline AD sinh(void) const;
	inline AD sqrt(void) const;

	/*
	Functions declared public so can be accessed by user through
	a macro interface not not intended for direct use.
	Macro interface is documented in BoolFun.h.
	Developer documentation for these fucntions is in BoolFunLink.h
	*/
	static inline bool UnaryBool(
		bool FunName(const Base &x),
		const AD<Base> &x
	);
	static inline bool BinaryBool(
		bool FunName(const Base &x, const Base &y),
		const AD<Base> &x , const AD<Base> &y
	);

	// Make this object correspond to a new variable on the tape
	inline void MakeVariable( size_t taddr )
	{	CppADUnknownError( Parameter(*this) ); // currently a parameter
		CppADUnknownError( taddr > 0 );       // make sure valid taddr

		taddr_ = taddr;
		id_   = *AD<Base>::Id();
	}

	// Make this object correspond to a parameter
	inline void MakeParameter( void )
	{	CppADUnknownError( Variable(*this) ); // currently a variable
		CppADUnknownError( id_ == *AD<Base>::Id() ); 

		id_ = 0;
	}
	//
	// regular member function  connecting this AD object to its tape
	ADTape<Base> *tape_this(void) const
	{	return tape_ptr(id_); }
	//
	// static public functions connecting this AD class to its tape
	//
	static size_t tape_active_count(int inc)
	{	static size_t count       = 0;
		static bool   link_atexit = true;
		CppADUnknownError( (inc == -1) | (inc == 0) | (inc == 1) );
		if( inc > 0 )
		{	if( link_atexit )
			{	atexit( tape_atexit );
				link_atexit = false;
			}
			count++;
			CppADUnknownError( count < CPPAD_LENGTH_TAPE_TABLE );
		}
		else if( inc < 0 )
		{	CppADUnknownError( count > 0 );
			count--;
		}

		return count;
	}
	static ADTape<Base> *tape_any(void)
	{	static size_t i_previous = 0;
		ADTape<Base> **table = tape_table();

		size_t i = i_previous; 
		while( table[i] == CPPAD_NULL )
		{	i = (i + 1) % CPPAD_LENGTH_TAPE_TABLE;
			if( i == i_previous )
				return CPPAD_NULL;
		}
		i_previous = i;
		return table[i];
	}
	// Identifier for the current tape
	static size_t *Id(void)
	{	// assume initialized as zero
		static size_t id;
		if( id )
			return &id;

		// first call to Id()
		id = 1;
		return &id;
	}

	static bool tape_active(size_t id)
	{	size_t i = id % CPPAD_LENGTH_TAPE_TABLE;
		return ( (id > 0) & (tape_id() [i] == id) );
	}

	static ADTape<Base> *tape_ptr(size_t id)
	{	CppADUnknownError( tape_active(id) );

		size_t i = id % CPPAD_LENGTH_TAPE_TABLE;
		ADTape<Base> *tape = tape_table() [i];

		CppADUnknownError( tape != CPPAD_NULL );
		return tape;
	}

	static size_t tape_new_id()
	{	ADTape<Base> **table = tape_table();
		size_t id = *Id();
		size_t i, j;
		for(j = 1; j <= CPPAD_LENGTH_TAPE_TABLE; j++)
		{	id++;
			i = id % CPPAD_LENGTH_TAPE_TABLE;	 
			if( table[i] == CPPAD_NULL )
			{	CppADUnknownError( ! tape_active(id) );
				table[i] = new ADTape<Base>(id); 
				tape_id() [i] = id;
				tape_active_count(+1);
				return id;
			}
		}
		CppADUsageError(
			0,
			"Request for too many AD tapes at the same time."
		);
		return 0;
	}
	static void tape_delete(size_t id)
	{	CppADUnknownError( tape_active(id) );
		size_t i = id % CPPAD_LENGTH_TAPE_TABLE;
		ADTape<Base> *tape = tape_table() [i];

		CppADUnknownError( tape != CPPAD_NULL );	
		delete tape;
		tape_table() [i] = CPPAD_NULL;
		tape_id()    [i] = 0;
		tape_active_count(-1);
		return;
	}


private:
	// value_ corresponding to this object
	Base value_;

	// taddr_ in tape for this variable 
	size_t taddr_;

	// identifier corresponding to taddr
	// This is a parameter if and only if id_ != *AD<Base>::Id()
	size_t id_;
	//
	// private functions connecting this AD class to its tapes
	//
	static size_t *tape_id(void)
	{	// assume all id numbers are initially zero
		static size_t table[CPPAD_LENGTH_TAPE_TABLE];
		return table;
	}	
	static ADTape<Base> **tape_table(void)
	{	// assume all pointers initially zero
		static ADTape<Base> *table[CPPAD_LENGTH_TAPE_TABLE];
		CppADUnknownError( CPPAD_NULL == 0 );
		return table;
	}
	static void tape_atexit(void)
	{	ADTape<Base> **table = tape_table();
		size_t i;
		for(i = 0; i < CPPAD_LENGTH_TAPE_TABLE; i++)
		{	if( table[i] != CPPAD_NULL )
				delete table[i];
		}
		return;
	}
}; 
// ---------------------------------------------------------------------------

} // END CppAD namespace

// operations that expect the AD template class to be defined

# undef CPPAD_ASSIGN_MEMBER
# undef CPPAD_BINARY_MEMBER
# undef CPPAD_COMPARE_MEMBER

# endif
