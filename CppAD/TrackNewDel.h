# ifndef CppADTrackNewDelIncluded
# define CppADTrackNewDelIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
/*
$begin TrackNewDel$$
$spell
	Cpp
	new_vec
	Vec
	new_len
	ncopy
	const
$$

$section Routines That Track Use of New and Delete$$
$index new, track$$
$index delete, track$$
$index track, new and delete$$
$index memory, track$$

$head Syntax$$
$syntax%%new_vec% = TrackNewOne(%file%, %line%, %old_vec%)
%$$
$syntax%%new_one% = TrackNewVec(%file%, %line%, %new_len%, %old_one%)
%$$
$syntax%TrackDelOne(%file%, %line%, %old_one%)
%$$
$syntax%TrackDelVec(%file%, %line%, %old_vec%)
%$$
$syntax%%new_vec% = TrackExtend(%file%, %line%, %new_len%, %ncopy%, %old_vec%)
%$$
$syntax%%count% = TrackCount(%file%, %line%)
%$$


$head Purpose$$
These routines 
aid in the use of 
$code new$$, $code new[]$$, $code delete$$, and  $code delete[]$$
during the execution of a C++ program.

$head file$$
The argument $italic file$$ has prototype
$syntax%
	const char *%file%
%$$
It should be the source code file name 
where the call to $code TrackNew$$ is located.
The best way to accomplish this is the use the preprocessor symbol
$code __FILE__$$ for this argument.

$head line$$
The argument $italic line$$ has prototype
$syntax%
	int %line%
%$$
It should be the source code file line number 
where the call to $code TrackNew$$ is located.
The best way to accomplish this is the use the preprocessor symbol
$code __LINE__$$ for this argument.

$head old_one$$
The argument $italic old_one$$ has prototype
$syntax%
	%Type% *%old_one%
%$$
This argument is used to identify the type $italic Type$$.

$head old_vec$$
The argument $italic old_vec$$ has prototype
$syntax%
	%Type% *%old_vec%
%$$
This argument is used to identify the type $italic Type$$.

$head new_len$$
The argument $italic new_len$$ has prototype
$syntax%
	size_t %new_len%
%$$

$head new_one$$
The return value $italic new_one$$ has prototype
$syntax%
	%Type% *%new_one%
%$$
It points to the newly allocated object
that was allocated using
$syntax%
	new Type
%$$

$head new_vec$$
The return value $italic new_vec$$ has prototype
$syntax%
	%Type% *%new_vec%
%$$
It points to the newly allocated vector of objects
that were allocated using
$syntax%
	new Type[%new_len%]
%$$

$head ncopy$$
The argument $italic ncopy$$ has prototype
$syntax%
        size_t %ncopy%
%$$
This specifies the number of elements that are copied from
the old array to the new array.
The value of $italic ncopy$$ 
must be less than or equal $italic new_len$$.

$head TrackNewOne$$
$index TrackNewOne$$
$index NDEBUG$$
This routine is used to start the tracking of memory allocation 
using $code new$$.
The value of $italic old_one$$ does not matter for this case
(however the type of $italic old_one$$ does matter).
If $code NDEBUG$$ is not defined and the memory cannot be allocated,
$xref/ErrorHandler/$$ is used to generate a message
stating that there was not sufficient memory.
The preprocessor macro call
$syntax%
	CppADTrackNewOne(%old_one%)
%$$
expands to
$syntax%
	CppAD::TrackNewOne(__FILE__, __LINE__, %old_one%)
%$$

$head TrackNewVec$$
$index TrackNewVec$$
$index NDEBUG$$
This routine is used to start the tracking of memory allocation 
using $code new[]$$.
The value of $italic old_vec$$ does not matter for this case
(however the type of $italic old_vec$$ does matter).
If $code NDEBUG$$ is not defined and the memory cannot be allocated,
$xref/ErrorHandler/$$ is used to generate a message
stating that there was not sufficient memory.
The preprocessor macro call
$syntax%
	CppADTrackNewVec(%new_len%, %old_vec%)
%$$
expands to
$syntax%
	CppAD::TrackNewVec(__FILE__, __LINE__, %new_len%, %old_vec%)
%$$


$head TrackDelOne$$
$index TrackDelOne$$
This routine is used to delete an object 
that was allocated using $code TrackNewOne$$.
If $code NDEBUG$$ is not defined, $code TrackDelOne$$ checks that
$italic old_one$$ was allocated by $code TrackNewOne$$
and has not yet been freed.
If this is not the case,
$xref/ErrorHandler/$$ is used to generate an error message.
The preprocessor macro call
$syntax%
	CppADTrackDelOne(%old_one%)
%$$
expands to
$syntax%
	CppAD::TrackDelOne(__FILE__, __LINE__, %old_one%)
%$$


$head TrackDelVec$$
$index TrackDelVec$$
This routine is used to delete a vector of objects 
that were allocated using $code TrackNewVec$$ or $code TrackExtend$$.
If $code NDEBUG$$ is not defined, $code TrackDelVec$$ check that
$italic old_vec$$ was allocated by $code TrackNewVec$$ or $code TrackExtend$$
and has not yet been freed.
If this is not the case,
$xref/ErrorHandler/$$ is used to generate an error message.
The preprocessor macro call
$syntax%
	CppADTrackDelVec(%old_vec%)
%$$
expands to
$syntax%
	CppAD::TrackDelVec(__FILE__, __LINE__, %old_vec%)
%$$

$head TrackExtend$$
$index TrackExtend$$
This routine is used to 
allocate a new vector (using $code TrackNewVec$$),
and copy $italic ncopy$$ elements from the old vector to the new vector.
The preprocessor macro call
$syntax%
	CppADTrackExtend(%new_len%, %ncopy%, %old_vec%)
%$$
expands to
$syntax%
	CppAD::TrackExtend(__FILE__, __LINE__, %new_len%, %ncopy%, %old_vec%)
%$$
If $italic ncopy$$ is greater than zero, $italic old_vec$$ 
must have been allocated using $code TrackNewVec$$ or $code TrackExtend$$.
In this case, the vector pointed to by $italic old_vec$$ 
must be have at least $italic ncopy$$ elements
and it will be freed (using $code TrackDelVec$$).

$head TrackCount$$
$index TrackCount$$
The return value $italic count$$ has prototype
$syntax%
	size_t %count%
%$$
If $code NDEBUG$$ is defined, $italic count$$ will be zero.
Otherwise, it will be
the number of allocations (both single object and vector) that 
have been completed by
$code TrackNewOne$$, $code TrackNewVec$$ and $code TrackExtend$$
and not yet freed
(by $code TrackDelOne$$ or $code TrackDelVec$$).
$syntax%
	CppADTrackCount()
%$$
expands to
$syntax%
	CppAD::TrackCount(__FILE__, __LINE__)
%$$

$head Example$$
$children%
	Example/TrackNewDel.cpp
%$$
The file $xref/TrackNewDel.cpp/$$
contains an example and test of these functions.
It returns true, if it succeeds, and false otherwise.

$end
------------------------------------------------------------------------------
*/
# include <CppAD/local/CppADError.h>
# include <sstream>
# include <string>

# define CppADDebugTrack 1

# ifndef CppADNull
# define CppADNull	0
# endif

# define CppADTrackNewOne(old_one) \
	CppAD::TrackNewOne(__FILE__, __LINE__, old_one)

# define CppADTrackNewVec(new_len, old_vec) \
	CppAD::TrackNewVec(__FILE__, __LINE__, new_len, old_vec)

# define CppADTrackDelOne(old_one) \
	CppAD::TrackDelOne(__FILE__, __LINE__, old_one)

# define CppADTrackDelVec(old_vec) \
	CppAD::TrackDelVec(__FILE__, __LINE__, old_vec)

# define CppADTrackExtend(new_len, ncopy, old_vec) \
	CppAD::TrackExtend(__FILE__, __LINE__, new_len, ncopy, old_vec)

# define CppADTrackCount() \
	CppAD::TrackCount(__FILE__, __LINE__)

namespace CppAD { // Begin CppAD namespace

// TrackElement ------------------------------------------------------------
class TrackElement {
public:
	typedef enum { 
		none   ,
		new_one,
		new_vec,
		del_one,
		del_vec,
		extend
	} Routine;

	Routine    routine; // which routine created this element
	std::string   file; // corresponding file name
	int           line; // corresponding line number
	void          *ptr; // value returned by TrackNew
	TrackElement *next; // next element in linked list

	// default contructor (used to initialize root)
	TrackElement(void)
	: routine(none), file(""), line(0), ptr(CppADNull), next(CppADNull)
	{ }
	
	TrackElement(Routine r, const char *f, int l, void *p)
	: routine(r), file(f), line(l), ptr(p), next(CppADNull)
	{	CppADUnknownError( p != CppADNull);
	}

	// There is only one tracking list and it starts it here
	static TrackElement *Root(void)
	{	static TrackElement root;
		return &root;
	}

	// Print the linked list
	static void Print(void)
	{	TrackElement *E = Root();
		std::cout << "Begin Track List" << std::endl;
		while( E->next != CppADNull )
		{	E = E->next;
			std::cout << "next = " << E->next;
			std::cout << ", ptr = " << E->ptr;
			std::cout << ", routine  = " << E->routine;
			std::cout << ", line = " << E->line;
			std::cout << ", file = " << E->file;
			std::cout << std::endl;
		}
		std::cout << "End Track List:" << std::endl;
	}
}; 


// TrackError ----------------------------------------------------------------
inline void TrackError(
	const char *routine,
	const char *file   ,
	int         line   ,
	char       *msg    )
{
	std::ostringstream buf;
	buf << routine
	    << ": at line "
	    << line
	    << " in file "
	    << file
	    << std::endl
	    << msg; 
	std::string str = buf.str();
	size_t      n   = str.size();
	size_t i;
	char *message = new char[n + 1];
	for(i = 0; i < n; i++)
		message[i] = str[i];
	message[n] = '\0';
	CppADUsageError(false , message);
}

// TrackNew ---------------------------------------------------------------
# ifdef NDEBUG
template <class Type>
inline Type *TrackNewOne(const char *file, int line, size_t len, Type *old_one)
{	return (new Type); 
}
template <class Type>
inline Type *TrackNewVec(const char *file, int line, size_t len, Type *old_vec)
{	return (new Type[len]); 
}

# else

template <class Type>
Type *TrackNew(
	TrackElement::Routine  r      ,
	const char            *file   , 
	int                    line   , 
	size_t                 len    ,
	Type                  *old_ptr)
{
	// try to allocate the new memrory
	Type *new_ptr = CppADNull;
	try
	{	if( r == TrackElement::new_one )
			new_ptr = new Type;
		else	new_ptr = new Type[len];
	}
	catch(...)
	{	char *name = "Track program error";
		if( r == TrackElement::new_one )
			name = "TrackNewOne";
		else if( r == TrackElement::new_vec )
			name = "TrackNewVec";
		else if( r == TrackElement::extend )
			name = "TrackExtend";
		TrackError(name, file, line, 
			"Cannot allocate sufficient memory"
		);
	}
	// create tracking element
	void *v_ptr = static_cast<void *>(new_ptr);
	TrackElement *E = new TrackElement(r, file, line, v_ptr);

	// get the root
	TrackElement *root = TrackElement::Root();

	// put this elemenent at the front of linked list
	E->next    = root->next;  
	root->next = E;

	return new_ptr;
}

template <class Type>
Type *TrackNewOne(
	const char *file   , 
	int         line   , 
	Type       *old_one)
{	size_t len = 0;
	return TrackNew(TrackElement::new_one, file, line, len, old_one);
}
template <class Type>
Type *TrackNewVec(
	const char *file   , 
	int         line   , 
	size_t      len    ,
	Type       *old_vec)
{	return TrackNew(TrackElement::new_vec, file, line, len, old_vec);
}

# endif

// TrackDel --------------------------------------------------------------
# ifdef NDEBUG
template <class Type>
inline void TrackDelOne(const char *file, int line, Type *old_one)
{	 delete old_one; 
}
template <class Type>
inline void TrackDelVec(const char *file, int line, Type *old_vec)
{	 delete [] old_vec; 
}

# else

template <class Type>
void TrackDel(
	TrackElement::Routine  r       ,
	const char            *file    ,
	int                    line    ,
	Type                  *old_ptr )
{
	TrackElement        *P;
	TrackElement        *E;

	// search list for pointer
	P          = TrackElement::Root();
	E          = P->next;
	void *v_ptr = static_cast<void *>(old_ptr);
	while(E != CppADNull && E->ptr != v_ptr)
	{	P = E;
		E = E->next;
	}
	// name of routine that user called
	char *name = "Track program error";
	if( r == TrackElement::del_one )
		name = "TrackDelOne";
	else if( r == TrackElement::del_vec )
		name = "TrackDelVec";
	else if( r == TrackElement::extend )
		name = "TrackExtend";

	// check if pointer was not in list
	if( E == CppADNull || E->ptr != v_ptr )
	{	TrackError(name, file, line, 
			"Invalid value for the argument old pointer"
		); 
	}

	// check for proper type of pointer
	if( E->routine == TrackElement::new_one && r != TrackElement::del_one )
	{	TrackError(name, file, line, 
			"must use TrackDelOne to delete this old pointer"
		); 
	}
	if( E->routine != TrackElement::new_one && r == TrackElement::del_one )
	{	TrackError(name, file, line, 
			"must use TrackDelVec to delete this old pointer"
		); 
	}

	// remove tracking element from list
	P->next = E->next;

	// delete allocated pointer
	if( r == TrackElement::del_one )
		delete old_ptr;
	else	delete [] old_ptr;

	// delete tracking element
	delete E;

	return;
}

template <class Type>
void TrackDelOne(
	const char *file    ,
	int         line    ,
	Type       *old_one )
{	TrackDel(TrackElement::del_one, file, line, old_one);
}

template <class Type>
void TrackDelVec(
	const char *file    ,
	int         line    ,
	Type       *old_vec )
{	TrackDel(TrackElement::del_vec, file, line, old_vec);
}

# endif

// TrackExtend --------------------------------------------------------------
template <class Type>
Type *TrackExtend(
	const char *file     , 
	int         line     , 
	size_t      new_len  , 
	size_t      ncopy    ,
	Type       *old_vec  ) 
{	// check size of ncopy
	CppADUsageError( 
		ncopy <= new_len,
		"TrackExtend: ncopy is greater than new_len."
	);

	// allocate the new memrory
	Type *new_vec = TrackNewVec(file, line, new_len, old_vec);

	// copy the data
	size_t i = ncopy;
	while(i)
	{	--i;
		new_vec[i] = old_vec[i];
	}

	// delete the old vector 
	if( ncopy > 0 )
		TrackDelVec(file, line, old_vec);

	return new_vec;
}

// TrackCount --------------------------------------------------------------
inline size_t TrackCount(const char *file, int line)
{
	TrackElement *E = TrackElement::Root();
	size_t count = 0;
	while( E->next != CppADNull ) 
	{
# if CppADTrackDebug
		std::cout << "Before TrackCount:" << std::endl;
		TrackElement::Print();
# endif
		++count;
		E = E->next;
	}
	return count;
}
// ---------------------------------------------------------------------------

} // End CppAD namespace

# undef CppADDebugTrack

# endif
