#! /bin/bash -e
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
#
ipopt_cflags=`pkg-config --cflags ipopt | sed -e 's|/include/coin|/include|'`
ipopt_libs=`pkg-config --libs ipopt`
echo_exec g++ get_started.cpp  \
	-g \
	-I ../.. \
	-I ../../cppad_ipopt/src \
	$ipopt_cflags \
	$ipopt_libs \
	-o get_started
#
echo_exec ./get_started
