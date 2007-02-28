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
