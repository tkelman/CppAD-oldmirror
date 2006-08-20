# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# This program is free software; you can use it under the terms of the 
#	         Common Public License Version 1.0.
# You should have received a copy of the this license along with this program.
# -----------------------------------------------------------------------------
#
# date currently in configure.ac
AcDate=`grep "^ *AC_INIT(" configure.ac | \
	sed -e "s/.*, *\([0-9]\{8\}\) *,.*/\1/"`
#
# delete old version of *.gpl.tgz and *.gpl.zip
for file in cppad-$AcDate.gpl.tgz cppad-$AcDate.gpl.zip
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
if [ ! -e cppad-$AcDate.cpl.tgz ]
then
	echo "GplLicense: cannot find cppad-$AcDate.cpl.tgz"
	exit 1
fi
#
# delete old version of directory (if it exists)
if [ -e cppad-$AcDate ]
then
	echo "rm -f -r cppad-$AcDate"
	rm -f -r cppad-$AcDate
fi
if [ -e cppad-$AcDate ]
then
	echo "GplLicense: cannot remove old cppad-$AcDate directory"
	exit 1
fi
#
# extract from the *.cpl.tgz file
echo "tar -xzf cppad-$AcDate.cpl.tgz"
tar -xzf cppad-$AcDate.cpl.tgz
#
# make sure can change into new directory 
if ! cd cppad-$AcDate
then
	echo "GplLicense: cannot make new cppad-$AcDate working directory"
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
	sed < cppad-$AcDate/$file > GplLicense.tmp \
-e 's/Common Public License Version 1.0/GNU General Public License Version 2/' 

	mv GplLicense.tmp cppad-$AcDate/$file
	#
	if ! grep "GNU General Public License Version 2" \
		cppad-$AcDate/$file > /dev/null
	then
		name=`echo $file | sed -e 's|.*/||'`
		if [ "$name" != "config.h" ] 
		then
	echo "GplLicense: can not change cppad-$AcDate/$file license" 
	exit 1
		fi
	fi
	if [ "$ext" = ".sh" ]
	then
		chmod +x cppad-$AcDate/$file
	fi
done
#
# change licenses from cpl1.0.txt to gpl2.txt
rm cppad-$AcDate/cpl1.0.txt
cp gpl2.txt cppad-$AcDate/gpl2.txt
sed < Makefile.am > cppad-$AcDate/Makefile.am \
	-e 's/cpl1.0.txt/gpl.txt/'
sed < omh/License.omh > cppad-$AcDate/omh/License.omh \
	-e 's/$verbatim%cpl1.0.txt%$\$/$verbatim%gpl2.txt%$$/'
#
# fix time stamp on automake files
touch cppad-$AcDate/aclocal.m4
sleep 2
touch cppad-$AcDate/CppAD/config.h.in
sleep 2
touch cppad-$AcDate/Makefile.in
touch cppad-$AcDate/*/Makefile.in
sleep 2
touch cppad-$AcDate/configure
#
# create *.gpl.tgz file
echo "tar -czf cppad-$AcDate.gpl.tgz cppad-$AcDate"
tar -czf cppad-$AcDate.gpl.tgz cppad-$AcDate
#
echo "GplLicense: creating dos version"
list="
	$list
	AUTHORS
	ChangeLog
	INSTALL
	NEWS
	gpl2.txt
"
for file in $list
do
	ext=`echo $file | sed -e "s/.*\././"`
	file=`echo $file | sed -e 's|^\./||'`
	#
	unix2dos cppad-$AcDate/$file >& /dev/null
	#
	if [ "$ext" = ".sh" ]
	then
		chmod +x cppad-$AcDate/$file
	fi
done
echo "zip -q -r cppad-$AcDate.gpl.zip cppad-$AcDate"
zip -q -r cppad-$AcDate.gpl.zip cppad-$AcDate
