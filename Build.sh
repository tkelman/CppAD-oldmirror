# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# This program is free software; you can use it under the terms of the 
#	         Common Public License Version 1.0.
# You should have received a copy of the this license along with this program.
# -----------------------------------------------------------------------------
#
# Bash script for building the CppAD distribution.
#
# Default values used for arguments to configure during this script.
# These defaults are development system dependent and can be changed.
ADOLC_DIR=$HOME/adolc_base
FADBAD_DIR=$HOME
BOOST_DIR=/usr/include/boost-1_33
#
# date currently in configure.ac
AcDate=`grep "^ *AC_INIT(" configure.ac | \
	sed -e "s/.*, *\([0-9]\{8\}\) *,.*/\1/"`
#
# version
#
if [ "$1" = "version" ] || [ "$1" = "all" ]
then
	echo "Build.sh version"
	#
	# Today's date in ccyy-mm-dd decimal digit format where cc is century,
	# yy is year in century, mm is month in year, dd is day in month.
	ccyy_mm_dd=`date +%C%g-%m-%d`
	#
	# Today's date in ccyymmdd format
	ccyymmdd=`date +%C%g%m%d`
	#
	# configure.ac
	sed configure.ac > configure.ac.$$ \
		-e "s/(CppAD, [0-9]\{8\} *,/(CppAD, $ccyymmdd,/"
	diff configure.ac  configure.ac.$$
	mv   configure.ac.$$ configure.ac
	#
	# AUTHORS
	sed AUTHORS > AUTHORS.$$ \
	-e "s/, *[0-9]\{4\}-[0-9][0-9]-[0-9][0-9] *,/, $ccyy_mm_dd,/"
	diff AUTHORS    AUTHORS.$$
	mv   AUTHORS.$$ AUTHORS 
	#
	# change Autoconf version to today
	AcDate=$ccyymmdd
	#
	for name in Doc.omh omh/InstallUnix.omh omh/InstallWindows.omh
	do
		sed $name > $name.$$ \
			-e "s/cppad-[0-9]\{8\}/cppad-$ccyymmdd/g"
		diff $name $name.$$
		mv   $name.$$ $name
	done
	#
	# configure file is out of date so remove it
	if [ -e configure ]
	then
		rm configure
	fi
	#
	if [ "$1" != "all" ]
	then
		exit 0
	fi
fi
#
# automake
#
if [ "$1" = "automake" ] || [ "$1" = "all" ]
then
	echo "Build.sh automake"
	#
	if [ -e configure ]
	then
		rm configure
	fi
	echo "---------------------------------------------------------"
	echo "If aclocal generates warning messages, run ./FixAclocal.sh"
	echo "aclocal"
	aclocal
	echo "---------------------------------------------------------"
	#
	echo "autoheader"
	autoheader
	#
	echo "autoconf"
	autoconf
	#
	echo "automake -add-missing"
	automake --add-missing
	#
	if [ "$1" != "all" ]
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
		echo "Build.sh configure test"
	else
		echo "Build.sh configure"
	fi
	#
	TEST="--with-GetStarted"
	if [ "$2" = "test" ]
	then
		TEST="$TEST --with-Introduction"
		TEST="$TEST --with-Example"
		TEST="$TEST --with-TestMore"
		TEST="$TEST --with-Speed"
		TEST="$TEST --with-PrintFor"
		TEST="$TEST --with-SpeedExample"
		TEST="$TEST --with-profiling"
		if [ -e $ADOLC_DIR/include/adolc ]
		then
			TEST="$TEST ADOLC_DIR=$ADOLC_DIR"
		fi
		if [ -e $FADBAD_DIR/FADBAD++ ]
		then
			TEST="$TEST FADBAD_DIR=$FADBAD_DIR"
		fi
		if [ -e $BOOST_DIR/boost ]
		then
			TEST="$TEST BOOST_DIR=$BOOST_DIR"
		fi
	fi
	#
	echo "configure \\"
	echo "  $TEST   \\"
	echo "	CPP_ERROR_WARN=\"-Wall -ansi -pedantic-errors -std=c++98\""
	#
	./configure \
		$TEST   \
		CPP_ERROR_WARN="-Wall -ansi -pedantic-errors -std=c++98"
	#
	# Fix Makefile for what appears to be a bug in gzip under cygwin
	echo "FixMakefile.sh"
	./FixMakefile.sh
	#
	if [ "$1" != "all" ]
	then
		exit 0
	fi
fi
#
# make
#
if [ "$1" = "make" ] || [ "$1" = "all" ]
then
	echo "Build.sh make"
	#
	echo "make"
	make
	#
	if [ "$1" != "all" ]
	then
		exit 0
	fi
fi
#
# omhelp
#
if [ "$1" = "omhelp" ] || [ "$1" = "all" ]
then
	echo "Build.sh omhelp"
	#
	echo "RunOMhelp.sh Dev"
	if ! ./RunOMhelp.sh Dev
	then
		exit 1
	fi
	#
	echo "RunOMhelp.sh Doc"
	if ! ./RunOMhelp.sh Doc
	then
		exit 1
	fi
	#
	if [ "$1" != "all" ]
	then
		exit 0
	fi
fi
#
# dist
#
if [ "$1" = "dist" ] || [ "$1" = "all" ]
then
	echo "Build.sh dist"
	#
	if [ -e cppad-$AcDate ]
	then
		echo "rm -f -r cppad-$AcDate"
		if ! rm -f -r cppad-$AcDate
		then
			echo "Build: cannot remove old cppad-$AcDate"
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
	echo "make dist"
	make dist
	#
	if [ ! -e cppad-$AcDate.tar.gz ]
	then
		echo "cppad-$AcDate.tar.gz does not exist"
		echo "perhaps version is out of date"
		#
		exit 1
	fi
	# change *.tar.gz to *.cpl.tgz
	mv cppad-$AcDate.tar.gz cppad-$AcDate.cpl.tgz
	#
	# create GPL licensed version
	echo "GplLicense.sh"
	if ! ./GplLicense.sh
	then
		exit 1
	fi
	#
	if [ "$1" != "all" ]
	then
		exit 0
	fi
fi
if [ "$1" = "dos" ] || ( [ "$1" = "all" ] && [ "$2" != "unix" ] )
then
	echo "./DosFormat.sh"
	if ! ./DosFormat.sh
	then
		exit 1
	fi
	#
	if [ "$1" != "all" ]
	then
		exit 0
	fi
fi
if [ "$1" = "test" ] || ( [ "$1" = "all" ] && [ "$2" = "unix" ] )
then
	#
	if [ -e cppad-$AcDate ]
	then
		echo "rm -f -r cppad-$AcDate"
		if ! rm -f -r cppad-$AcDate
		then
			echo "Build: cannot remove old cppad-$AcDate"
			exit 1
		fi
	fi
	#
	echo "tar -xzf cppad-$AcDate.cpl.tgz"
	tar -xzf cppad-$AcDate.cpl.tgz
	#
	wd=`pwd`
	cd cppad-$AcDate
	./Build.sh configure test
	make
	echo "creating $wd/Test.log"
	Example/Example           >  ../Test.log
	TestMore/TestMore         >> ../Test.log
	Introduction/Introduction >> ../Test.log
	Fadbad/Example            >> ../Test.log
	Adolc/Example             >> ../Test.log
	RunOMhelp.sh Doc
	cat OMhelp.Doc.log        >> ../Test.log
	if [ "$1" != "all" ]
	then
		exit 0
	fi
fi
if [ "$1" = "all" ]
then
	exit 0
fi
#
if [ "$1" = "" ]
then
	echo "usage: Build.sh option (where valid options are listed below)" 
else
	echo "$1 is not a valid option (valid options are listed below)"
fi
echo "option"
echo "------"
echo "version         update configure.ac and Doc.omh version number"
echo "automake        run aclocal,autoheader,autoconf,automake -> configure"
echo "configure       excludes --with-* except GetStarted and Introduction"
echo "configure test  includes all the possible options except PREFIX_DIR"
echo "make            use make to build all of the requested targets"
echo "omhelp          build all the documentation"
echo "dist            create the distribution files cppad-yy-mm-dd.*.tgz"
echo "dos             create the distribution files cppad-yy-mm-dd.*.zip"
echo "test            unpack the unix distribution and compiles its tests"
echo
echo "Build.sh all"
echo "This command will execute all the options in the order above"
echo "with the exception that \"configue test\" and \"test\" will be excluded."
echo
echo "Build.sh all unix"
echo "This command will execute all the options in the order above"
echo "with the exception that \"configue test\" and \"dos\" will be excluded."
#
exit 1
