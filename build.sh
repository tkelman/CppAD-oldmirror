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
# prefix directories for the corresponding packages
BOOST_DIR=/usr/include
CPPAD_DIR=$HOME/prefix/cppad  
ADOLC_DIR=$HOME/prefix/adolc
FADBAD_DIR=$HOME/prefix/fadbad
SACADO_DIR=$HOME/prefix/sacado
IPOPT_DIR=$HOME/prefix/ipopt
# library path for the ipopt and adolc
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$ADOLC_DIR/lib:$IPOPT_DIR/lib"
# version type is one of "trunk" or "stable"
version_type="stable"
# -----------------------------------------------------------------------------
if [ $0 != "./build.sh" ]
then
	echo "./build.sh: must be executed in the directory that contians it"
	exit 1
fi
if [ "$2" != "" ]
then
	# when running multiple options, start by removing old log files
	touch junk.log
	list=`ls *.log`
	for log in $list
	do
		echo "rm $log"
		rm $log
	done
	#
	# run multiple options in order
     for option in $*
     do
		echo "=============================================================="
		echo "begin: ./build.sh $option"
          ./build.sh $option
     done
	echo "=============================================================="
     exit 0
fi
# -----------------------------------------------------------------------------
if [ ! -e work ]
then
	echo "mkdir work"
	mkdir work
fi
# -----------------------------------------------------------------------------
# Today's date in yyyy-mm-dd decimal digit format where 
# yy is year in century, mm is month in year, dd is day in month.
yyyy_mm_dd=`date +%F`
#
# Version of cppad that corresponds to today.
if [ "$version_type" == "trunk" ]
then
	version=`echo $yyyy_mm_dd | sed -e 's|-||g'`
else
	version=`grep '^ *AC_INIT(' configure.ac | 
		sed -e 's/[^,]*, *\([^ ,]*\).*/\1/'`
	yyyy_mm_dd=`echo $version | 
		sed -e 's|\..*||' -e 's|\(....\)\(..\)|\1-\2-|'`
fi
#
# Files are created by the configure command and copied to the source tree
# (This list is also in the gpl_license.sh file.)
configure_file_list="
	cppad/config.h
	cppad/configure.hpp
	doc.omh
	doxyfile
	example/test_one.sh
	omh/install_unix.omh
	omh/install_windows.omh
	test_more/test_one.sh
"
# -----------------------------------------------------------------------------
# change version to current date
if [ "$1" = "version" ]
then
	#
	# automatically change version for certain files
	# (the [.0-9]* is for using build.sh in CppAD/stable/* directories)
	#
	# libtool does not seem to support version by date
	# sed < cppad_ipopt/src/makefile.am > cppad_ipopt/src/makefile.am.$$ \
	#	-e "s/\(-version-info\) *[0-9]\{8\}[.0-9]*/\1 $version/"
	#
	#
	list="
		configure
		cppad/config.h
	"
	for name in $list
	do
		svn revert $name
	done
	
	echo "sed -i.old AUTHORS ..."
	sed -i.old AUTHORS \
		-e "s/, [0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\} *,/, $yyyy_mm_dd,/"
	#
	echo "sed -i.old configure.ac ..."
	sed -i.old configure.ac \
		-e "s/(CppAD, [0-9]\{8\}[.0-9]* *,/(CppAD, $version,/" 
	#
	echo "sed -i.old configure ..."
	sed -i.old configure \
		-e "s/CppAD [0-9]\{8\}[.0-9]*/CppAD $version/g" \
		-e "s/VERSION='[0-9]\{8\}[.0-9]*'/VERSION='$version'/g" \
		-e "s/configure [0-9]\{8\}[.0-9]*/configure $version/g" \
		-e "s/config.status [0-9]\{8\}[.0-9]*/config.status $version/g" \
		-e "s/\$as_me [0-9]\{8\}[.0-9]*/\$as_me $version/g" \
        	-e "s/Generated by GNU Autoconf.*$version/&./"
	#
	echo "sed -i.old cppad/config.h ..."
	sed -i.old cppad/config.h \
		-e "s/CppAD [0-9]\{8\}[.0-9]*/CppAD $version/g" \
		-e "s/VERSION \"[0-9]\{8\}[.0-9]*\"/VERSION \"$version\"/g"
	list="
		AUTHORS
		configure.ac
		configure
		cppad/config.h
	"
	for name in $list
	do
		echo "-------------------------------------------------------------"
		echo "diff $name.old $name"
		if diff $name.old $name
		then
			echo "	no difference was found"
		fi
		#
		echo "rm $name.old"
		rm $name.old
	done
	echo "-------------------------------------------------------------"
	#
	echo "OK: ./build.sh version"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ "$1" = "automake" ] 
then
	#
	# check that autoconf and automake output are in original version
	makefile_in=`sed configure.ac \
        	-n \
        	-e '/END AC_CONFIG_FILES/,$d' \
        	-e '1,/AC_CONFIG_FILES/d' \
        	-e 's|/makefile$|&.in|' \
        	-e '/\/makefile.in/p'`
	auto_output="
		depcomp 
		install-sh 
		missing 
		configure 
		cppad/config.h 
		cppad/config.h.in 
		$makefile_in
	"
	missing=""
	for name in $auto_output
	do
		if [ ! -e $name ]
		then
			if [ "$missing" != "" ]
			then
				missing="$missing, $name"
			else
				missing="$name"
			fi
		fi
	done
	if [ "$missing" != "" ]
	then
		echo "The following files:"
		echo "	$missing"
		echo "are not in subversion repository."
		echo "Check them in when this command is done completes."
	fi
	#
	echo "aclocal"
	aclocal
	#
	echo "autoheader"
	autoheader
	#
	echo "skipping libtoolize"
	# echo "libtoolize -c -f -i"
	# if ! libtoolize -c -f -i
	# then
	# 	exit 1
	# fi
	#
	echo "autoconf"
	autoconf
	#
	echo "automake --add-missing"
	automake --add-missing
	#
	link_list="missing install-sh depcomp"
	for name in $link_list
	do
		if [ -h "$name" ]
		then
			echo "Converting $name from a link to a regular file"
			#
			echo "cp $name $name.$$"
			cp $name $name.$$
			#
			echo "mv $name.$$ $name"
			mv $name.$$ $name
		fi
	done
	#
	echo "OK: ./build.sh automake"
	exit 0
fi
# -----------------------------------------------------------------------------
# configure
if [ "$1" == "configure" ]
then
	echo "cd work"
	cd work
	#
	dir_list="
		--prefix=$CPPAD_DIR
	"
	if [ -e $BOOST_DIR/boost ]
	then
		dir_list="$dir_list 
			BOOST_DIR=$BOOST_DIR"
	fi
	if [ -e $ADOLC_DIR/include/adolc ]
	then
		dir_list="$dir_list 
			ADOLC_DIR=$ADOLC_DIR"
	fi
	if [ -e $FADBAD_DIR/FADBAD++ ]
	then
		dir_list="$dir_list 
			FADBAD_DIR=$FADBAD_DIR"
	fi
	if [ -e $SACADO_DIR/include/Sacado.hpp ]
	then
		dir_list="$dir_list 
			SACADO_DIR=$SACADO_DIR"
	fi
	if [ -e $IPOPT_DIR/include/coin/IpIpoptApplication.hpp ]
	then
		dir_list="$dir_list 
		IPOPT_DIR=$IPOPT_DIR"
	fi
	dir_list=`echo $dir_list | sed -e 's|\t\t*| |g'`
	echo "../configure \\"
	echo "$dir_list" | sed -e 's| | \\\n\t|g' -e 's|$| \\|' -e 's|^|\t|'
	echo "CXX_FLAGS=\"-Wall -ansi -pedantic-errors -std=c++98 -Wshadow\"\\"
	echo "--with-Documentation"
	#
	../configure $dir_list \
		CXX_FLAGS="-Wall -ansi -pedantic-errors -std=c++98 -Wshadow" \
		--with-Documentation
	#
	for file in $configure_file_list
	do
		echo "cp $file ../$file"
		cp $file ../$file
	done
	#
	echo "OK: ./build.sh configure"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ "$1" = "dist" ] 
then
	# ----------------------------------------------------------------------
	# Things to do in the distribution directory
	# ----------------------------------------------------------------------
	echo "Only include the *.xml version of the documentation in distribution"
	if ! grep < doc.omh > /dev/null \
		'This comment is used to remove the table below' 
	then
		echo "Missing comment expected in doc.omh"
		echo "Try re-running build.sh configure to generate it."
		exit 1
	fi
	echo "sed -i.save doc.omh ..."
	sed -i.save doc.omh \
		-e '/This comment is used to remove the table below/,/$tend/d'
	#
	if [ -e doc ]
	then
		echo "rm -r doc"
		rm -r doc
	fi
	#
	echo "bin/run_omhelp.sh doc xml"
	      bin/run_omhelp.sh doc xml
	#
	echo "mv doc.omh.save doc.omh"
	mv doc.omh.save doc.omh
	# ----------------------------------------------------------------------
	# Things to do in the work directory
	# ----------------------------------------------------------------------
	echo "cd work"
	cd work
	#
	if [ -e cppad-$version ]
	then
		echo "rm -rf cppad-$version"
		rm -rf cppad-$version
	fi
	for file in cppad-*.tgz 
	do
		if [ -e $file ]
		then
			echo "rm $file"
			rm $file
		fi
	done
	#
	echo "make dist"
	make dist
	#
	if [ ! -e cppad-$version.tar.gz ]
	then
		echo "cppad-$version.tar.gz does not exist"
		echo "perhaps version is out of date"
		#
		exit 1
	fi
	# change *.tgz to *.cpl.tgz
	echo "mv cppad-$version.tar.gz cppad-$version.cpl.tgz"
	mv cppad-$version.tar.gz cppad-$version.cpl.tgz
	#
	echo "OK: ./build.sh dist"
	exit 0
fi
# -----------------------------------------------------------------------------
# omhelp comes after dist becasue dist only includes one help output
if [ "$1" = "omhelp" ] 
then
	if ! grep < doc.omh > /dev/null \
		'This comment is used to remove the table below'
	then
		echo "doc.omh is missing a table."
		echo "Try re-running build.sh configure."
	fi
	for flag in "printable" ""
	do
		for ext in htm xml
		do
			echo "begin: bin/run_omhelp.sh doc $ext $flag"
			             bin/run_omhelp.sh doc $ext $flag
			echo="end:   bin/run_omhelp.sh doc $ext $flag"
		done
	done
	#
	echo "OK: ./build.sh omhelp"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ "$1" = "doxygen" ]
then
	if [ -e doxygen.err ]
	then
		echo "rm doxygen.err"
		rm doxygen.err
	fi
	#
	if [ -e doxydoc ]
	then
		echo "rm -r doxydoc"
		rm -r doxydoc
	fi
	#
	echo "mkdir doxydoc"
	mkdir doxydoc
	#
	echo "doxygen doxyfile"
	doxygen doxyfile
	#
	echo "cat doxygen.err"
	cat doxygen.err 
	#
	echo "bin/check_doxygen.sh"
	bin/check_doxygen.sh
	#
	echo "OK: ./build.sh doxygen"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ "$1" = "gpl" ] 
then
	# create GPL licensed version
	echo "bin/gpl_license.sh"
	bin/gpl_license.sh
	#
	echo "OK: ./build.sh gpl"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ "$1" = "copy2doc" ] 
then
	for ext in cpl gpl
	do
		echo "cp work/cppad-$version.$ext.tgz doc/cppad-$version.$ext.tgz"
		cp work/cppad-$version.$ext.tgz doc/cppad-$version.$ext.tgz
	done
	echo "cp -r doxydoc doc/doxydoc"
	cp -r doxydoc doc/doxydoc
	#
	echo "OK: ./build.sh copy2doc"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ "$1" == "all" ]
then
	list="
		version
		automake
		configure
		dist
		omhelp
		doxygen
		gpl
		copy2doc
	"
	if [ "$version_type" != "trunk" ]
	then
		# only use the help built during the build.sh dist command
		list=`echo $list | sed -e 's|omhelp||'`
	fi
	echo "./build.sh $list"
	./build.sh $list
	echo "OK: ./build.sh all"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ "$1" = "test" ] 
then
	log_dir=`pwd`
	log_file="build_test.log"
	# --------------------------------------------------------------
	# Things to do in the distribution directory
	# --------------------------------------------------------------
	#
	# start log for this test
	echo "date > $log_file"
	      date > $log_dir/$log_file
	#
	# Run automated checking of file names in original source directory
	#
	list="
		check_example.sh
		check_if_0.sh
		check_include_def.sh
		check_include_file.sh
		check_include_omh.sh
		check_makefile.sh
	"
	for check in $list 
	do
		echo "bin/$check >> $log_file"
		      bin/$check >> $log_dir/$log_file
	done
	# add a new line after last file check
	echo ""             >> $log_dir/$log_file
	# ----------------------------------------------------------------------
	# Things to do in the work directory
	# ----------------------------------------------------------------------
	echo "cd work"
	echo "cd work" >> $log_dir/$log_file
	      cd work
	#
	# erase old distribution directory
	if [ -e cppad-$version ]
	then
		echo "rm -rf cppad-$version"
		echo "rm -rf cppad-$version" >> $log_dir/$log_file
		      rm -rf cppad-$version
	fi
	#
	# create distribution directory
	echo "tar -xzf cppad-$version.cpl.tgz"
	echo "tar -xzf cppad-$version.cpl.tgz" >> $log_dir/$log_file
	      tar -xzf cppad-$version.cpl.tgz
	#
	# ----------------------------------------------------------------------
	# Things to do in the work/disribution directory
	# ----------------------------------------------------------------------
	echo "cd cppad-$version"
	echo "cd cppad-$version" >> $log_dir/$log_file
	      cd cppad-$version
	#
	echo "./build.sh configure >> $log_file" 
	      ./build.sh configure >> $log_dir/$log_file
	#
	# test user documentation
	echo "bin/run_omhelp.sh doc xml  >> $log_file"
	      bin/run_omhelp.sh doc xml  >> $log_dir/$log_file
	# 
	# test developer documentation
	echo "./build.sh doxygen   >> $log_file"
	      ./build.sh doxygen   >> $log_dir/$log_file
	#
	# openmp test script
	echo "openmp/run.sh       >> $log_file"
	      openmp/run.sh       >> $log_dir/$log_file
	# ----------------------------------------------------------------------
	# Things to do in the work/disribution/work directory
	# ----------------------------------------------------------------------
	echo "cd work"
	echo "cd work" >> $log_dir/$log_file
	      cd work
	#
	dir=`pwd` 
	echo "Use: tail -f $dir/make_test.log"
	echo "to follow the progress of the following command:"
	#
	# build and run all the tests
	echo "make test                >& make_test.log"
	      make test                >& make_test.log
	#
	echo "cat make_test.log        >> $log_file"
	      cat make_test.log        >> $log_dir/$log_file
	#
	if grep 'warning:' make_test.log
	then
		echo "There are warnings in $dir/make.log"
		exit 1
	fi
	# --------------------------------------------------------------------
	echo "cd ../../.."
	cd ../../..
	# end the build_test.log file with the date and time
	echo "date >> $log_file"
	      date >> $log_dir/$log_file
	#
	echo "No errors or warnings found; see build_test.log."
	#
	echo "OK: ./build.sh test"
	exit 0
fi
# -----------------------------------------------------------------------------
# report build.sh usage error
if [ "$1" != "" ]
then
     echo "$1 is not a valid option"
fi
#
if [ "$version_type" == "trunk" ]
then
	all_cases="run all the options above in order"
else
	all_cases="run all the options above in order with exception of omhelp"
fi
cat << EOF
usage: ./build.sh option_1 option_2 ...

options                                                               requires
-------                                                               --------
version:   set version in AUTHORS, configure.ac, configure, config.h.
automake:  run the tools required by autoconf and automake.
configure: run the configure script in the work directory.            automake
dist:      create the distribution file work/cppad-version.cpl.tgz.   configure
omhelp:    build all formats of user documentation in doc/*.          configure
doxygen:   build developer documentation in doxydoc/*.                configure
gpl:       create work/*.gpl.zip and work/*.cpl.zip.                  dist
copy2doc:  copy tarballs and doxygen output into doc directory.       gpl

all:       $all_cases
test:      use tarball to make test and put result in build_test.log. dist
EOF
#
exit 1
