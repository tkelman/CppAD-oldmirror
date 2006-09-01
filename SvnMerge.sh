# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# -----------------------------------------------------------------------------
#
#
# Merge the changes that occurred in FromDirectory into the ToDirectory
#
DryRun="--dry-run"
if [ "$1" == "not--dry-run" ]
then
	DryRun=""
fi
echo "DryRun = $DryRun"
#
# script to help in execution of svn merge command
#
# Change into the directory corresponding to the entire repository; i.e.,
# http://www.coin-or.org/svn/CppAD/ 
cd ..
#
# Name of the directory that will receive the changes
ToDirectory=trunk
#
# Name of the directory where the changes have been committed
FromDirectory=branches/ADFun
#
# Version of the repository corresponding to FromDirectory just before changes
Start=435
# 
# Version of the repository corresponding to FromDirecrtory after the changes
End=449
#
# echo the svn merge command
echo "cd  .."
echo "svn merge $DryRun > SvnMerge.out -r $Start:$End   \\"
echo "	http://www.coin-or.org/svn/CppAD/$FromDirectory \\"
echo "	$ToDirectory"
#
# execute the svn merge command
svn merge $DryRun > SvnMerge.out -r $Start:$End              \
	http://www.coin-or.org/svn/CppAD/$FromDirectory      \
	$ToDirectory
#
# put comment here so have newline at end of command above
