# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# $begin openmp_run.sh$$ $newlinech #$$
# $spell
#	openmp
#	vec
#	exe
#	rm
#	false false
#	cpp
# $$
#
# $section Compile and Run an OpenMP example$$
#
# $head Syntax$$
# $syntax%cd openmp
# %$$
# $syntax%./run.sh %name% %openmp% %optimize%$$
#
# $head Purpose$$
# The script file $code openmp/run.sh$$ compiles and runs one of the openmp
# example programs.
#
# $head name$$
# The argument $italic name$$ is the name of the example program.
# Currently the valid choice for $italic name$$ is 
# $cref/multi_newton/multi_newton.cpp/$$.
#
# $head openmp$$
# The argument $italic openmp$$ is either $code true$$ or $code false$$.
# If it is true, the program will be compiled with OpenMP multi-threading.
# Otherwise it is complied to run without multi-threading. Thus you
# can easily test the speed for both cases on your system.
#
# $head optimize$$
# The argument $italic optimize$$ is either $code true$$ or $code false$$.
# If it is true, the program will be compiled with optimization flags.
# Otherwise it is complied with debugging flags.
#
# $head Restrictions$$
# Current this script only runs under the bash shell; i.e., it will not
# run in an MSDOS box.
#
# $head Example$$
# The following is an example $code run.sh$$ command 
# $codep
#	./run.sh multi_newton false false
# $$
# The following is the corresponding output
# $codep
#	g++ -I.. multi_newton.cpp -o multi_newton  -g
#	./multi_newton
#	_OPENMP is not defined
#	running in single tread mode
#	Zeros found for sin(x) in the interval [0, 7*pi]
#	{ 0, 6.28319, 12.5664, 18.8496 }
#	size_vec = { 10, 20 }
#	rate_vec = { 48, 23 }
#	rm multi_newton.exe
# $$	
#
# $end
for flag in "$2" "$3"
do
	if [ "$flag" != "true"  ] && [ "$flag" != "false" ]
	then
		echo "usage: run.sh name openmp optimize"
		echo "name     the C++ file name wthout extension,"
		echo "openmp   true (use openmp) or false (do not use), and"
		echo "optimize true (optimized compile) or false (debugging)"
		exit 1
	fi
done
if [ "$2" == "true"  ]
then
	flags=-fopenmp
else
	flags=""
fi
if [ "$3" == "true" ]
then
	flags="$flags -DNDEBUG -O2"
else
	flags="$flags -g"
fi
#
echo "g++ -I.. $1.cpp -o $1 $flags"
g++ -I.. $1.cpp -o $1 $flags
#
echo "./$1"
./$1
#
if [ -e "$1.exe" ]
then
	echo "rm $1.exe"
	rm $1.exe
fi
if [ -e "$1" ]
then
	echo "rm $1"
	rm $1
fi
