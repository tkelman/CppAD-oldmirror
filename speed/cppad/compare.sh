#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Source code directory that is changing.
dir="$HOME/cppad/branches/omp_alloc/cppad/local"
# Source code files that are changing (new version in $dir/new).
change_list="recorder.hpp player.hpp pod_vector.hpp"
# New source code files in $dir/new.
new_list=""
#
# check if we alread have results for current version
if [ ! -e speed_cur.out ]
then
	# Revert the source code to the current version
	if [ "$change_list" != "" ] ; then
		for file in $change_list
		do
			echo "svn revert $dir/$file"
			svn revert $dir/$file
		done
	fi
	#
	# compile and link the current version
	echo "pushd ../src; make clean; make; popd; make clean; make test.sh"
	pushd ../src; make clean; make; popd; make clean; make test.sh
	#
	# run speed test for the current version
	echo "./cppad speed 123 retape > speed_cur.out"
	./cppad speed 123 retape > speed_cur.out
fi
#
# Convert source to the new version
if [ "$new_list" != "" ] || [ "$change_list" != "" ] ; then
	for file in $change_list $new_list
	do
		echo "cp $dir/new/$file $dir/$file"
		cp $dir/new/$file $dir/$file
	done
fi
#
# compile and link the new current 
echo "pushd ../src; make clean; make; popd; make clean; make test.sh"
pushd ../src; make clean; make; popd; make clean; make test.sh
#
# run speed test for the current version
echo "./cppad speed 123 retape > speed_new.out"
./cppad speed 123 retape > speed_new.out
#
# compare versions
echo "sed -n -e 's|_rate|_rate_cur|' -e '/_rate_/p' < speed_cur.out > run.out"
sed -n -e 's|_rate|_rate_cur|' -e '/_rate_/p' < speed_cur.out > run.out
#
echo "sed -n -e 's|_rate|_rate_new|' -e '/_rate_/p' < speed_new.out >> run.out"
sed -n -e 's|_rate|_rate_new|' -e '/_rate_/p' < speed_new.out >> run.out
#
echo "cat run.out | sort -u"
cat run.out | sort -u
