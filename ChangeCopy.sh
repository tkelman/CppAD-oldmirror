#! /bin/bash
list=`find . \
	\( -name '*.h'   \) -or \
	\( -name '*.am'  \) -or \
	\( -name '*.ac'  \) -or \
	\( -name '*.ac'  \) -or \
	\( -name '*.cpp' \) -or \
	\( -name '*.omh' \)`
list="
	$list
	AUTHORS
	COPYING
	Build
	CleanCppAD
	FixAclocal
	RunOMhelp
"
for file in $list
do
	if ! grep "Copyright (C)" $file >> /dev/null
	then
		echo "$file has not copyright message"
		exit 1
	fi
done
