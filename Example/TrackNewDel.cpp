/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can use it under the terms of the 
	         Common Public License Version 1.0.
You should have received a copy of the this license along with this program.
-------------------------------------------------------------------------- */

/*
$begin TrackNewDel.cpp$$

$section Tracking Use of New and Delete: Example and Test$$

$index new, example$$
$index example, new$$
$index test, new$$

$index delete, example$$
$index example, delete$$
$index test, delete$$

$code
$verbatim%Example/TrackNewDel.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/TrackNewDel.h>

bool TrackNewDel(void)
{	bool ok = true;

	// initial count
	size_t count = CppADTrackCount();

	// allocate an array of lenght 5
	double *ptr = 0;
	size_t  newlen = 5;
	ptr = CppADTrackNewVec(newlen, ptr);

	// copy data into the array
	size_t ncopy = newlen;
	size_t i;
	for(i = 0; i < ncopy; i++)
		ptr[i] = double(i);

	// extend the buffer to be lenght 10
	newlen = 10;
	ptr    = CppADTrackExtend(newlen, ncopy, ptr);
		
	// copy data into the new part of the array
	for(i = ncopy; i < newlen; i++)
		ptr[i] = double(i);

	// check the values in the array
 	for(i = 0; i < newlen; i++)
		ok &= (ptr[i] == double(i));

	// free the memory allocated since previous call to TrackCount
	CppADTrackDelVec(ptr);

	// check for memory leak
	ok &= (count == CppADTrackCount());

	return ok;
}

// END PROGRAM
