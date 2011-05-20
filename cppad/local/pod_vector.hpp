/* $Id$ */
# ifndef CPPAD_POD_VECTOR_INCLUDED
# define CPPAD_POD_VECTOR_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/local/cppad_assert.hpp>
# include <cppad/omp_alloc.hpp>

CPPAD_BEGIN_NAMESPACE
/*!
\file pod_vector.hpp
File used to define pod_vector class
*/

// ---------------------------------------------------------------------------
/*!
Vector class with Type elements where Type is Plain Old Data.
*/
template <class Type>
class pod_vector {
private:
	/// maximum number of Type elements current allocation can hold
	size_t capacity_;
	/// number of elements currently in this vector
	size_t length_;
	/// pointer to the first type elements 
	/// (not defined and should not be used when capacity_ = 0)
	Type   *data_;
public:
	/// Default constructor: sets capacity and length to zero
	/// (data is also set to zero to avoid compiler warnings).
	inline pod_vector(void) 
	: capacity_(0), length_(0), data_(0)
	{ }
	// ----------------------------------------------------------------------
	/// Destructor: returns allocated memory to \c omp_alloc; see \c extend.
	/// The element destructors are not called because this is Plain Old Data.
	~pod_vector(void)
	{	if( capacity_ > 0 )
		{	void* v_ptr = reinterpret_cast<void*>( data_ );
			omp_alloc::return_memory(v_ptr); 
		}
	}
	// ----------------------------------------------------------------------
	/// current number of elements in this vector.
	inline size_t size(void) const
	{	return length_; }
	/// current capacity (amount of allocated storage) for this vector.
	inline size_t capacity(void) const
	{	return capacity_; }
	/// current data pointer, no longer valid after any of the following:
	/// extend, erase, operator=, and ~pod_vector. 
	/// Take extreem care when using this function.
	inline Type* data(void)
	{	return data_; }
	inline const Type* data(void) const
	{	return data_; }
	// ----------------------------------------------------------------------
	/*!
	Increase the number of elements the end of this vector. 

	\param n
	is the number of elements to add to this vector.

	\return
	is the number  of elements in the vector before it was extended.

	- The new elements are not initialized; i.e., their constructor 
	is not called (becasue they are Plain Old Data; i.e. pod).

	- This is the only routine that allocates memory for \c pod_vector.
	and it uses omp_alloc for this allocation, hence this determines
	which thread corresponds to this vector (when in parallel mode).
	*/
	inline size_t extend(size_t n)
	{	size_t old_length = length_;
		length_          += n;
		// check if we can use current memory
		if( capacity_ >= length_ )
			return old_length;

		// save old memory pointer
		Type* old_data      = data_;

		// get new memory and set capacity
		size_t length_bytes = length_ * sizeof(Type);
		size_t capacity_bytes;
		void* v_ptr = omp_alloc::get_memory(length_bytes, capacity_bytes);
		capacity_   = capacity_bytes / sizeof(Type);
		data_       = reinterpret_cast<Type*>(v_ptr);
		CPPAD_ASSERT_UNKNOWN( length_ <= capacity_ );

		// copy old data to new data
		size_t i;
		for(i = 0; i < old_length; i++)
			data_[i] = old_data[i];

		// return old memory to available pool
		if( old_length > 0 )
		{	v_ptr = reinterpret_cast<void*>( old_data );
			omp_alloc::return_memory(v_ptr); 
		}

		// return value for extend is the old length
		return old_length;
	}
	// ----------------------------------------------------------------------
	/// non-constant element access; i.e., we can change this element value
	Type& operator[](
		/// element index, must be less than length
		size_t i
	)
	{	CPPAD_ASSERT_UNKNOWN( i < length_ );
		return data_[i]; 
	}
	// ----------------------------------------------------------------------
	/// constant element access; i.e., we cannot change this element value
	const Type& operator[](
		/// element index, must be less than length
		size_t i
	) const
	{	CPPAD_ASSERT_UNKNOWN( i < length_ );
		return data_[i]; 
	}
	// ----------------------------------------------------------------------
	/*!
 	Erase all the information in this vector.

	- size and capacity of this vector are set to zero.

	- the memory allocated for this vector is returned to \c omp_alloc.
	*/
	void erase(void)
	{	if( capacity_ > 0 )
		{	void* v_ptr = reinterpret_cast<void*>( data_ );
			omp_alloc::return_memory(v_ptr); 
		}
		capacity_ = length_ = 0;
		return;
	}	
	/// vector assignment operator
	void operator=(
		/// right hand size of the assingment operation
		const pod_vector& x
	)
	{	size_t i;
		if( capacity_ == x.capacity_ )
			length_ = x.length_;
		else
		{	// free old memory and get new memory of sufficient length
			erase();
			extend( x.length_ );
			// In both cases should be first omp_alloc capacity >= length 
			CPPAD_ASSERT_UNKNOWN( capacity_ == x.capacity_ );
		}
		CPPAD_ASSERT_UNKNOWN( length_ == x.length_ );
		for(i = 0; i < length_; i++)
		{	data_[i] = x.data_[i]; }
	}
};

CPPAD_END_NAMESPACE
# endif
