# BEGIN SHORT COPYRIGHT
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# This program is free software; you can use it under the terms of the 
#	         Common Public License Version 1.0.
# You should have received a copy of the this license along with this program.
# -----------------------------------------------------------------------------
# END SHORT COPYRIGHT
#
# automake input file
#
# Microsoft project files
EXTRA_DIST          = GetStarted.sln GetStarted.vcproj
#
noinst_PROGRAMS     = GetStarted 
noinst_DATA         = GetStarted.out
#
if CppAD_BOOSTVECTOR
AM_CPPFLAGS         = -I$(BOOST_DIR) \
                      -I. -I$(top_builddir) 
else
AM_CPPFLAGS         = -I. -I$(top_builddir) 
endif
#
AM_CXXFLAGS         = -g $(CPP_ERROR_WARN)
#
GetStarted_SOURCES  = GetStarted.cpp

GetStarted.out : GetStarted$(EXEEXT)
	./GetStarted > GetStarted.out
