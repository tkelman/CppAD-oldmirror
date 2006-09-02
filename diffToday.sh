# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
# Change date of subversion version of file to current date and then
# difference that with local version of file
#
# century, year, month, and date
ccyymmdd=`date +%C%g%m%d`
ccyy_mm_dd=`date +%C%g-%m-%d`
#
if [ ! -e "$1" ]
then
	echo "diffToday file_name"
	exit
fi
svn cat $1 | sed > $1.tmp \
	-e "s/(CppAD, [0-9]\{8\} *,/(CppAD, $ccyymmdd,/" \
	-e "s/, *[0-9]\{4\}-[0-9][0-9]-[0-9][0-9] *,/, $ccyy_mm_dd,/" \
	-e "s/cppad-[0-9]\{8\}/cppad-$ccyymmdd/g"
#
echo "diff $1-local $1-subversion"
diff          $1      $1.tmp
rm                    $1.tmp


