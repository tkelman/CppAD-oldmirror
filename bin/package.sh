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
if [ $0 != "bin/package.sh" ]
then
	echo "bin/package.sh: must be executed from its parent directory"
	exit 1
fi
echo_log_eval() {
	echo $*
	echo $* >> $top_srcdir/package.log
	if ! eval $* >> $top_srcdir/package.log
	then
		echo "Error: check package.log"
		exit 1
	fi
}
log_eval() {
	echo $* >> $top_srcdir/package.log
	if ! eval $* >> $top_srcdir/package.log
	then
		echo "Error: check package.log"
		exit 1
	fi
}
if [ -e package.log ]
then
	echo "rm package.log"
	rm package.log
fi
top_srcdir=`pwd`
# ----------------------------------------------------------------------------
this_license=`\
	grep '$verbatim%' omh/license.omh | sed -e 's|$verbatim%\(...\).*|\1|'`
if [ "$this_license" == 'epl' ]
then
	remove_list='gpl-3.0.txt bin/gpl_license.sh'
elif [ "$this_license" == 'gpl' ]
then
	remove_list='epl-v10.txt epl-v10.html bin/gpl_license.sh'
else
	echo 'bin/package.sh: cannot find license in omh/license.omh'
	exit 1
fi
# ----------------------------------------------------------------------------
# Automated updates to source directory
#
# Get version number and make sure all copies of it are up to date.
version=`bin/version.sh get`
echo_log_eval bin/version.sh get
echo_log_eval bin/version.sh copy
# ----------------------------------------------------------------------------
# Run automated checking of file names in original source directory
# (check_include_omh.sh uses files built by cmake)
list="
	check_define.sh
	check_example.sh
	check_if_0.sh
	check_include_def.sh
	check_include_file.sh
	check_include_omh.sh
	check_makefile.sh
	check_op_code.sh
	check_replace.sh
	check_svn_id.sh
	check_verbatim.sh
"
for check in $list 
do
	echo_log_eval bin/$check
done
# ----------------------------------------------------------------------------
# Check for doxygen errors
echo_log_eval bin/run_doxygen.sh
# ----------------------------------------------------------------------------
# Create the package directory
package_dir="build/cppad-$version"
if [ -e "$package_dir" ]
then
	echo_log_eval rm -r $package_dir
fi
for lic in epl gpl
do
	if [ -e "$package_dir.epl.tgz" ]
	then
		echo_log_eval rm $package_dir.$lic.tgz
	fi
done
echo_log_eval mkdir -p $package_dir
# -----------------------------------------------------------------------------
# Source file that are coppied to the package directory
file_list=`find . \
	\( -name '*.ac' \) -or \
	\( -name '*.am' \) -or \
	\( -name '*.c' \) -or \
	\( -name '*.cmake' \) -or \
	\( -name '*.cpp' \) -or \
	\( -name '*.h' \) -or \
	\( -name '*.hpp' \) -or \
	\( -name '*.html' \) -or \
	\( -name '*.in' \) -or \
	\( -name '*.omh' \) -or \
	\( -name '*.pc' \) -or \
	\( -name '*.py' \) -or \
	\( -name '*.sed' \) -or \
	\( -name '*.sh' \) -or \
	\( -name '*.txt' \) | sed \
		-e '/\.\/build\//d' \
		-e '/bug\/build\//d' \
		-e '/\/junk.sh$/d' \
		-e '/\/temp.sh$/d' \
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
"
#
# Copy the files, creating sub-directories when necessary
echo_log_eval echo "copy files to $package_dir"
for file in $file_list $other_files
do
	sub_dir=`echo $file | sed -e 's|\(.*\)/[^/]*$|\1|'`
	if [ "$sub_dir" != "$file" ]
	then
		if [ ! -e "$package_dir/$sub_dir" ]
		then
			log_eval mkdir -p $package_dir/$sub_dir
		fi
	fi
	log_eval cp $file $package_dir/$file
done
echo_log_eval echo "remove certain files from $package_dir"
for file in $remove_list 
do
	if [ -e $package_dir/$file ]
	then
		echo_log_eval rm $package_dir/$file 
	fi
done
# ----------------------------------------------------------------------------
# build the xml version of documentation for this distribution
echo_log_eval cd $package_dir
#
# Only include the *.xml verison of the documentation in distribution
# So remove the table at the top (but save the original doc.omh file).
if ! grep < doc.omh > /dev/null \
	'This comment is used to remove the table below' 
then
	echo "Missing comment expected in doc.omh"
	exit 1
fi
echo_log_eval echo "sed -i.save doc.omh ..."
sed -i.save doc.omh \
	-e '/This comment is used to remove the table below/,/$tend/d'
#
# This command creates omhelp.xml.log in current directory (and says so)
echo_log_eval echo "bin/run_omhelp.sh xml"
if ! bin/run_omhelp.sh xml
then
	echo_log_eval cp omhelp.xml.log $top_srcdir/omhelp.xml.log
	exit 1
fi
# Copy the log to the directory where the package.sh command was executed
echo_log_eval cp omhelp.xml.log $top_srcdir/omhelp.xml.log
# Restore the original doc.omh
echo_log_eval mv doc.omh.save doc.omh
# ----------------------------------------------------------------------------
# change back to the package parent directory and create the tarball
echo_log_eval cd ..
echo_log_eval tar -czf cppad-$version.$this_license.tgz cppad-$version
# ----------------------------------------------------------------------------
# create gpl version of package
echo_log_eval cd $top_srcdir
if [ -e 'bin/gpl_license.sh' ]
then
	if [ "$this_license" != 'epl' ]
	then
		echo 'package.sh: bin/gpl_license.sh found in gpl verison of source.'
		exit 1
	fi
	echo_log_eval bin/gpl_license.sh cppad-$version build build
fi
