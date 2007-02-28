#! /bin/bash
for flag in "$2" "$3"
do
	if [ "$flag" != "true"  ] && [ "$flag" != "false" ]
	then
		echo "usage: run.sh name openmp debug"
		echo "name   is the C++ file name wthout extension,"
		echo "openmp is true (use openmp) or false (do not use), and"
		echo "debug  is true (debugging compile) or flase (optimized)"
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
	flags="$flags -g"
else
	flags="$flags -DNDEBUG -O2"
fi
#
echo "g++ -I../.. $1.cpp -o $1 $flags"
g++ -I${HOME}/include $1.cpp -o $1 $flags
#
./$1
