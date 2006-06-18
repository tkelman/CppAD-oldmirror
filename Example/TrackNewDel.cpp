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

	// allocate a single object
	double *ptr_one = 0;
	ptr_one = CppADTrackNewOne(ptr_one);

	// assing a value to the object
	*ptr_one = 3.;

	// check the value in the object
	ok &= (*ptr_one == 3.);

	// allocate a vector of lenght 5
	double *ptr_vec = 0;
	size_t  new_len = 5;
	ptr_vec = CppADTrackNewVec(new_len, ptr_vec);

	// assing a value to the elements of the vector
	size_t ncopy = new_len;
	size_t i;
	for(i = 0; i < ncopy; i++)
		ptr_vec[i] = double(i);

	// extend the vector to be lenght 10
	new_len = 10;
	ptr_vec = CppADTrackExtend(new_len, ncopy, ptr_vec);
		
	// copy data into the new part of the vector
	for(i = ncopy; i < new_len; i++)
		ptr_vec[i] = double(i);

	// check the values in the vector
 	for(i = 0; i < new_len; i++)
		ok &= (ptr_vec[i] == double(i));

	// free the memory allocated since previous call to TrackCount
	CppADTrackDelVec(ptr_vec);
	CppADTrackDelOne(ptr_one);

	// check for memory leak
	ok &= (count == CppADTrackCount());

	return ok;
}

// END PROGRAM
