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
if [ $0 != "bin/package.sh" ]
then
	echo "bin/package.sh: must be executed from its parent directory"
	exit 1
fi
echo_exec() {
     echo $* 
     eval $*
}
echo_exec_log() {
	echo "$* >> package.log"
	echo    >> $top_srcdir/package.log
	eval $* >> $top_srcdir/package.log
}
top_srcdir=`pwd`
if [ -e package.log ]
then
	echo_exec rm package.log
fi
# ----------------------------------------------------------------------------
version=`bin/version.sh get`
echo_exec bin/version.sh set
# ----------------------------------------------------------------------------
# Run automated checking of file names in original source directory
# (check_include_omh.sh uses files built by cmake)
list="
	check_example.sh
	check_if_0.sh
	check_include_def.sh
	check_include_file.sh
	check_include_omh.sh
	check_makefile.sh
	check_op_code.sh
	check_svn_id.sh
	check_verbatim.sh
"
for check in $list 
do
	echo_exec bin/$check
done
# ----------------------------------------------------------------------------
# Create the package directory
package_dir="work/cppad-$version"
if [ -e "$package_dir" ]
then
	echo_exec rm -r $package_dir
fi
if [ -e "$package_dir.epl.tgz" ]
then
	echo_exec rm $package_dir.epl.tgz
fi
echo_exec mkdir -p $package_dir
# -----------------------------------------------------------------------------
# Source file that are coppied to the package directory
file_list=`find . \
	\( -name '*.ac' \) -or \
	\( -name '*.am' \) -or \
	\( -name '*.c' \) -or \
	\( -name '*.cpp' \) -or \
	\( -name '*.h' \) -or \
	\( -name '*.hpp' \) -or \
	\( -name '*.in' \) -or \
	\( -name '*.omh' \) -or \
	\( -name '*.pc' \) -or \
	\( -name '*.py' \) -or \
	\( -name '*.sh' \) -or \
	\( -name '*.txt' \) | sed \
		-e '/\.\/work\//d' \
		-e '/\.\/build\//d' \
		-e '/\.\/gpl_license.sh\//d' \
		-e '/\.\/gpl-3.0.txt\//d' \
		-e 's|^\./||'`
other_files="
	AUTHORS 
	ChangeLog 
	configure 
	COPYING 
	depcomp
	INSTALL 
	NEWS 
	README 
	uw_copy_040507.html
"
#
# Copy the files, creating sub-directories when necessary
echo "copy source files to work/$package_dir >> package.log"
for file in $file_list $other_files
do
	sub_dir=`echo $file | sed -e 's|\(.*\)/[^/]*$|\1|'`
	if [ "$sub_dir" != "$file" ]
	then
		if [ ! -e "$package_dir/$sub_dir" ]
		then
			echo "mkdir -p $package_dir/$sub_dir" >> package.log
			mkdir -p $package_dir/$sub_dir
		fi
	fi
	echo "cp $file $package_dir/$file" >> package.log
	cp $file $package_dir/$file
done
# ----------------------------------------------------------------------------
# build the xml version of documentation for this distribution
echo_exec mkdir $package_dir/doc
echo_exec cd $package_dir/doc
#
cmd='omhelp ../doc.omh -noframe -debug -xml -l http://www.coin-or.org/CppAD/'
echo "$cmd > omhelp.xml.log"
if ! eval $cmd > $top_srcdir/omhelp.xml.log
then
	grep '^OMhelp Error:' $top_srcdir/omhelp.xml.log
	echo 'OMhelp could not build the CppAD xml documnentation.'
	echo 'See the complete error message in omhelp.xml.log'
	exit 1
fi
if grep '^OMhelp Warning:' $top_srcdir/omhelp.xml.log
then
	echo 'See the complete warning message in omhelp.xml.log'
	exit 1
fi
# ----------------------------------------------------------------------------
# change back to the package parent directory and create the tarball
echo_exec cd ../..
echo_exec tar -czf cppad-$version.epl.tgz cppad-$version
# ----------------------------------------------------------------------------
# create gpl version of package
echo_exec cd ..
echo_exec bin/gpl_license.sh