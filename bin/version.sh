#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != 'bin/version.sh' ]
then
	echo 'bin/version.sh: must be executed from its parent directory'
	exit 1
fi
if [ "$1" != 'get' ] && [ "$1" != 'set' ] && [ "$1" != 'copy' ]
then
	echo 'usage: bin/version.sh (get | set | copy) [version]'
	echo 'get:  Gets the current version number from CMakeLists.txt.'
	echo 'set:  Sets CMakeLists.txt version number to version.'
	echo '      If version is not present, uses current yyyymmdd.'
	echo 'copy: Copies version number from CMakeLists.txt to other files.'
	exit 1
fi
echo_exec() {
     echo $* 
     eval $*
}
# -----------------------------------------------------------------------------
if [ "$1" == 'set' ]
then
	if [ "$2" == '' ]
	then
		version=`date +%Y%m%d`
	else
		version="$2"
	fi
	echo 'sed -i.old CMakeLists.txt ...'
	sed  \
	-e "s/(\(cppad_version *\)\"[0-9.]\{8\}[0-9.]*\" *)/(\1\"$version\" )/"  \
		-i.old CMakeLists.txt
	if diff CMakeLists.txt CMakeLists.txt.old
	then
		echo 'No change in CMakeLists.txt'
		exit 1
	fi
	echo 'bin/version.sh set: OK'
	exit 0
fi
# -----------------------------------------------------------------------------
# get the current version number
version=`grep '^SET *( *cppad_version ' CMakeLists.txt | \
	sed -e 's|^SET *( *cppad_version *"\([0-9.]\{8\}[0-9.]*\)" *)|\1|'`
if ! (echo $version | grep '[0-9]\{8\}') > /dev/null
then
	echo 'package.sh: Cannot find verison number in CMakeLists.txt'
	exit 1
fi 
if [ "$1" == 'get' ]
then
	echo "$version"
	exit 0
fi
# -----------------------------------------------------------------------------
# Make the version number in the relevant files is the same
yyyy_mm_dd=`echo $version | sed \
	-e 's|\([0-9]\{4\}\)0000|\10101|' \
	-e 's|\(....\)\(..\)\(..\).*|\1-\2-\3|'`
echo 'sed -i.old AUTHORS ...'
sed  \
	-e "s/, [0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\} *,/, $yyyy_mm_dd,/" \
	-i.old AUTHORS
#
echo 'sed -i.old configure.ac ...'
sed  \
	-e "s/(cppad, [0-9]\{8\}[.0-9]* *,/(cppad, $version,/"  \
	-i.old configure.ac
#
echo 'sed -i.old configure ...'
sed \
	-e "s/cppad [0-9]\{8\}[.0-9]*/cppad $version/g" \
	-e "s/VERSION='[0-9]\{8\}[.0-9]*'/VERSION='$version'/g" \
	-e "s/configure [0-9]\{8\}[.0-9]*/configure $version/g" \
	-e "s/config.status [0-9]\{8\}[.0-9]*/config.status $version/g" \
	-e "s/\$as_me [0-9]\{8\}[.0-9]*/\$as_me $version/g" \
	-e "s/Generated by GNU Autoconf.*$version/&./" \
	-i.old configure 
list='
	doc.omh
	omh/install/unix.omh
	omh/install/download.omh
'
for file in $list
do
	sed -e "s/cppad-[0-9]\{8\}[0-9.]*/cppad-$version/" -i.old $file
done 
list="
	$list
	AUTHORS
	configure.ac
	configure
"
for name in $list
do
	echo '-------------------------------------------------------------'
	echo "diff $name.old $name"
	if diff $name.old $name
	then
		echo '	no difference was found'
	fi
	#
	echo_exec rm $name.old
done
echo 'bin/version.sh copy: OK'
