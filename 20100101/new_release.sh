# ! /bin/bash 
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
repository="https://projects.coin-or.org/svn/CppAD"
stable_version="20100101"
release="5"
release_version="$stable_version.$release"
msg="Creating releases/$release_version"
# -----------------------------------------------------------------------------
if svn list $repository/releases | grep "$release_version" > /dev/null
then
	echo "new_release.sh: Release number $release_version already exists."
	echo "In file new_release.sh, you must first change the assigment"
	echo "	release=$release"
	echo "to a higher release number."
	exit 1
fi
# -----------------------------------------------------------------------------
echo "svn revert configure.ac"
svn revert configure.ac
if ! grep "AC_INIT(CppAD.*, $release_version" configure.ac > /dev/null
then
	echo "new_release.sh: Change version number in configure.ac to be"
	echo "$release_version, then execute."
	echo "	./build.sh version automake config_none"
	echo "then commit the changes."
	exit 1
fi
echo "svn revert cppad/config.h"
svn revert cppad/config.h
if ! grep "PACKAGE_STRING.*CppAD.*$release_version" cppad/config.h > /dev/null
then
	echo "new_release.sh: Version in cppad/config.h is not $release_version"
	echo "	./build.sh version automake config_none"
	echo "should fix this"
	exit 1
fi
echo "svn revert cppad/configure.hpp"
svn revert cppad/configure.hpp
if ! grep "PACKAGE_STRING.*CppAD.*$release_version" \
	cppad/configure.hpp > /dev/null
then
	echo "new_release.sh: Version in cppad/sonfigure.hpp not $release_version"
	echo "	./build.sh version automake config_none"
	echo "should fix this"
	exit 1
fi
# -----------------------------------------------------------------------------
# check initial working directory
dir=`pwd | sed -e 's|.*/[Cc][Pp][Pp][Aa][Dd]/||'`
if [ "$dir" != "stable/$stable_version" ]
then
	echo "new_release.sh: can only execute in stable/$stable_version"
	exit 1
fi
# -----------------------------------------------------------------------------
rep_stable="$repository/stable/$stable_version"
rep_release="$repository/releases/$release_version"
echo "svn copy $rep_stable $rep_release -m \"$msg\""
if ! svn copy $rep_stable $rep_release -m "$msg"
then
	"Cannot create $rep_release"
	exit 1
fi
# -----------------------------------------------------------------------------
echo "cd ../.."
cd ../..
if [ -e conf ]
then
	echo "rm -rf conf"
	if ! rm -rf conf
	then
		echo "new_release.sh: cannot remove old conf directory"
		exit 1
	fi
fi
echo "svn checkout $repository/conf conf"
if ! svn checkout $repository/conf conf
then
	echo "new_release.sh: cannot checkout conf"
	exit 1
fi
echo "cd conf"
if ! cd conf
then
	echo "new_release.sh: cannot change into conf directory"
	exit 1
fi
echo "Change stable and release numbers in projDesc.xml"
sed -i projDesc.xml \
	-e "/^ *<stable/,/^ *<\/stable/s/[0-9]\{8\}/$stable_version/" \
	-e "/^ *<release/,/^ *<\/release/s/[0-9]\{8\}\.[0-9]*/$release_version/"
#
msg="Update stable and release numbers in conf/projDesc.xml"
echo "Use the command the following command to finish the process"
echo "	svn commit -m \"$msg\" ../conf/projDesc.xml"
