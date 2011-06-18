# Changes to be made at each commit (with the exception of */makefile.in)
# Note that bin/commit.sh and bin/commit.sed are also excluded.
#
# Make sure config.h has certian default values (Used by Microsoft install)
s/^\(# *define *CPPAD_BOOSTVECTOR\).*$/\1 0/
s/^\(# *define *CPPAD_CPPADVECTOR\).*$/\1 1/
s/^\(# *define *CPPAD_STDVECTOR\).*$/\1 0/
s/^\(# *define *CPPAD_TAPE_ADDR_TYPE\).*$/\1 unsigned int/
#
# Delete short copyright comment (it is obvious)
/BEGIN SHORT COPYRIGHT/d
/END SHORT COPYRIGHT/d
#
# improve certain wording
s/an example and a test/an example and test/
#
# Delete statement of where the file is (it is obvious)
/^\$comment This file is in the [^$]*directory\$\$ *$/d
#
# Change copyright second year to current year
s/Copyright (C) \([0-9]*\)-[0-9][0-9] Bradley M. Bell/Copyright (C) \1-11 Bradley M. Bell/
s/Copyright (C) \([0-9]*\)-20[0-9][0-9] Bradley M. Bell/Copyright (C) \1-11 Bradley M. Bell/
