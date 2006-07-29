#
#
#
# if not a line of dashes, do nothing
/--------------------------------------------------------------------/b dash
b end
:dash
#
# if next line is not a coyright message, do nothing
N
/CppAD: C++ Algorithmic Differentiation: Copyright (C)/b copyright
/CppAD: C++ Algorithm Differentiation: Copyright (C)/b copyright
b end
:copyright
s/-/*/g
#
# Join the next 15 lines 
N
N
N
N
N
N
N
N
N
N
N
N
N
N
N
# if there is not another line of dashes, do nothing
/\n[# ]*-----------------------------------------------------------/b replace
b end
:replace
s/--/**/
#
# divide into C++ and Shell form for copyright message
/^#/b shell
#
# replace the C++ form of the copyright message
s|.*|/* --------------------------------------------------------------------------\
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell\
\
This program is free software; you can use it under the terms of the \
	         Common Public License Version 1.0.\
You should have received a copy of the this license along with this program.\
-------------------------------------------------------------------------- */|
b end
#
# replace the Shell script for of the copyright message
:shell
s|.*|# -----------------------------------------------------------------------------\
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell\
#\
# This program is free software; you can use it under the terms of the \
#	         Common Public License Version 1.0.\
# You should have received a copy of the this license along with this program.\
# -----------------------------------------------------------------------------/|
#
# end
:end
