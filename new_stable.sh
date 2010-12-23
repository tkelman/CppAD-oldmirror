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
copy_from_trunk="keep"     # do (frist time), keep (use current), redo
trunk_revision="1615"      # trunk revision number that stable corresponds to
yyyy_mm_dd="2010-01-01"    # Date corresponding to this trunk revision
# -----------------------------------------------------------------------------
echo "copy_from_trunk=$copy_from_trunk"
echo "trunk_revision=$trunk_revision"
echo "yyyy_mm_dd=$yyyy_mm_dd"
echo
stable_revision=`expr $trunk_revision + 1`
stable_version=`echo $yyyy_mm_dd | sed -e 's/-//g'`
release_version="$stable_version.0"
repository="https://projects.coin-or.org/svn/CppAD"
rep_trunk="$repository/trunk"
rep_stable="$repository/stable/$stable_version"
#
# check initial working directory
dir=`pwd | sed -e 's|.*/||'`
if [ "$dir" != "trunk" ]
then
	echo "new_stable.sh: must execute this script in the trunk"
	exit 1
fi
echo "cd .."
cd ..
dir=`pwd`
if [ "$copy_from_trunk" = "redo" ] 
then
	# delete old copy of branch
	msg="Replacing old stable/$stable_version."
	echo "svn delete $rep_stable -m \"$msg\""
	if ! svn delete $rep_stable -m "$msg"
	then
		"new_stable.sh: Cannot remove $rep_stable"
		exit 1
	fi
fi
if [ "$copy_from_trunk" = "do" ] || [ "$copy_from_trunk" = "redo" ]
then
	#
	# create the new stable version
	temp_1="Create stable/$stable_version"
	temp_2="from trunk revision $trunk_revision."
	msg="$temp_1 $temp_2"
	echo "svn copy -r $trunk_revision $rep_trunk $rep_stable -m \"$msg\""
	if ! svn copy -r $trunk_revision $rep_trunk $rep_stable -m "$msg"
	then
		"new_stable.sh: Cannot create $rep_stable"
		exit 1
	fi
	if [ ! -d stable ]
	then
		if ! mkdir stable
		then
			echo "new_stable.sh: Cannot create $dir/stable"
			exit 1
		fi
	fi
fi
if [ -e "stable/$stable_version" ]
then
	echo "rm -rf stable/$stable_version"
	if ! rm -rf stable/$stable_version
	then
		echo "new_stable.sh: cannot remove old stable version"
		exit 1
	fi
fi
#
# retrieve stable version from repository ------------------------------------
#
echo "svn checkout --quiet -r $stable_revision $rep_stable stable/$stable_version"
if ! svn checkout --quiet -r $stable_revision $rep_stable stable/$stable_version
then
	echo "new_stable.sh: cannot checkout stable/$stable_version"
	exit 1
fi
#
# make sure that new_stable.sh corresponds to this version 
# (may not be same as verion in repository that was copied).
echo "trunk/new_stable.sh stable/$stable_version/new_stable.sh"
if ! cp trunk/new_stable.sh stable/$stable_version/new_stable.sh
then
	echo "new_stable.sh: cannot copy new_stable.sh"
	exit 1
fi
#
echo "cd stable/$stable_version"
if ! cd stable/$stable_version
then
	echo "new_stable.sh: cannot change into $dir/stable/$stable_version"
	exit 1
fi
#
# Automatic editing ------------------------------------------------ 
#
# set the first release version number for this stable version
sed -i new_release.sh \
	-e "s/stable_version=.*/stable_version=\"$stable_version\"/"
#
# Set the stable version number to the one in configure.ac.
# Fix the date at the original stable date.
# Remove "doxygen" and "omhelp" from "all" cases.
sed -i build.sh \
     -e 's/yyyymmdd=.*/yyyymmdd="$configure_ac_version"/' \
     -e "s/yyyy_mm_dd=.*/yyyy_mm_dd=\"$yyyy_mm_dd\"/"  \
     -e '/echo *".[/]build.sh all *"/,/^[\t ]*" *$/s/^[\t ]*omhelp *$//' \
     -e '/echo *".[/]build.sh all *"/,/^[\t ]*" *$/s/^[\t ]*doxygen *$//' \
     -e '/echo *".[/]build.sh all dos *"/,/^[\t ]*" *$/s/^[\t ]*omhelp *$//' \
     -e '/echo *".[/]build.sh all dos *"/,/^[\t ]*" *$/s/^[\t ]*doxygen *$//' \
     -e '/echo *".[/]build.sh all test *"/,/^[\t ]*" *$/s/^[\t ]*omhelp *$//' \
     -e '/echo *".[/]build.sh all test *"/,/^[\t ]*" *$/s/^[\t ]*doxygen *$//' 
#
# use web for download of this release version
dir="http://www.coin-or.org/download/source/CppAD"
sed -i omh/install_windows.omh.in \
	-e "s|cppad-@VERSION@.[cg]pl.tgz|\n$dir/&%\n&|" 
#
# use web for dowload of this release version
sed -i omh/install_unix.omh.in \
	-e "s|cppad-@VERSION@.[cg]pl.tgz|\n$dir/&%\n&|" 
#
# only build one version of the documentation in the release
sed -i doc.omh.in \
	-e '/This comment is used to remove the table below/,/$tend/d'
#
# Instructions --------------------------------------------------------------
#
echo "1: In the directory stable/$stable_version, review differences using"
echo "       ./commit.sh files"
echo "   All changed files should be present. Check differences.
echo "2: If not correct, fix trunk/new_stable.sh and re-run it and goto 1."
echo "3: In stable/$stable_version run the following command:"
echo "      ./build.sh all test"
echo "4: If errors occur, fix trunk/new_stable.sh and goto 1."
echo "5: Commit changes to trunk/new_stable.sh."
echo "6: In stable/$stable_version commit changes using"
echo "      ./commit.sh run"
echo "7: Make sure commited all necessary changes in stable/$stable_version"
echo "      ./commit.sh files"
echo "8: In stable/$stable_version check first, then run the script"
echo "      ./new_release.sh"	
echo
exit 0
