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
release="1"
release_version="$stable_version.$release"
msg="Creating releases/$release_version"
# -----------------------------------------------------------------------------
if ! grep "AC_INIT(CppAD.*, $stable_version.$release" configure.ac >> /dev/null
then
	echo "Must change version number in configure.ac,"
	echo "then run build.sh all test, and check in changes."
	exit 1
fi
# -----------------------------------------------------------------------------
#
# check initial working directory
dir=`pwd | sed -e 's|.*/||'`
if [ "$dir" != "trunk" ]
then
	echo "new_release.sh: must execute this script in the trunk"
	exit 1
fi
echo "cd .."
cd ..
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
msg="Update stabel and release numbers in conf/projDesc.xml"
echo "Use the command the following command to finish the process"
echo "	svn commit -m \"$msg\" ../conf/projDesc.xml"
