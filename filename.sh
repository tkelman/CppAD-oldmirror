#! /bin/bash
#
# Script that changes the file names in the directory CppAD/local
#
# ---------------------------------------------------------------------------
if [ ! -e Makefile.old ]
then
	echo "Move files that need to be changed to old version name"
	#
	list="CppAD/*.h CppAD/local/*.h omh/*.omh"
	list="Doc.omh Dev.omh $list"
	for src in $list
	do
		tmp=`echo $src | sed -e 's|\.omh$|.old|' -e 's|\.h$|.old|'`
		echo "mv $src $tmp"
		mv $src $tmp
	done
	echo "mv Makefile.am Makefile.old"
	mv Makefile.am Makefile.old
else
	echo "Skipping file move step. Must checkout a fresh copy of"
	echo "CppAD/brances/filename to redo this step."
fi
# --------------------------------------------------------------------------
#
if [ ! -e filename.sed ]
then
	echo "Create the sed script that will edit the files"
	list="CppAD/local/*.old"
	for tmp in $list
	do
		src=`echo $tmp | sed -e 's|\.old$|.h|'`
		dest=`echo $tmp | sed -e 's|\.old$|.hpp|'`
		echo \
			"s|\\([\\t <%]\\)$src\\([\\t  >%]\\)|\\1$dest\\2|" \
			>> filename.sed
		echo \
			"s|\\([\\t <%]\\)$src\$|\\1$dest|" \
			>> filename.sed
	done
else
	echo "Skipping creation of filename.sed. Delete this file if"
	echo "you wish to recreate it."
fi
# --------------------------------------------------------------------------
#
echo "Create the new version of files that need to be changed"
#
# *.hpp files
list="CppAD/local/*.old"
for old in $list
do
	new=`echo $old | sed -e 's|\.old$|.hpp|'`
	sed -f filename.sed < $old > $new
done
#
# *.h files
list="CppAD/*.old"
for old in $list
do
	new=`echo $old | sed -e 's|\.old$|.h|'`
	sed -f filename.sed < $old > $new
done
#
# *.omh files
list="Doc.old Dev.old omh/*.old"
for old in $list
do
	new=`echo $old | sed -e 's|\.old$|.omh|'`
	sed -f filename.sed < $old > $new
done
#
sed -f filename.sed < Makefile.old > Makefile.am
