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
dir="cppad/local"
# -----------------------------------------------------------------------------
if [ $0 != "bin/speed_compare.sh" ]
then
	echo "bin/speed_compare.sh: must be executed from its parent directory"
	exit 1
fi
# Source code files that are changing (new version in $dir/new).
list=`cd $dir/new ; ls`
if [ "$list" == "" ] ; then
	echo "speed_compare.sh: $dir/new is empty"
	exit 1
fi
#
# check if we alread have results for current version
if [ ! -e speed_cur.out ]
then
	# Revert the source code to the current version
	for file in $list
	do
		echo "svn revert $dir/$file"
		svn revert $dir/$file
	done
fi
#
# compile and link the current version
echo "cd work/speed/src; make clean; make"
cd work/speed/src; make clean; make
#
echo "cd ../cppad; make clean; make test.sh"
cd ../cppad; make clean; make test.sh
#
# run speed test for the current version
echo "./cppad speed 123 retape > speed_cur.out"
./cppad speed 123 retape > speed_cur.out
#
dir="../../../$dir"
#
# Convert source to the new version
for file in $list
do
	echo "cp $dir/new/$file $dir/$file"
	cp $dir/new/$file $dir/$file
done
#
# compile and link the new version
echo "cd ../src; make clean; make"
cd ../src; make clean; make
#
echo "cd ../cppad; make clean; make test.sh"
cd ../cppad; make clean; make test.sh
#
# run speed test for the new version
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
