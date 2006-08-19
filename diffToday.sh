# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# This program is free software; you can use it under the terms of the 
#	         Common Public License Version 1.0.
# You should have received a copy of the this license along with this program.
# -----------------------------------------------------------------------------
#
# Change date of subversion version of file to current date and then
# difference that with local version of file
#
Today=`date +%g-%m-%d`
#
if [ ! -e "$1" ]
then
	echo "diffToday list_of_file_names"
	exit
fi
svn cat $1 | sed > $1.tmp \
-e "s/\([^0-9]\)[0-9][0-9]-[0-9][0-9]-[0-9][0-9]\([^0-9]\)/\1$Today\2/"  \
-e "s/\([^0-9]\)[0-9][0-9]-[0-9][0-9]-[0-9][0-9]$/\1$Today/" 
echo "diff $1-local $1-subversion"
diff          $1      $1.tmp
rm                    $1.tmp


