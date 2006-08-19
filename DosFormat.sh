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
	sed -e "s/.*, *\([0-9][0-9]-[0-9][0-9]-[0-9][0-9]\) *,.*/\1/"`
#
# delete old version of *.cpl.zip and *.gpl.zip
for file in cppad-$AcDate.cpl.zip cppad-$AcDate.gpl.zip
do
	if [ -e $file ]
	then
        	echo "rm -f $file"
        	if ! rm -f $file
        	then
			echo "DosFormat: cannot remove old $file"
        		exit 1
		fi
	fi
done
for license in cpl gpl
do
	# delete old version of directory (if it exists)
	if [ -e cppad-$AcDate ]
	then
		echo "rm -r cppad-$AcDate"
		if ! rm -r cppad-$AcDate
		then
			echo "DosFormat: cannot remove old cppad-$AcDate"
			exit 1
		fi
	fi
	#
	# Convert this license version 
	if [ ! -e cppad-$AcDate.$license.tgz ]
	then
		echo "DosFormat: cannot find cppad-$AcDate.$license.tgz"
		exit 1
	fi
	#
	# extract from the *.$license.tgz file
	echo "tar -xzf cppad-$AcDate.$license.tgz"
	tar -xzf cppad-$AcDate.$license.tgz
	#
	# make sure can change into new directory 
	if ! cd cppad-$AcDate
	then
	echo "DosFromat: cannot make new cppad-$AcDate working directory"
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
		\( -name '*.omh' \) -or \
		\( -name '*.txt' \)`
	list="
		$list
		AUTHORS
		COYPING
		ChangeLog
		INSTALL
		NEWS
	"
	cd ..
	echo "DosFormat: converting file formats dos version"
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
	echo "zip -q -r cppad-$AcDate.$license.zip cppad-$AcDate"
	zip -q -r cppad-$AcDate.$license.zip cppad-$AcDate
	#
done
