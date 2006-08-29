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
		src=`echo $tmp | sed -e 's|.*/||' -e 's|\.old$|.h|'`
		dest=`echo $src | sed \
			-e 's|\.h$|.hpp|'    \
			-e 's|^CppAD|cppad_|' \
			-e 's|^AD|ad|'        \
			-e 's|\([a-z]\)\([A-Z]\)|\1_\2|g' | tr [A-Z] [a-z]`
		#
		src="CppAD/local/$src"
		dest="CppAD/local/$dest"
		echo \
			"s|\\([\\t <%]\\)$src\\([\\t  >%]\\)|\\1$dest\\2|" \
			>> filename.sed
		echo \
			"s|\\([\\t <%]\\)$src\$|\\1$dest|" \
			>> filename.sed
	done
	echo "s|\$section Lu[A-Z][a-z]* Source Code\$\\\$|&\n\$spell\n\tcppad\n\thpp\n\$\$|" >> filename.sed
else
	echo "Skipping creation of filename.sed. Delete this file if"
	echo "you wish to recreate it."
fi
# --------------------------------------------------------------------------
#
echo "Create the new version of files that need to be changed"
#
# *.hpp files
rm CppAD/local/*.hpp
list="CppAD/local/*.old"
for old in $list
do
	dest=`echo $old | sed \
		-e 's|.*/||'        \
		-e 's|\.old$|.hpp|' \
		-e 's|^CppAD|cppad_|'\
		-e 's|^AD|ad|'       \
		-e 's|\([a-z]\)\([A-Z]\)|\1_\2|g' | tr [A-Z] [a-z]`
	new="CppAD/local/$dest"
	#
	echo "sed -f filename.sed < $old > $new"
	sed -f filename.sed < $old > $new
done
#
# *.h files
rm CppAD/*.h
list="CppAD/*.old"
for old in $list
do
	new=`echo $old | sed -e 's|\.old$|.h|'`
	#
	echo "sed -f filename.sed < $old > $new"
	sed -f filename.sed < $old > $new
done
#
# *.omh files
rm omh/*.omh
list="Doc.old Dev.old omh/*.old"
for old in $list
do
	new=`echo $old | sed -e 's|\.old$|.omh|'`
	echo "sed -f filename.sed < $old > $new"
	sed -f filename.sed < $old > $new
done
#
sed -f filename.sed < Makefile.old > Makefile.am
