# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# This program is free software; you can use it under the terms of the 
#	         Common Public License Version 1.0.
# You should have received a copy of the this license along with this program.
# ----------------------------------------------------------------------
log_entry="Change CppAD/local/*_.hpp (temporary file name) to 
CppAD/local/*.hpp (desired file name).
"
list1=`ls CppAD/local/*.hpp | sed -e 's|\.hpp|_.hpp|'` 
list2=`ls CppAD/local/*.hpp` 
list3=`ls CppAD/*.h | sed -e '/config.h/d'` 
list4=`ls omh/*.omh` 
change_list="
$list1
$list2
$list3
$list4
Doc.omh
Dev.omh
Makefile.am
"
echo "$log_entry" > SvnCommit.log
echo "svn commit --username bradbell --file SvnCommit.log $change_list"
svn commit --username bradbell --file SvnCommit.log $change_list
