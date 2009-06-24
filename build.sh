# ! /bin/bash 
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
# Bash script for building the CppAD distribution.
#
# Default values used for arguments to configure during this script.
# These defaults are development system dependent and can be changed.
BOOST_DIR=/usr/include/boost-1_33_1
ADOLC_DIR=$HOME/prefix/adolc
FADBAD_DIR=$HOME/prefix/fadbad
SACADO_DIR=$HOME/prefix/sacado
IPOPT_DIR=$HOME/prefix/ipopt
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$ADOLC_DIR/lib:$IPOPT_DIR/lib"
# -----------------------------------------------------------------------------
#
# get version currently in configure.ac file
# (in a way that works when version is not a date)
version=`grep "^ *AC_INIT(" configure.ac | \
	sed -e 's/[^,]*, *\([^ ,]*\).*/\1/'`
#
if [ "$1" = "all" ] && [ "$2" != "" ] && [ "$2" != "test" ] && [ "$2" != "dos" ]
then
	echo "./build.sh $1 $2"
	echo "is not valid, build.sh with no arguments lists valid choices."
	exit 1
fi
#
# Check if we are running all the test cases. 
if [ "$1" = "test" ] || ( [ "$1" = "all" ] && [ "$2" = "test" ] )
then
	date > build_test.log
	if [ -e cppad-$version ]
	then
		rm -rf cppad-$version
	fi
fi
#
# version
#
if [ "$1" = "version" ] || [ "$1" = "all" ]
then
	echo "build.sh version"
	#
	# Today's date in yy-mm-dd decimal digit format where 
	# yy is year in century, mm is month in year, dd is day in month.
	yyyymmdd=`date +%G%m%d`
	yyyy_mm_dd=`date +%G-%m-%d`
	#
	# automatically change version for certain files
	# (the [.0-9]* is for using build.sh in CppAD/stable/* directories)
	sed < AUTHORS > AUTHORS.$$ \
		-e "s/, [0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\} *,/, $yyyy_mm_dd,/"
	sed < configure.ac > configure.ac.$$\
		-e "s/(CppAD, [0-9]\{8\}[.0-9]* *,/(CppAD, $yyyymmdd,/" 
	#
	sed < configure > configure.$$ \
	-e "s/CppAD [0-9]\{8\}[.0-9]*/CppAD $yyyymmdd/g" \
	-e "s/VERSION='[0-9]\{8\}[.0-9]*'/VERSION='$yyyymmdd'/g" \
	-e "s/configure [0-9]\{8\}[.0-9]*/configure $yyyymmdd/g" \
	-e "s/config.status [0-9]\{8\}[.0-9]*/config.status $yyyymmdd/g" \
	-e "s/\$as_me [0-9]\{8\}[.0-9]*/\$as_me $yyyymmdd/g" 
	#
	chmod +x configure.$$
	sed < cppad/config.h > cppad/config.h.$$ \
		-e "s/CppAD [0-9]\{8\}[.0-9]*/CppAD $yyyymmdd/g" \
		-e "s/VERSION \"[0-9]\{8\}[.0-9]*\"/VERSION \"$yyyymmdd\"/g"
	list="
		AUTHORS
		configure.ac
		configure
		cppad/config.h
	"
	for name in $list
	do
		echo "diff $name $name.$$"
		diff $name $name.$$
		echo "mv   $name.$$ $name"
		mv   $name.$$ $name
	done
	#
	# change Autoconf version to today
	version=$yyyymmdd
	#
	if [ "$1" = "version" ]
	then
		exit 0
	fi
fi
#
# automake
#
if [ "$1" = "automake" ] || [ "$1" = "all" ]
then
	echo "build.sh automake"
	#
	# check that autoconf and automake output are in original version
	#
	makefile_in=`sed configure.ac \
        	-n \
        	-e '/END AC_CONFIG_FILES/,$d' \
        	-e '1,/AC_CONFIG_FILES/d' \
        	-e 's/makefile/&.in/' \
        	-e 's/^[ \t]*//' \
        	-e '/makefile/p'`
	auto_output="
		depcomp 
		install-sh 
		missing 
		configure 
		cppad/config.h 
		cppad/config.h.in 
		$makefile_in
	"
	for name in $auto_output
	do
		if [ ! -e $name ]
		then
			echo "$name is not in subversion repository"
			exit 1
		fi
	done
	#
	echo "---------------------------------------------------------"
	echo "If aclocal generates warning messages, run ./fix_aclocal.sh"
	echo "aclocal"
	if ! aclocal
	then
		exit 1
	fi
	echo "---------------------------------------------------------"
	#
	echo "autoheader"
	if ! autoheader
	then
		exit 1
	fi
	#
	echo "autoconf"
	if ! autoconf
	then
		exit 1
	fi
	#
	echo "automake --add-missing"
	if ! automake --add-missing
	then
		exit 1
	fi
	link_list="missing install-sh depcomp"
	for name in $link_list
	do
		if [ -h "$name" ]
		then
			echo "Converting $name from a link to a regular file"
			cp $name $name.$$
			if ! mv $name.$$ $name
			then
				echo "Cannot convert $name"
				exit 1
			fi
		fi
	done
	#
	if [ "$1" = "automake" ]
	then
		exit 0
	fi
fi
#
# configure
#
if [ "$1" = "configure" ] || [ "$1" = "all" ]
then
	if [ "$2" = "test" ]
	then
		echo "build.sh configure test"
	else
		echo "build.sh configure"
	fi
	#
	TEST=""
	if [ "$1" = "configure" ] && [ "$2" = "test" ]
	then
		TEST="
			--with-Introduction
			--with-Example
			--with-TestMore
			--with-Speed
			--with-PrintFor"
		if [ -e doc/index.htm ]
		then
			TEST="$TEST
				--with-Documentation"
		fi
		TEST="$TEST
			POSTFIX_DIR=coin"
		if [ -e $BOOST_DIR/boost ]
		then
			TEST="$TEST 
				BOOST_DIR=$BOOST_DIR"
		fi
		if [ -e $ADOLC_DIR/include/adolc ]
		then
			TEST="$TEST 
				ADOLC_DIR=$ADOLC_DIR"
		fi
		if [ -e $FADBAD_DIR/FADBAD++ ]
		then
			TEST="$TEST 
				FADBAD_DIR=$FADBAD_DIR"
		fi
		if [ -e $SACADO_DIR/include/Sacado.hpp ]
		then
			TEST="$TEST 
				SACADO_DIR=$SACADO_DIR"
		fi
		if [ -e $IPOPT_DIR/include/coin/IpIpoptApplication.hpp ]
		then
			TEST="$TEST 
				IPOPT_DIR=$IPOPT_DIR"
		fi
	fi
	TEST=`echo $TEST | sed -e 's|\t\t*| |g'`
	#
	echo "configure \\"
	echo "$TEST" | sed -e 's| | \\\n\t|g' -e 's|$| \\|' -e 's|^|\t|'
	echo "	CXX_FLAGS=\"-Wall -ansi -pedantic-errors -std=c++98\""
	#
	if ! ./configure $TEST \
		CXX_FLAGS="-Wall -ansi -pedantic-errors -std=c++98"
	then
		exit 1
	fi
	#
	# Fix makefile for what appears to be a bug in gzip under cygwin
	echo "fix_makefile.sh"
	./fix_makefile.sh
	#
	if [ "$1" = "configure" ]
	then
		exit 0
	fi
	#
	# make shell scripts created by configure executable
	echo "chmod +x example/test_one.sh"
	chmod +x example/test_one.sh
	echo "chmod +x test_more/test_one.sh"
	chmod +x test_more/test_one.sh
fi
#
# make
#
if [ "$1" = "make" ] || [ "$1" = "all" ]
then
	echo "build.sh make"
	#
	echo "make"
	if ! make
	then
		exit 1
	fi
	#
	if [ "$1" = "make" ]
	then
		exit 0
	fi
fi
#
# dist
#
if [ "$1" = "dist" ] || [ "$1" = "all" ]
then
	echo "build.sh dist"
	#
	if [ -e cppad-$version ]
	then
		echo "rm -f -r cppad-$version"
		if ! rm -f -r cppad-$version
		then
			echo "Build: cannot remove old cppad-$version"
			exit 1
		fi
	fi
	for file in cppad-*.tgz cppad-*.zip
	do
		if [ -e $file ]
		then
			echo "rm $file"
			rm $file
		fi
	done
	#
	# only build the *.xml version of the documentation for distribution
	if ! grep < doc.omh > /dev/null \
		'This comment is used to remove the table below' 
	then
		echo "Error: Missing comment expected in doc.omh"
		exit 1
	fi
	mv doc.omh doc.tmp
	sed < doc.tmp > doc.omh \
		-e '/This comment is used to remove the table below/,/$tend/d'
	echo "./run_omhelp.sh doc clean"
	if ! ./run_omhelp.sh doc clean
	then
		echo "./run_omhelp.sh doc clean failed."
		exit 1
	fi
	echo "./run_omhelp.sh doc xml"
	if ! ./run_omhelp.sh doc xml 
	then
		echo "./run_omhelp.sh doc xml failed."
		exit 1
	fi
	mv doc.tmp doc.omh
	#
	echo "make dist"
	if ! make dist
	then
		exit 1
	fi
	#
	if [ ! -e cppad-$version.tar.gz ]
	then
		echo "cppad-$version.tar.gz does not exist"
		echo "perhaps version is out of date"
		#
		exit 1
	fi
	# change *.tgz to *.cpl.tgz
	if ! mv cppad-$version.tar.gz cppad-$version.cpl.tgz
	then
		echo "cannot move cppad-$version.tar.gz to cppad-$version.tgz"
		exit 1
	fi
	#
	#
	if [ "$1" = "dist" ]
	then
		exit 0
	fi
fi
if [ "$1" = "omhelp" ] || [ "$1" = "all" ]
then
	for flag in "printable" ""
	do
		for ext in htm xml
		do
			echo "./run_omhelp.sh doc $ext $flag"
			if ! ./run_omhelp.sh doc $ext $flag
			then
				msg="Error: run_omhelp.sh doc $ext $flag"
				echo "$msg" 
				exit 1
			fi
			msg="OK: run_omhelp.sh doc $ext $flag"
			echo "$msg" 
		done
	done
	#
	if [ "$1" = "omhelp" ]
	then
		exit 0
	fi
fi
if [ "$1" = "doxygen" ] || [ "$1" = "all" ]
then
	echo "--------------------------------------------------------------"
	echo "doxygen doxyfile >& doxygen.log"
	if ! doxygen doxyfile >& doxygen.log
	then
		echo "Error: doxygen doxyfile"
		exit 1
	fi
	if ! ./check_doxygen.sh
	then
		echo "Warnings of doxygen output."
		if [ "$2" == "test" ]
		then
			exit 1
		fi
	fi
	#
	echo "pushd doxydoc/latex ; make >& ../../doxygen_tex.log"
	if ! pushd doxydoc/latex 
	then
		echo "Error: pushd doxydoc/latex"
		exit 1
	fi
	if ! make >& ../../doxygen_tex.log
	then
		echo "Error: pushd doxydoc/latex ; make"
		exit 1
	fi
	if ! popd 
	then
		echo "Error: pushd doxydoc/latex ; make ; popd"
		exit 1
	fi
	echo "mv doxydoc/latex/refman.pdf doxydoc/html/cppad.pdf"
	if ! mv doxydoc/latex/refman.pdf doxydoc/html/cppad.pdf
	then
		echo "Error: mv doxydoc/latex/refman.pdf doxydoc/html/cppad.pdf"
		exit 1
	fi
	echo "mv doxydoc/html doxydoc"
	if ! mv doxydoc doxydoc.$$
	then
		echo "Error: mv doxydoc doxydoc.$$"
		exit 1
	fi
	if ! mv doxydoc.$$/html doxydoc
	then
		echo "Error: mv doxydoc.$$/html doxydoc"
		exit 1
	fi
	if ! rm -r doxydoc.$$
	then
		echo "Error: rm -r doxydoc.$$"
		exit 1
	fi
	#
	if [ "$1" = "doxygen" ]
	then
		exit 0
	fi
	echo "--------------------------------------------------------------"
fi

if [ "$1" = "test" ] || ( [ "$1" = "all" ] && [ "$2" = "test" ] )
then
	# -------------------------------------------------------------
	# Run automated checking of file names in original source directory
	#
	list="
		check_example.sh
		check_include_def.sh
		check_include_file.sh
		check_include_omh.sh
		check_makefile.sh
	"
	for check in $list 
	do
		if ! ./$check >> build_test.log
		then
			echo "./$check failed"
			exit 1
		fi
	done
	# add a new line after last file check
	echo ""                 >> build_test.log
	#
	# Extract the distribution
	#
	if [ -e cppad-$version ]
	then
		echo "rm -f -r cppad-$version"
		if ! rm -f -r cppad-$version
		then
			echo "Build: cannot remove old cppad-$version"
			exit 1
		fi
	fi
	#
	if [ -e "cppad-$version.cpl.tgz" ]
	then
		dir="."
	else
		if [ -e "doc/cppad-$version.cpl.tgz" ]
		then
			dir="doc"
		else
			echo "cannot find cppad-$version.cpl.tgz"
			exit 1
		fi
	fi
	#
	#
	echo "tar -xzf $dir/cppad-$version.cpl.tgz"
	if ! tar -xzf $dir/cppad-$version.cpl.tgz
	then
		exit 1
	fi
	#
	dir=`pwd`
	cd cppad-$version
	# -------------------------------------------------------------
	# Configure
	#
	if ! ./build.sh configure test
	then
		echo "Error: build.sh configure test"  >> $dir/build_test.log
		echo "Error: build.sh configure test" 
		exit 1
	fi
	# -------------------------------------------------------------
	# Test user documentation 
	if [ ! -e "doc/index.xml" ]
	then
		echo "Error: doc/index.xml missing" >> $dir/build_test.log
		echo "Error: doc/index.xml missing"
		exit 1
	fi
	for user in doc dev
	do
		for ext in htm xml
		do
			echo "./run_omhelp.sh $user $ext"
			if ! ./run_omhelp.sh $user $ext
			then
				msg="Error: run_omhelp.sh $user $ext"
				echo "$msg" >> $dir/build_test.log 
				echo "$msg" 
				exit 1
			fi
			msg="OK: run_omhelp.sh $user $ext"
			echo "$msg" >> $dir/build_test.log
		done
	done
	# Test developer documentation ---------------------------------------
	echo "doxygen doxyfile >& doxygen.log"
	if ! doxygen doxyfile >& doxygen.log
	then
		echo "Error: doxygen doxyfile"
		exit 1
	fi
	if ! ./check_doxygen.sh
	then
		echo "Warnings of doxygen output; see doxygen.log."
		exit 1
	fi
	echo "OK: doxygen doxyfile" >> $dir/build_test.log
	# -------------------------------------------------------------
	# Compile
	#
	# gcc 3.4.4 with optimization generates incorrect warning; see 
	# 	http://cygwin.com/ml/cygwin-apps/2005-06/msg00161.html
	# The sed commands below are intended to remove them.
	echo "make >& $dir/make.log"
	echo "You may use commmand below to view progress of command above"
	echo "tail -f $dir/make.log"
	if ! make >&  ../make.log
	then
		echo "There are errors in $dir/make.log"
		exit 1
	fi
	sed ../make.log > make.log.$$ \
		-e '/op_code.hpp:368: warning: array subscript is above/d' \
		-e '/stl_uninitialized.h:82: warning: .__cur. might be/d'
	if grep 'warning:' make.log.$$
	then
		tmp=`pwd`
		echo "Stopping because there are unexpected warnings in"
		echo "$dir/make.log"
		exit 1
	fi
	echo "OK: make" 
	echo "OK: make" >> $dir/build_test.log
	# ---------------------------------------------------------------
	# Run execuables
	#
	list="
		example/example
		introduction/exp_apx/exp_apx
		introduction/get_started/get_started
		test_more/test_more
	"
	if [ -e $IPOPT_DIR/include/coin/IpIpoptApplication.hpp ]
	then
		list="
			ipopt_cppad/ipopt_cppad
			$list
		"
	fi
	for program in $list
	do
		echo "running $program"
		echo "$program"   >> $dir/build_test.log
		if ! ./$program   >> $dir/build_test.log
		then
			echo "Error: $program failed."
			echo "Error: $program failed." >> $dir/build_test.log
			exit 1
		fi
		# add a new line between program outputs
		echo ""  >> $dir/build_test.log
	done
	list="
		cppad
		double
		example
		profile
	"
	if [ -e $ADOLC_DIR/include/adolc ]
	then
        	list="$list adolc"
	fi
	if [ -e $FADBAD_DIR/FADBAD++ ]
	then
        	list="$list fadbad"
	fi
	if [ -e $SACADO_DIR/include/Sacado.hpp ]
	then
		list="$list sacado"
	fi
	seed="123"
	retape="false"
	for name in $list
	do
		# Note that example does not use command line arguments,
		# but it does not currently care about their presence.
		echo "running speed/$name/$name correct $seed $retape"
		echo "./speed/$name/$name correct $seed $retape" \
			>> $dir/build_test.log
		if ! ./speed/$name/$name correct  $seed $retape \
			>> $dir/build_test.log
		then
			program="speed/$name/$name"
			echo "Error: $program failed."
			echo "Error: $program failed." >> $dir/build_test.log
			exit 1
		fi
		# add a new line between program outputs
		echo ""  >> $dir/build_test.log
	done
	echo "openmp/run.sh"
	echo "openmp/run.sh" >> $dir/build_test.log
	if !  openmp/run.sh >> $dir/build_test.log
	then
		failed="openmp/run.sh $program"
		echo "Error: $failed failed."
		echo "Error: $failed failed." >> $dir/build_test.log
		exit 1
	fi
	echo "" >> $dir/build_test.log
	#
	cd ..
	if [ "$1" = "test" ]
	then
		# end the build_test.log file with the date and time
		date >> build_test.log
		#
		dir=`pwd`
		echo "Check $dir/build_test.log for errors and warnings."
		exit 0
	fi
fi
if [ "$1" = "gpl" ] || [ "$1" = "all" ]
then
	# create GPL licensed version
	echo "gpl_license.sh"
	if ! ./gpl_license.sh
	then
		echo "Error: gpl_license.sh failed."
		if [ "$2" = "test" ]
		then
			echo "Error: gpl_license.sh failed." >> build_test.log
		fi
		exit 1
	else
		echo "OK: gpl_license.sh."
		if [ "$2" = "test" ]
		then
			echo "OK: gpl_license.sh." >> build_test.log
		fi
	fi
	if [ "$1" = "gpl" ]
	then
		exit 0
	fi
fi
if [ "$1" = "dos" ] || ( [ "$1" = "all" ] && [ "$2" == "dos" ] )
then
	echo "./dos_format.sh"
	if ! ./dos_format.sh
	then
		echo "Error: dos_format.sh failed."
		if [ "$2" = "test" ]
		then
			echo "Error: dos_format.sh failed." >> build_test.log
		fi
		exit 1
	else
		echo "OK: dos_format.sh."
		if [ "$2" = "test" ]
		then
			echo "OK: dos_format.sh." >> build_test.log
		fi
	fi
	#
	if [ "$1" = "dos" ]
	then
		exit 0
	fi
fi
if [ "$1" = "move" ] || [ "$1" = "all" ] 
then
	# copy tarballs into doc directory
	list="
		cppad-$version.cpl.tgz
		cppad-$version.gpl.tgz
	"
	if [ "$1" = "all" ] && [ "$2" == "dos" ] 
	then
		list="$list
			cppad-$version.cpl.zip
			cppad-$version.gpl.zip
		"
	fi
	for file in $list
	do
		echo "mv $file doc/$file"
		if ! mv $file doc/$file
		then
			echo "Error: mv $file doc."
			if [ "$2" = "test" ]
			then
				echo "Error: mv $file doc." >> build_test.log
			fi
			exit 1
		fi
	done
	if [ "$1" = "move" ]
	then
		exit 0
	fi
fi
if [ "$1" = "all" ]
then
	if [ "$2" = "test" ]
	then
		# end the build_test.log file with the date and time
		date >> build_test.log
	fi
	exit 0
fi
#
if [ "$1" = "" ]
then
	echo "usage: build.sh option (where valid options are listed below)" 
else
	echo "$1 is not a valid option (valid options are listed below)"
fi
echo "option"
echo "------"
echo "version        update configure.ac and doc.omh version number"
echo "automake       run aclocal,autoheader,autoconf,automake -> configure"
echo "configure      excludes --with-*"
echo "configure test includes all the possible options except PREFIX_DIR"
echo "omhelp         build all formats for user documentation in doc/*"
echo "doxygen        build developer documentation in doxydoc/*"
echo "make           use make to build all of the requested targets"
echo "dist           create the distribution file cppad-version.cpl.tgz"
echo "test           unpack *.cpl.tgz, compile, tests, result in build_test.log"
echo "gpl            create *.gpl.tgz"
echo "dos            create *.gpl.zip, and *.cpl.zip"
echo "move           move *.tgz to doc directory"
echo
echo "build.sh all"
echo "This command will execute all the options above in order with the"
echo "exception that \"configue test\", \"test\", and \"dos\" are excluded."
echo
echo "build.sh all dos"
echo "This command will execute all the options above in order with the"
echo "exception that \"configure test\" and \"test\" are excluded."
echo
echo "build.sh all test"
echo "This command will execute all the options above in order with the"
echo "exception that \"configure\",  \"dos\", and \"move\" are excluded."
#
exit 1
