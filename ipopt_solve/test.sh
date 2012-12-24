#! /bin/bash -e
#
echo_exec g++ -g \
	-I . \
	-I .. \
	-I ../cppad_ipopt/src \
	-I $HOME/prefix/ipopt/include \
	get_started.cpp -o get_started
#
echo_exec ./get_started
