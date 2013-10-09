#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# $begin get_eigen.sh$$ $newlinech #$$
# $spell
#	Eigen
#	CppAD
# $$
#
# $section Download and Install Eigen in Build Directory$$
# $index eigen, download and install$$
# $index download, install eigen$$
# $index install, eigen$$ 
#
# $head Syntax$$
# $code bin/get_eigen.sh$$
#
# $head Purpose$$
# If you are using Unix, this command will download and install 
# $href%http://eigen.tuxfamily.org%Eigen%$$ in the
# CppAD $code build$$ directory.
#
# $head Distribution Directory$$
# This command must be executed in the 
# $cref/distribution directory/download/Distribution Directory/$$.
#
# $head External Directory$$
# The Eigen source code is downloaded into the sub-directory
# $code build/external$$ below the distribution directory.
#
# $head Prefix Directory$$
# The Eigen include files are installed in the sub-directory
# $code build/prefix/include/EIGEN$$ below the distribution directory.
#
# $end
# -----------------------------------------------------------------------------
if [ $0 != "bin/get_eigen.sh" ]
then
	echo "bin/get_eigen.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
echo 'Download eigen to build/external and install it to build/prefix'
version='3.1.3'
web_page='https://bitbucket.org/eigen/eigen/get'
prefix=`pwd`'/build/prefix'
# -----------------------------------------------------------------------------
if [ ! -d external ]
then
	echo_eval mkdir -p build/external
fi
echo_eval cd build/external
# -----------------------------------------------------------------------------
if [ ! -e "eigen-$version.tar.gz" ]
then
	echo_eval wget --no-check-certificate $web_page/$version.tar.gz
	echo_eval mv $version.tar.gz eigen-$version.tar.gz
fi
if [ -e eigen-eigen-* ]
then
	echo_eval rm -r eigen-eigen-*
fi
if [ -e "eigen-$version" ]
then
	echo_eval rm -r eigen-$version
fi
if [ -e "$prefix/include/Eigen" ]
then
	echo_eval rm "$prefix/include/Eigen"
fi
echo_eval tar -xzf eigen-$version.tar.gz
#
git_name=`ls | grep eigen-eigen`
echo_eval mv $git_name eigen-$version
#
echo_eval cd eigen-$version
echo_eval mkdir build
echo_eval cd build
echo_eval cmake .. -DCMAKE_INSTALL_PREFIX=$prefix
echo_eval make install
echo_eval ln -s $prefix/include/eigen3/Eigen $prefix/include/Eigen
#
echo "get_eigen.sh: OK"
