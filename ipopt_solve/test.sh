#! /bin/bash -e
#
ipopt_cflags=`pkg-config --cflags ipopt | sed -e 's|/include/coin|/include|'`
ipopt_libs=`pkg-config --libs ipopt`
echo_exec g++ get_started.cpp  \
	-g \
	-I . \
	-I .. \
	-I ../cppad_ipopt/src \
	$ipopt_cflags \
	-L ../work/cppad_ipopt/src \
	-lipopt \
	-lcppad_ipopt \
	$ipopt_libs \
	-o get_started
#
echo_exec ./get_started
