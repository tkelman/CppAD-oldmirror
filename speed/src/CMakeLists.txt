# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Build the cppad_ipopt/src library
# Inherit environment from ../CMakeList.txt

# Specifies build type for this directory. Possible values are 
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE RELEASE)

#  add_library(<name> [STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL]
#	source1 source2 ... sourceN)
# )
ADD_LIBRARY(speed_src EXCLUDE_FROM_ALL
	link_det_lu.cpp
	link_det_minor.cpp
	link_mat_mul.cpp
	link_ode.cpp
	link_poly.cpp
	link_sparse_hessian.cpp
	link_sparse_jacobian.cpp
	microsoft_timer.cpp
)

# Compiler flags for cppad source
add_cppad_cxx_flags( speed_src )
