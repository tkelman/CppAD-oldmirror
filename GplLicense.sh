# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# This program is free software; you can use it under the terms of the 
#	         Common Public License Version 1.0.
# You should have received a copy of the this license along with this program.
# -----------------------------------------------------------------------------
#
# Bradley M. Bell has given COIN-OR permission to use this script to generate 
# a distribution of CppAD that has "GNU General Public License Version 2"
# in place of "Common Public License Version 1.0." in all occurrences
# of the message above.
#
# date currently in configure.ac
version=`grep "^ *AC_INIT(" configure.ac | \
	sed -e "s/.*, *\([0-9]\{8\}\) *,.*/\1/"`
#
# delete old version of *.gpl.tgz and *.gpl.zip
for file in cppad-$version.gpl.tgz cppad-$version.gpl.zip
do
	if [ -e $file ]
	then
        	echo "rm -f $file"
        	if ! rm -f $file
        	then
			echo "GplLicense: cannot remove old $file"
        		exit 1
		fi
	fi
done
#
# start from the *.cpl.tgz file
if [ ! -e cppad-$version.cpl.tgz ]
then
	echo "GplLicense: cannot find cppad-$version.cpl.tgz"
	exit 1
fi
#
# delete old version of directory (if it exists)
if [ -e cppad-$version ]
then
	echo "rm -f -r cppad-$version"
	rm -f -r cppad-$version
fi
if [ -e cppad-$version ]
then
	echo "GplLicense: cannot remove old cppad-$version directory"
	exit 1
fi
#
# extract from the *.cpl.tgz file
echo "tar -xzf cppad-$version.cpl.tgz"
tar -xzf cppad-$version.cpl.tgz
#
# make sure can change into new directory 
if ! cd cppad-$version
then
	echo "GplLicense: cannot make new cppad-$version working directory"
	exit 1
fi
#
list=`find . \
	\( -name '*.am'  \) -or \
	\( -name '*.ac'  \) -or \
	\( -name '*.ac'  \) -or \
	\( -name '*.sh' \) -or \
	\( -name '*.cpp' \) -or \
	\( -name '*.h'   \) -or \
	\( -name '*.hpp' \) -or \
	\( -name '*.omh' \)`
list="
	$list
	COPYING
"
#
# change back up to original directory (to be safe)
cd ..
#
echo "GplLicense: changing license from CPL to GPL"
for file in $list
do
	ext=`echo $file | sed -e "s/.*\././"`
	file=`echo $file | sed -e 's|^\./||'`
	#
	sed < cppad-$version/$file > GplLicense.tmp \
-e 's/Common Public License Version 1.0/GNU General Public License Version 2/' 

	mv GplLicense.tmp cppad-$version/$file
	#
	if ! grep "GNU General Public License Version 2" \
		cppad-$version/$file > /dev/null
	then
		name=`echo $file | sed -e 's|.*/||'`
		if [ "$name" != "config.h" ] 
		then
	echo "GplLicense: can not change cppad-$version/$file license" 
	exit 1
		fi
	fi
	if [ "$ext" = ".sh" ]
	then
		chmod +x cppad-$version/$file
	fi
done
#
# change licenses from cpl1.0.txt to gpl2.txt
rm cppad-$version/cpl1.0.txt
cp gpl2.txt cppad-$version/gpl2.txt
sed < Makefile.am > cppad-$version/Makefile.am \
	-e 's/cpl1.0.txt/gpl.txt/'
sed < omh/License.omh > cppad-$version/omh/License.omh \
	-e 's/$verbatim%cpl1.0.txt%$\$/$verbatim%gpl2.txt%$$/'
#
# fix time stamp on automake files
touch cppad-$version/aclocal.m4
sleep 2
touch cppad-$version/CppAD/config.h.in
sleep 2
touch cppad-$version/Makefile.in
touch cppad-$version/*/Makefile.in
sleep 2
touch cppad-$version/configure
#
# create *.gpl.tgz file
echo "tar -czf cppad-$version.gpl.tgz cppad-$version"
tar -czf cppad-$version.gpl.tgz cppad-$version
