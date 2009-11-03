# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
# Bradley M. Bell has given COIN-OR permission to use this script to generate 
# a distribution of CppAD that has "GNU General Public License Version 2"
# in place of "Common Public License Version 1.0." in all occurrences
# of the message above.
#
version=`grep "^ *AC_INIT(" configure.ac | \
        sed -e 's/[^,]*, *\([^ ,]*\).*/\1/'`
dir="cppad-$version"
#
# delete old version of *.gpl.tgz and *.gpl.zip
for file in $dir.gpl.tgz $dir.gpl.zip
do
	if [ -e $file ]
	then
        	echo "rm -f $file"
        	if ! rm -f $file
        	then
			echo "gpl_license.sh: cannot remove old $file"
        		exit 1
		fi
	fi
done
#
# start from the *.cpl.tgz file
if [ ! -e $dir.cpl.tgz ]
then
	echo "gpl_license.sh: cannot find $dir.cpl.tgz"
	exit 1
fi
#
# delete old version of directory (if it exists)
if [ -e $dir ]
then
	echo "rm -f -r $dir"
	rm -f -r $dir
fi
if [ -e $dir ]
then
	echo "gpl_license.sh: cannot remove old $dir directory"
	exit 1
fi
#
# extract from the *.cpl.tgz file
echo "tar -xzf $dir.cpl.tgz"
tar -xzf $dir.cpl.tgz
#
# make sure can change into new directory 
if ! cd $dir
then
	echo "gpl_license.sh: cannot make new $dir working directory"
	exit 1
fi
#
list=`find . \
	\( -name '*.am'  \) -or \
	\( -name '*.ac'  \) -or \
	\( -name '*.ac'  \) -or \
	\( -name '*.sh'  \) -or \
	\( -name '*.cpp' \) -or \
	\( -name '*.hpp' \) -or \
	\( -name '*.in'  \) -or \
	\( -name '*.omh' \)`
#
# change back up to original directory (to be safe)
cd ..
#
echo "gpl_license.sh: changing license from CPL to GPL"
for file in $list
do
	file=`echo $file | sed -e 's|^\./||'`
	name=`echo $file | sed -e 's|.*\/||'`
	if [ "$name" = "makefine.in" ] || [ "$name" == "config.h.in" ]
	then
		if grep "Common Public License" $dir/$file > /dev/null
		then
			echo "CPL license in $dir/$file"
			exit 1
		fi
	else
		if grep "GNU General Public License" $dir/$file > /dev/null
		then
		if [ "$name" != "doc.omh.in" ] && [ "$name" != "doc.omh" ]
			then
				echo "GPL license in initial $dir/$file"
				exit 1
			fi
			head -20 $dir/$file | \
			if grep "GNU General Public License" > /dev/null
			then
				echo "GPL license in initial $dir/$file"
				exit 1
			fi
		fi
		#
		sed -i $dir/$file \
-e 's/Common Public License Version 1.0/GNU General Public License Version 2/' 
		#
		if ! grep "GNU General Public License" $dir/$file > /dev/null
		then
			echo "Cannot change CPL to GPL for $dir/$file"
			exit 1
		fi
	fi
	if [ "$ext" = ".sh" ]
	then
		chmod +x $dir/$file
	fi
done
#
# change the COPYING file
sed -n < COPYING > $dir/COPYING \
-e 's/Common Public License Version 1.0/GNU General Public License Version 2/' \
-e '/-\{70\}/,/-\{70\}/p'
cat gpl2.txt >> $dir/COPYING
#
# change the file cpl1.0.txt to the file gpl2.txt
rm $dir/cpl1.0.txt
cp gpl2.txt $dir/gpl2.txt
#
# change occorances of cpl1.0.txt to gpl2.txt
sed < makefile.am > $dir/makefile.am  \
-e 's/Common Public License Version 1.0/GNU General Public License Version 2/' \
	-e 's/cpl1.0.txt/gpl2.txt/'
#
sed < makefile.in > $dir/makefile.in \
-e 's/Common Public License Version 1.0/GNU General Public License Version 2/' \
	-e 's/cpl1.0.txt/gpl2.txt/'
#
sed < omh/license.omh > $dir/omh/license.omh \
-e 's/Common Public License Version 1.0/GNU General Public License Version 2/' \
	-e 's/$verbatim%cpl1.0.txt%$\$/$verbatim%gpl2.txt%$$/'
#
# Rerun omhelp to change documentation version of license from CPL to GPL 
cd $dir
if ! ./run_omhelp.sh doc
then
	echo "gpl_license.sh: error running omhelp"
	exit 1
fi
if [ -e "doc/error.wrd" ]
then
	rm -rf doc/error.wrd
fi
cd ..
#
# create *.gpl.tgz file
echo "tar -czf $dir.gpl.tgz $dir"
tar -czf $dir.gpl.tgz $dir