#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/only_date.sh" ]
then
	echo "bin/only_date.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
version=`cat configure.ac | grep "^ *AC_INIT(" | 
        sed -e 's/[^,]*, *\([^ ,]*\).*/\1/'`
yyyy_mm_dd=`echo $version | sed -e 's/\(....\)\(..\)/\1-\2-/'`
# ---------------------------------------------------------------------
function check_difference
{
	sed -e '/$Id:.*$/d' $1 > bin/only_date.2.$$
	if diff bin/only_date.1.$$ bin/only_date.2.$$ > /dev/null
	then
		printf "%-20s at most the version number is different.\n" $1:
	else
		printf "%-20s changes not counting version number:\n" $1:
		if ! diff bin/only_date.1.$$ bin/only_date.2.$$
		then
			echo "bin/only_date.1.sh: program error"
			rm bin/only_date.1.$$
			rm bin/only_date.2.$$
			exit 1
		fi
	fi
	rm bin/only_date.1.$$
	rm bin/only_date.2.$$
}
# ---------------------------------------------------------------------
svn cat AUTHORS | sed > bin/only_date.1.$$ \
	-e "s/, [0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\} *,/, $yyyy_mm_dd,/" \
	-e '/$Id:.*$/d'
check_difference AUTHORS
# ---------------------------------------------------------------------
svn cat configure | sed > bin/only_date.1.$$ \
	-e "s/CppAD [0-9]\{8\}[.0-9]*/CppAD $version/g" \
	-e "s/VERSION='[0-9]\{8\}[.0-9]*'/VERSION='$version'/g" \
	-e "s/configure [0-9]\{8\}[.0-9]*/configure $version/g" \
	-e "s/config.status [0-9]\{8\}[.0-9]*/config.status $version/g" \
	-e "s/\$as_me [0-9]\{8\}[.0-9]*/\$as_me $version/g" \
	-e "s/Generated by GNU Autoconf.*$version/&./" \
	-e '/$Id:.*$/d'
check_difference configure
# ---------------------------------------------------------------------
svn cat configure.ac | sed > bin/only_date.1.$$ \
     -e "s/(CppAD, [0-9]\{8\}[.0-9]* *,/(CppAD, $version,/" \
	-e '/$Id:.*$/d'
check_difference configure.ac
# ---------------------------------------------------------------------
# configure.hpp is a special case beacuse it is changed by make, no configure
change=`svn diff cppad/configure.hpp`
if [ "$change" != "" ]
then
	svn cat cppad/configure.hpp | sed > bin/only_date.1.$$ \
		-e "s/cppad-[0-9]\{8\}[.0-9]*/cppad-$version/g" \
		-e '/$Id:.*$/d'
	check_difference cppad/configure.hpp
fi
# ---------------------------------------------------------------------
