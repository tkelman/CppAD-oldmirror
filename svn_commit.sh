# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
# Define your subversion commit by editing the definition of 
# log_entry, add_list, change_list, delete_list and copy_branch below.
#
# log_entry
# The contents of this variable will be used as the log entry for the commit.
#
# add_list
# List of files that will be added to the repository during this commit.
# Do not use add_list to add directories; use the following instead:
# 	svn add --non-recursive dir
# 	svn commit directory -m "adding directory dir" dir
# where dir is replaced by the name of the directory. 
# Then use add_list to add the files within that directory.
#
# delete_list
# List of files that will be deleted from the repository during this commit.
#
# move_list
# List of files that have been moved in local copy, just the old names. 
# The new file names should go in the change_list.
#
# change_list
# List of files that are currently in repository and change during this commit.
#
# copy_branch
# You may specify up one other branch that should receive a copy of all that 
# changes that you are making with this commit. 
# This other branch cannot be the trunk and you must have a copy of it
# on your local machine. If copy_branch is empty; i.e., copy_branch="", 
# the changes will not be copied (and commited) into another branch.
#
# ----------------------------------------------------------------------
log_entry="Start cleaning up user interface to OpenMP multi-threading.

omp_max_thread.hpp: set maximum number of threads.

svn_commt.sh: file that made this commit.
check_include_def.sh: update to version where all file names are lower case.
makefile.am: include omp_max_thread.hpp and openmp directory in distribution.
whats_new_07.omh: users view of the changes.
ad_fun.hpp: include omp_max_thread.hpp with cppad.hpp and in documentation.
omp_max_thread.hpp: fix # ifdef at beginning of file.
bender_quad.hpp:  fix # ifdef at beginning of file.
ad.hpp: include omh_max_thread in member functions.
preprocessor.hpp:  fix # ifdef at beginning of file.
tape_link.hpp: fix developer doc, # ifdef, and use omh_max_thread.
newton.hpp: move to multi_newton.hpp
multi_newton.hpp: change name to multi_newton (need to add user doc).
multi_newton.cpp: add some omhelp commands at beginning.
newton.cpp: move to multi_newton.cpp
run.sh: delete multi_newton executable after running (so not in distribution).
det_grad_33.hpp: remove space at end of # ifdef.
uniform_01.hpp: remove space at end of # ifdef.
" 
add_list="
	cppad/local/omp_max_thread.hpp
"
delete_list="
"
move_list="
	openmp/newton.hpp
	openmp/newton.cpp
"
#
change_list="
	svn_commit.sh
	check_include_def.sh
	makefile.am
	omh/whats_new_07.omh
	cppad/local/ad_fun.hpp
	cppad/local/bender_quad.hpp
	cppad/local/ad.hpp
	cppad/local/preprocessor.hpp
	cppad/local/tape_link.hpp
	openmp/multi_newton.cpp
	openmp/multi_newton.hpp
	openmp/run.sh
	speed/det_grad_33.hpp
	speed/uniform_01.hpp
"
#
copy_branch="" 
# ----------------------------------------------------------------------
if [ "$copy_branch" != "" ]
then
	if [ ! -d ../branches/$copy_branch/.svn ]
	then
		echo "../branches/$copy_branch/.svn is not a directory"
	fi
fi
this_branch=`pwd | sed -e "s|.*/CppAD/||"`
echo "$this_branch: $log_entry" > svn_commit.log
count=0
bad_name=""
for file in $add_list $change_list
do
	count=`expr $count + 1`
	ext=`echo $file | sed -e "s/.*\././"`
	if \
	[ -f $file            ]      &&   \
	[ $file != "svn_commit.sh" ] &&   \
	[ $ext  != ".sed"     ]      &&   \
	[ $ext  != ".vcproj"  ]      &&   \
	[ $ext  != ".sln"     ]      &&   \
	[ $ext  != ".vim"     ]
	then
		# automatic edits and backups
		echo "cp $file junk.$count"
		cp $file junk.$count
		sed -f svn_commit.sed < junk.$count > $file
		diff junk.$count $file
		if [ "$ext" == ".sh" ]
		then
			chmod +x $file
		fi
	fi
	if [ ! -e $file ]
	then
		bad_name="$file"
		echo "$file is not a file or directory not exist"
	fi
done
for file in $add_list
do
	echo "svn add $file ?"
	if [ "$copy_branch" != "" ]
	then
		echo "svn add ../branches/$copy_branch/$file ?"
	fi
done
for file in $change_list
do
	if [ "$copy_branch" != "" ]
	then
		echo "cp $file ../branches/$copy_branch/$file ?"
	fi
done
for file in $delete_list $move_list
do
	echo "svn delete $file ?"
	if [ "$copy_branch" != "" ]
	then
		echo "svn delete ../branches/$copy_branch/$file ?"
	fi
done
echo "-------------------------------------------------------------"
cat svn_commit.log
response=""
if [ "$bad_name" != "" ]
then
	echo "bad file or directory name: $bad_name"
	response="a"
else
	while [ "$response" != "c" ] && [ "$response" != "a" ]
	do
		read -p "continue [c] or abort [a] ? " response
	done
fi
if [ "$response" = "a" ]
then
	echo "aborting commit"
	count=0
	for file in $add_list $change_list
	do
		count=`expr $count + 1`
		ext=`echo $file | sed -e "s/.*\././"`
		if \
		[ -f $file            ] && \
		[ $ext  != ".sh"      ] && \
		[ $ext  != ".sed"     ] && \
		[ $ext  != ".vcproj"  ] && \
		[ $ext  != ".sln"     ] && \
		[ $ext  != ".vim"     ]
		then
			# undo the automatic edits
			echo "mv junk.$count $file"
			mv junk.$count $file
			if [ "$ext" == ".sh" ]
			then
				chmod +x $file
			fi
		fi
	done
	exit 
fi
echo "continuing commit"
#
for file in $add_list
do
	svn add $file
done
for file in $delete_list
do
	svn delete $file
done
copy_list=""
if [ "$copy_branch" != "" ]
then
	for file in $add_list
	do
		target="../branches/$copy_branch/$file"
		cp $file $target
		svn add $target
		copy_list="$copy_list $target"
	done
	for file in $change_list
	do
		target="../branches/$copy_branch/$file"
		echo "cp $file $target"
		cp $file $target
		copy_list="$copy_list $target"
	done
	for file in $delete_list $move_list
	do
		svn delete $target
		target="../branches/$copy_branch/$file"
		copy_list="$copy_list $target"
	done
	
fi
svn commit --username bradbell --file svn_commit.log $add_list $change_list $delete_list $move_list $copy_list
