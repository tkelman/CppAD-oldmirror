# BEGIN SHORT COPYRIGHT
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# This program is free software; you can use it under the terms of the 
#	         Common Public License Version 1.0.
# You should have received a copy of the this license along with this program.
# -----------------------------------------------------------------------------
# END SHORT COPYRIGHT
# automake input file
#
noinst_PROGRAMS  = Example Speed 
#
if CppAD_BOOSTVECTOR
AM_CPPFLAGS      = -I$(BOOST_DIR) \
                   -I. -I$(top_builddir) -I$(FADBAD_DIR) 
else
AM_CPPFLAGS      = -I. -I$(top_builddir) -I$(FADBAD_DIR) 
endif
#
# Fadbad will not survive some of the error and warning flags that could be
# in CPP_ERROR_WARN, so do not use them
Example_CXXFLAGS = -g -Wno-deprecated  
Speed_CXXFLAGS   = -O2 -DNDEBUG -Wno-deprecated  
#
Example_SOURCES  = \
	Example.cpp \
	DetLu.cpp \
	DetMinor.cpp 
#
Speed_SOURCES    = \
	Speed.cpp \
	DetLu.cpp \
	DetMinor.cpp 
