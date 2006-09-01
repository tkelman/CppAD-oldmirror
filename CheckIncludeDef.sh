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
echo "Differences between include file names and directives"
echo "-----------------------------------------------------"
#
grep '^# *ifndef *CppAD[a-zA-Z]*Included$' \
	CppAD/local/*.h \
	CppAD/*.h \
	Example/*.h \
	| sort -u > junk.1
sed -e 's%\([^.]*\)\.h:.*%\1%' -e 's%^.*/%%' < junk.1 > junk.2
sed -e 's%.*# *ifndef *CppAD\([a-zA-Z]*\)Included$%\1%' < junk.1 > junk.3
diff junk.2 junk.3
#
grep '^# *define *CppAD[a-zA-Z]*Included$' \
	CppAD/local/*.h \
	CppAD/*.h \
	Example/*.h \
	| sort -u > junk.1
sed -e 's%\([^.]*\)\.h:.*%\1%' -e 's%^.*/%%' < junk.1 > junk.2
sed -e 's%.*# *define *CppAD\([a-zA-Z]*\)Included$%\1%' < junk.1 > junk.3
diff junk.2 junk.3
echo "-----------------------------------------------------"
echo "Nothing should be between the two dashed lines above"
