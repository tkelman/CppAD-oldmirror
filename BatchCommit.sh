# ! /bin/bash
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
